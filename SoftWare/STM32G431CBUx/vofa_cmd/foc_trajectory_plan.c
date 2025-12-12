#include "foc_trajectory_plan.h"
#include <math.h>
#include <string.h>

/* 静态函数声明 */
static bool validate_parameters(struct s_curve_planner_config *conf);
static void calculate_s_curve_profile(struct device *dev);
static void update_planner_state(struct s_curve_planner_data *data, float dt);

/* 检查参数有效性 */
static bool validate_parameters(struct s_curve_planner_config *conf)
{
    if (conf->acc <= 0 || conf->ja <= 0 || conf->vel_max <= 0)
    {
        return false;
    }
    if (conf->pos_tolerance <= 0)
    {
        conf->pos_tolerance = 0.001f;
    }

    /* 如果没有设置min_acc/min_jerk，则使用对称值 */
    if (conf->min_acc >= 0)
    {
        conf->min_acc = -conf->acc;
    }
    if (conf->min_jerk >= 0)
    {
        conf->min_jerk = -conf->ja;
    }

    return true;
}

/* 计算S型曲线规划 */
// 修正后的S型曲线规划计算函数
static void calculate_s_curve_profile(struct device *dev)
{
    struct s_curve_planner_config *conf = (struct s_curve_planner_config *)dev->config;
    struct s_curve_planner_data *data = (struct s_curve_planner_data *)dev->data;

    float q0 = data->current_pos; // 当前位置 (m)
    float q1 = data->target_pos;  // 目标位置 (m)
    float v0 = data->current_vel; // 当前速度 (m/s)
    float v_max = conf->vel_max;  // 配置的最大速度 (m/s)
    float a_max = conf->acc;      // 最大加速度 (m/s²)
    float j_max = conf->ja;       // 最大加加速度 (m/s³)

    // 1. 计算总位移和方向
    float s = q1 - q0; // 位移 (m)
    data->direction = (s >= 0) ? 1.0f : -1.0f;
    s = fabs(s);

    // 2. 检查微小位移
    if (s < conf->pos_tolerance)
    {
        data->planning_complete = true;
        data->actor_state = TRAJ_ACTOR_STATE_END;
        return;
    }

    // 3. 计算加加速度阶段时间
    float Tj = a_max / j_max; // 达到最大加速度所需时间 (s)

    // 4. 计算能达到的最大速度（标准S曲线公式）
    // 首先检查是否受限于位移
    float s_min_no_const = 2.0f * (0.5f * j_max * Tj * Tj * Tj +
                                   a_max * Tj * Tj); // 无匀速段的最小位移

    float v_lim; // 实际能达到的最大速度

    if (s >= s_min_no_const + v_max * Tj)
    {
        // 情况1：位移足够大，能达到配置的最大速度
        v_lim = v_max;
    }
    else
    {
        // 情况2：位移限制，需要计算实际能达到的最大速度
        // 使用标准S曲线公式求解
        v_lim = sqrt((j_max * j_max * Tj * Tj * Tj * Tj +
                      4.0f * a_max * a_max * Tj * Tj +
                      4.0f * j_max * s) /
                     4.0f) -
                (j_max * Tj * Tj) / 2.0f;

        // 确保不超过配置的最大速度
        if (v_lim > v_max)
        {
            v_lim = v_max;
        }
    }

    // 应用方向
    v_lim *= data->direction;

    // 5. 计算各阶段时间（基于标准S曲线7段模型）
    // T1: 加加速阶段
    data->Ts[0] = Tj * 1000; // 转换为ms存储

    // 计算匀加速段时间
    float delta_v = fabs(v_lim) - fabs(v0);
    float Ta; // 总加速时间

    if (delta_v <= j_max * Tj * Tj)
    {
        // 无法达到最大加速度的情况
        float Tj_adj = sqrt(delta_v / j_max);
        data->Ts[0] = Tj_adj * 1000;
        data->Ts[1] = 0; // 无匀加速段
        data->Ts[2] = Tj_adj * 1000;
        Ta = 2.0f * Tj_adj;
    }
    else
    {
        // 能达到最大加速度的情况
        data->Ts[0] = Tj * 1000;
        float t_const = (delta_v - j_max * Tj * Tj) / a_max;
        data->Ts[1] = t_const * 1000; // 匀加速段时间
        data->Ts[2] = Tj * 1000;
        Ta = 2.0f * Tj + t_const;
    }

    // 6. 计算减速段时间（对称，假设末速度为0）
    float delta_v_dec = fabs(v_lim); // 从v_lim减速到0
    float Td;                        // 总减速时间

    if (delta_v_dec <= j_max * Tj * Tj)
    {
        float Tj_dec = sqrt(delta_v_dec / j_max);
        data->Ts[4] = Tj_dec * 1000;
        data->Ts[5] = 0;
        data->Ts[6] = Tj_dec * 1000;
        Td = 2.0f * Tj_dec;
    }
    else
    {
        data->Ts[4] = Tj * 1000;
        float t_const_dec = (delta_v_dec - j_max * Tj * Tj) / a_max;
        data->Ts[5] = t_const_dec * 1000;
        data->Ts[6] = Tj * 1000;
        Td = 2.0f * Tj + t_const_dec;
    }

    // 7. 计算加速段和减速段位移
    float s_acc = fabs(v0) * Ta +
                  (1.0f / 6.0f) * j_max * (data->Ts[0] / 1000.0f) * (data->Ts[0] / 1000.0f) * (data->Ts[0] / 1000.0f) +
                  0.5f * a_max * (data->Ts[1] / 1000.0f) * (data->Ts[1] / 1000.0f) +
                  (1.0f / 6.0f) * j_max * (data->Ts[2] / 1000.0f) * (data->Ts[2] / 1000.0f) * (data->Ts[2] / 1000.0f);

    float s_dec = fabs(v_lim) * Td -
                  (1.0f / 6.0f) * j_max * (data->Ts[4] / 1000.0f) * (data->Ts[4] / 1000.0f) * (data->Ts[4] / 1000.0f) -
                  0.5f * a_max * (data->Ts[5] / 1000.0f) * (data->Ts[5] / 1000.0f) -
                  (1.0f / 6.0f) * j_max * (data->Ts[6] / 1000.0f) * (data->Ts[6] / 1000.0f) * (data->Ts[6] / 1000.0f);

    // 8. 计算匀速段时间
    float s_const = s - (s_acc + s_dec);
    if (s_const > 0)
    {
        data->Ts[3] = (s_const / fabs(v_lim)) * 1000.0f;
    }
    else
    {
        data->Ts[3] = 0;
        // 重新调整v_lim（如果需要，可以递归调整）
        // 简化处理：这里保持现有计算
    }

    // 9. 计算总时间和各阶段结束速度
    data->total_time = (data->Ts[0] + data->Ts[1] + data->Ts[2] +
                        data->Ts[3] + data->Ts[4] + data->Ts[5] + data->Ts[6]) /
                       1000.0f;

    // 设置各阶段结束速度
    data->V[0] = v0;
    data->V[1] = v0 + data->direction * 0.5f * j_max * (data->Ts[0] / 1000.0f) * (data->Ts[0] / 1000.0f);
    data->V[2] = data->V[1] + data->direction * a_max * (data->Ts[1] / 1000.0f);
    data->V[3] = data->V[2] + data->direction * (a_max * (data->Ts[2] / 1000.0f) -
                                                 0.5f * j_max * (data->Ts[2] / 1000.0f) * (data->Ts[2] / 1000.0f));
    data->V[4] = v_lim;
    data->V[5] = v_lim - data->direction * 0.5f * j_max * (data->Ts[4] / 1000.0f) * (data->Ts[4] / 1000.0f);
    data->V[6] = data->V[5] - data->direction * a_max * (data->Ts[5] / 1000.0f);
    data->V[7] = 0;

    // 10. 更新状态
    data->planning_complete = false;
    data->actor_state = TRAJ_ACTOR_STATE_START;
    data->target_vel = v_lim;
    data->elapsed_time = 0;
}
/* 更新规划器状态 */
static void update_planner_state(struct s_curve_planner_data *data, float dt)
{
    if (data->actor_state == TRAJ_ACTOR_STATE_IDLE ||
        data->actor_state == TRAJ_ACTOR_STATE_END)
    {
        return;
    }

    /* 更新已过时间 */
    data->elapsed_time += dt;

    /* 计算当前处于哪个阶段 */
    float accumulated_time = 0;
    int current_stage = 0;

    for (int i = 0; i < 7; i++)
    {
        float stage_time = data->Ts[i] / 1000.0f; /* 转换为秒 */
        accumulated_time += stage_time;

        if (data->elapsed_time <= accumulated_time)
        {
            current_stage = i;
            break;
        }
    }

    /* 如果超过总时间，则进入结束状态 */
    if (data->elapsed_time >= data->total_time)
    {
        data->actor_state = TRAJ_ACTOR_STATE_END;
        data->planning_complete = true;
        data->current_pos = data->target_pos;
        data->current_vel = 0;
        data->acc = 0;
        data->jerk = 0;
        return;
    }

    /* 更新状态 */
    data->actor_state = (trajectory_actor_state_t)(TRAJ_ACTOR_STATE_T1 + current_stage);

    /* 计算当前阶段内的相对时间 */
    float stage_start_time = 0;
    for (int i = 0; i < current_stage; i++)
    {
        stage_start_time += data->Ts[i] / 1000.0f;
    }
    float stage_rel_time = data->elapsed_time - stage_start_time;
    float stage_duration = data->Ts[current_stage] / 1000.0f;

    /* 根据当前阶段计算位置、速度、加速度、加加速度 */
    float t = stage_rel_time;
    float Tj = data->Ts[0] / 1000.0f; /* 加加速度阶段时间 */

    switch (current_stage)
    {
    case 0: /* T1: 加加速阶段 */
        data->jerk = (data->direction > 0) ? data->V[1] / (0.5f * Tj * Tj) : -data->V[1] / (0.5f * Tj * Tj);
        data->acc = data->jerk * t;
        data->current_vel = data->V[0] + 0.5f * data->jerk * t * t;
        data->current_pos += data->current_vel * dt;
        break;

    case 1: /* T2: 匀加速阶段 */
        data->jerk = 0;
        data->acc = (data->V[2] - data->V[1]) / (stage_duration);
        data->current_vel = data->V[1] + data->acc * t;
        data->current_pos += data->current_vel * dt;
        break;

    case 2: /* T3: 减加速阶段 */
        data->jerk = (data->V[3] - data->V[2] - data->acc * Tj) / (0.5f * Tj * Tj);
        data->acc = data->acc + data->jerk * t;
        data->current_vel = data->V[2] + data->acc * t + 0.5f * data->jerk * t * t;
        data->current_pos += data->current_vel * dt;
        break;

    case 3: /* T4: 匀速阶段 */
        data->jerk = 0;
        data->acc = 0;
        data->current_vel = data->V[3];
        data->current_pos += data->current_vel * dt;
        break;

    case 4: /* T5: 加减速阶段 */
        data->jerk = (data->V[5] - data->V[4]) / (0.5f * Tj * Tj);
        data->acc = data->jerk * t;
        data->current_vel = data->V[4] + 0.5f * data->jerk * t * t;
        data->current_pos += data->current_vel * dt;
        break;

    case 5: /* T6: 匀减速阶段 */
        data->jerk = 0;
        data->acc = (data->V[6] - data->V[5]) / stage_duration;
        data->current_vel = data->V[5] + data->acc * t;
        data->current_pos += data->current_vel * dt;
        break;

    case 6: /* T7: 减减速阶段 */
        data->jerk = (data->V[7] - data->V[6] - data->acc * Tj) / (0.5f * Tj * Tj);
        data->acc = data->acc + data->jerk * t;
        data->current_vel = data->V[6] + data->acc * t + 0.5f * data->jerk * t * t;
        data->current_pos += data->current_vel * dt;
        break;

    default:
        break;
    }

    /* 更新输出值 */
    data->cur_output = data->current_pos;
}

