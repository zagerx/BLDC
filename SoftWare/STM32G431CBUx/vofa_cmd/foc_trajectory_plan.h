#ifndef FOC_TRAJECTORY_PLAN_H
#define FOC_TRAJECTORY_PLAN_H

#include "device.h"
#include <stdint.h>
#include <stdbool.h>

/* 状态枚举定义 */
typedef enum
{
    TRAJ_ACTOR_STATE_START = 0,
    TRAJ_ACTOR_STATE_T1,   // 加加速阶段
    TRAJ_ACTOR_STATE_T2,   // 匀加速阶段
    TRAJ_ACTOR_STATE_T3,   // 减加速阶段
    TRAJ_ACTOR_STATE_T4,   // 匀速阶段
    TRAJ_ACTOR_STATE_T5,   // 加减速阶段
    TRAJ_ACTOR_STATE_T6,   // 匀减速阶段
    TRAJ_ACTOR_STATE_T7,   // 减减速阶段
    TRAJ_ACTOR_STATE_END,  // 运动结束
    TRAJ_ACTOR_STATE_IDLE, // 空闲状态
} trajectory_actor_state_t;

/* 规划器配置结构体 */
struct s_curve_planner_config
{
    float acc;           /* 最大加速度 */
    float ja;            /* 最大加加速度 (jerk) */
    float vel_max;       /* 最大速度 */
    float pos_tolerance; /* 位置容差 */
    float min_acc;       /* 最小加速度 (负向最大) */
    float min_jerk;      /* 最小加加速度 (负向最大) */
};

/* 规划器数据/状态结构体 */
struct s_curve_planner_data
{
    trajectory_actor_state_t actor_state; /* 当前执行状态 */

    /* 时间参数 */
    uint16_t tau;       /* 时间参数 */
    uint16_t Ts[8];     /* 各阶段时间分配: T1~T7 */
    float elapsed_time; /* 已过时间 */
    float total_time;   /* 总运动时间 */

    /* 速度参数 */
    float V[8];        /* 各阶段结束速度 */
    float current_vel; /* 当前速度 */
    float target_vel;  /* 目标速度 */

    /* 位置参数 */
    float current_pos; /* 当前位置 */
    float target_pos;  /* 目标位置 */
    float last_target; /* 上次目标值 */
    float cur_output;  /* 当前输出值 (位置) */

    /* 加速度/加加速度参数 */
    float acc;  /* 当前加速度 */
    float jerk; /* 当前加加速度 */

    /* 方向与标志 */
    float direction;        /* 运动方向: +1 或 -1 */
    bool planning_complete; /* 规划完成标志 */
};

/* API接口函数声明 */
int s_planner_init(struct device *dev, float start_pos);
int s_planner_plan(struct device *dev, float target_pos);
float s_planner_action(struct device *dev, float dt);
trajectory_actor_state_t s_planner_get_state(struct device *dev);
void s_planner_reset(struct device *dev, float start_pos);

#endif /* FOC_TRAJECTORY_PLAN_H */