/* 初始化规划器 */
int s_planner_init(struct device *dev, float start_pos)
{
    if (!dev || !dev->data)
        return -1;

    struct s_curve_planner_data *data = (struct s_curve_planner_data *)dev->data;

    /* 初始化数据结构 */
    memset(data, 0, sizeof(struct s_curve_planner_data));

    data->actor_state = TRAJ_ACTOR_STATE_IDLE;
    data->current_pos = start_pos;
    data->last_target = start_pos;
    data->cur_output = start_pos;
    data->current_vel = 0;
    data->acc = 0;
    data->jerk = 0;
    data->direction = 1.0f;
    data->planning_complete = true;
    data->elapsed_time = 0;
    data->total_time = 0;

    return 0;
}

/* 规划器 */
int s_planner_plan(struct device *dev, float target_pos)
{
    if (!dev || !dev->config || !dev->data)
    {
        return -1;
    }

    struct s_curve_planner_config *conf = (struct s_curve_planner_config *)dev->config;
    struct s_curve_planner_data *data = (struct s_curve_planner_data *)dev->data;

    if (!validate_parameters(conf))
    {
        return -2;
    }

    /* 检查是否需要重新规划 */
    float pos_diff = fabs(target_pos - data->current_pos);
    if (pos_diff < conf->pos_tolerance && data->actor_state == TRAJ_ACTOR_STATE_IDLE)
    {
        return 0;
    }

    /* 设置新的目标位置 */
    data->target_pos = target_pos;
    data->last_target = data->cur_output;

    /* 执行S型曲线规划 */
    calculate_s_curve_profile(dev);

    return 0;
}

/* 执行器 */
float s_planner_action(struct device *dev, float dt)
{
    if (!dev || !dev->data || dt <= 0)
    {
        return 0.0f;
    }

    struct s_curve_planner_data *data = (struct s_curve_planner_data *)dev->data;

    if (data->actor_state == TRAJ_ACTOR_STATE_IDLE ||
        data->actor_state == TRAJ_ACTOR_STATE_END)
    {
        return data->cur_output;
    }

    /* 如果还在规划中，先计算规划 */
    if (data->actor_state == TRAJ_ACTOR_STATE_START)
    {
        /* 开始执行 */
        data->actor_state = TRAJ_ACTOR_STATE_T1;
    }

    /* 更新规划器状态 */
    update_planner_state(data, dt);

    return data->cur_output;
}

/* 获取规划器状态 */
trajectory_actor_state_t s_planner_get_state(struct device *dev)
{
    if (!dev || !dev->data)
    {
        return TRAJ_ACTOR_STATE_IDLE;
    }
    struct s_curve_planner_data *data = (struct s_curve_planner_data *)dev->data;
    return data->actor_state;
}

/* 重置规划器 */
void s_planner_reset(struct device *dev, float start_pos)
{
    if (!dev || !dev->data)
        return;

    struct s_curve_planner_data *data = (struct s_curve_planner_data *)dev->data;

    data->actor_state = TRAJ_ACTOR_STATE_IDLE;
    data->current_pos = start_pos;
    data->target_pos = start_pos;
    data->last_target = start_pos;
    data->cur_output = start_pos;
    data->current_vel = 0.0f;
    data->acc = 0.0f;
    data->jerk = 0.0f;
    data->planning_complete = true;
    data->elapsed_time = 0;
    data->total_time = 0;
}