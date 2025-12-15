#include "foc_trajectory_plan.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
int16_t test_ret;
int16_t test_flag = 3;

static void traj_exec_init(s_traj_exec_data_t *d, float start_pos, float start_vel,
			   float start_acc);
static void traj_exec_step(s_traj_exec_data_t *d, const float dt);
static void traj_exec_load(s_traj_exec_data_t *d, const s_traj_seg_t *segs, int seg_cnt);
static s_traj_plan_status_t traj_plan(s_traj_plan_data_t *d);
static void traj_plan_init(s_traj_plan_data_t *d, float acc_max, float v_max, float jerk_max,
			   float exex_cycle);
static bool traj_plan_seven_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				    float dir);
// 该函数只会在系统上电之后调用一次
int s_planner_init(struct device *dev, float start_pos, float start_vel, float start_acc,
		   float exex_cycle)
{
	s_tarj_data_t *data = dev->data;
	s_traj_plan_data_t *plan = &data->plan_data;
	s_traj_exec_data_t *exec_data = &(data->exec_data);
	traj_exec_init(exec_data, start_pos, start_vel, start_acc);

	s_tarj_config_t *conf = dev->config;
	traj_plan_init(plan, conf->acc, conf->vmax, conf->jerk, exex_cycle);
	return 0;
}

void s_planner_action(struct device *dev, const float dt)
{
	s_tarj_data_t *data = dev->data;
	s_traj_exec_data_t *exec = &data->exec_data;

	if (exec->state != TRAJ_EXEC_RUNNING) {
		return;
	}

	traj_exec_step(exec, dt);
}

s_traj_plan_status_t s_planner_update_target(struct device *dev, float new_target_pos)
{
	s_tarj_data_t *d = dev->data;
	s_traj_plan_data_t *plan = &d->plan_data;
	s_traj_plan_output_t *plan_out = &(plan->plan_out);
	s_traj_exec_data_t *exec = &d->exec_data;
	s_traj_plan_status_t ret;

	if (fabsf(new_target_pos - plan->pre_pos) < 1e-4f) {
		return TRAJ_PLAN_NOT_NEEDED;
	}
	plan->pre_pos = new_target_pos;

	plan->plan_in.a0 = exec->acc;
	plan->plan_in.v0 = exec->vel;
	plan->plan_in.p0 = exec->pos;

	plan->plan_in.target_pos = new_target_pos;
	ret = traj_plan(plan);
	test_ret = ret;
	if (ret != TRAJ_PLAN_OK) {
		return ret;
	}

	traj_exec_load(exec, plan_out->segs, plan_out->seg_cnt);
	return TRAJ_PLAN_OK;
}
static s_traj_plan_status_t traj_plan(s_traj_plan_data_t *d)
{
	if (!d) {
		return TRAJ_PLAN_ERR_PARAM;
	}

	s_traj_plan_input_t *in = &d->plan_in;
	s_traj_plan_output_t *out = &d->plan_out;

	/* ================= 1. 基本参数检查 ================= */
	if (in->j_max <= 0.0f || in->a_max <= 0.0f || in->v_max <= 0.0f) {
		return TRAJ_PLAN_ERR_PARAM;
	}
	test_flag = 20;

	float dp = in->target_pos - in->p0;
	if (fabsf(dp) < 1e-6f) {
		out->seg_cnt = 0;
		return TRAJ_PLAN_OK;
	}
	test_flag = 21;
	/* ================= 2. 方向统一 ================= */
	float dir = (dp >= 0.0f) ? 1.0f : -1.0f;

	float v0 = in->v0;
	float a0 = in->a0;

	/* ================= 3. 状态约束检查 ================= */
	if (fabsf(v0) > in->v_max + 0.2f || fabsf(a0) > in->a_max + 0.2f) {
		test_flag = 22;
		return TRAJ_PLAN_ERR_PARAM;
	}
	test_flag = 2;

	/* ================= 4. 轨迹类型判定（冻结逻辑） ================= */
	trajectory_type_t type;

	/* ================= 5. 清空输出 ================= */
	out->seg_cnt = 0;

	/* ================= 6. 调用对应规划器 ================= */
	bool ok = false;
	type = TRAJ_TYPE_SEVEN_SEGMENT;
	switch (type) {

	case TRAJ_TYPE_SEVEN_SEGMENT:
		ok = traj_plan_seven_segment(in, out, dir);
		break;

	case TRAJ_TYPE_FIVE_SEGMENT:
		ok = traj_plan_five_segment(in, out, dir);
		break;

	case TRAJ_TYPE_THREE_SEGMENT:
		ok = traj_plan_three_segment(in, out, dir);
		break;

	default:
		return TRAJ_PLAN_ERR_NO_SOLUTION;
	}

	if (!ok || out->seg_cnt <= 0) {
		return TRAJ_PLAN_ERR_NO_SOLUTION;
	}

	return TRAJ_PLAN_OK;
}
static bool traj_plan_seven_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				    float dir)
{

	float j = in->j_max;
	float a_max = in->a_max;
	float v_max = in->v_max;

	// 获取当前状态（保持原始符号）
	float v0 = in->v0;
	float a0 = in->a0;

	// 计算总位移（带符号）
	float dp_total = in->target_pos - in->p0;

	/* === 1. 将状态转换到规划方向 === */
	// 将速度和加速度投影到规划方向上
	float v0_dir = v0 * dir; // 在规划方向上的速度分量
	float a0_dir = a0 * dir; // 在规划方向上的加速度分量

	/* === 2. 计算加速段各段时间 === */
	// t1: 加速度从a0_dir增加到a_max所需时间
	float t1 = (a_max - a0_dir) / j;
	if (t1 < 0) {
		// 如果初始加速度已经超过最大加速度，需要先减速
		// 但根据我们的假设，这种情况不应该出现
		t1 = 0;
	}

	// 计算t1段结束时的速度
	float v1 = v0_dir + a0_dir * t1 + 0.5f * j * t1 * t1;

	// t3: 加速度从a_max减到0的时间
	float t3 = a_max / j;

	// 计算没有t2段时能达到的速度
	float v_without_t2 = v1 + a_max * t3 - 0.5f * j * t3 * t3;

	// t2: 匀加速段时间
	float t2 = (v_max - v_without_t2) / a_max;

	// 确保所有时间都为正
	if (t1 < 1e-6f) {
		t1 = 0;
	}
	if (t2 < 1e-6f) {
		t2 = 0;
	}
	if (t3 < 1e-6f) {
		t3 = 0;
	}

	/* === 3. 计算减速段各段时间 === */
	// t5: 加速度从0增加到-a_max的时间（负方向加速）
	float t5 = a_max / j;

	// t7: 加速度从-a_max减到0的时间
	float t7 = a_max / j;

	// t6: 匀减速段时间
	// 减速段速度变化：从v_max减到0
	// 速度方程：v_max = a_max * t5 + a_max * t6 + a_max * t7
	// 注意：t5和t7段的速度变化量分别为0.5*a_max*t5和0.5*a_max*t7
	// 所以：v_max = 0.5*a_max*t5 + a_max*t6 + 0.5*a_max*t7
	// 由于t5 = t7，所以：v_max = a_max*t5 + a_max*t6
	float t6 = (v_max - a_max * t5) / a_max;

	// 确保减速段时间都为正
	if (t5 < 1e-6f) {
		t5 = 0;
	}
	if (t6 < 1e-6f) {
		t6 = 0;
	}
	if (t7 < 1e-6f) {
		t7 = 0;
	}

	/* === 4. 计算各段位移 === */
	// 加速段位移
	float s1 = v0_dir * t1 + 0.5f * a0_dir * t1 * t1 + (1.0f / 6.0f) * j * t1 * t1 * t1;
	float v1_actual = v0_dir + a0_dir * t1 + 0.5f * j * t1 * t1;
	float s2 = v1_actual * t2 + 0.5f * a_max * t2 * t2;
	float v2 = v1_actual + a_max * t2;
	float s3 = v2 * t3 + 0.5f * a_max * t3 * t3 - (1.0f / 6.0f) * j * t3 * t3 * t3;
	float s_acc = s1 + s2 + s3;

	// 减速段位移
	float s5 = v_max * t5 - 0.5f * a_max * t5 * t5 + (1.0f / 6.0f) * j * t5 * t5 * t5;
	float v5 = v_max - a_max * t5 + 0.5f * j * t5 * t5;
	float s6 = v5 * t6 - 0.5f * a_max * t6 * t6;
	float v6 = v5 - a_max * t6;
	float s7 = v6 * t7 - 0.5f * a_max * t7 * t7 - (1.0f / 6.0f) * j * t7 * t7 * t7;
	float s_dec = s5 + s6 + s7;

	// 计算总位移
	float total_distance = fabsf(dp_total);

	// t4: 匀速段时间
	float t4 = (total_distance - s_acc - s_dec) / v_max;

	// 确保匀速段时间为正
	if (t4 < 1e-6f) {
		t4 = 0;
	}

	/* === 5. 验证计算结果 === */
	// 验证总位移
	float calculated_distance = s_acc + v_max * t4 + s_dec;
	if (fabsf(calculated_distance - total_distance) > 1e-3f) {
		// 位移不匹配，说明规划有误
		return false;
	}

	// 验证最终速度（应为0）
	float final_vel = v6 - a_max * t7 + 0.5f * j * t7 * t7;
	if (fabsf(final_vel) > 1e-3f) {
		// 最终速度不为0
		return false;
	}

	/* === 6. 填充轨迹段 === */
	int i = 0;
	float j_dir = j * dir;

	// 加速段
	out->segs[i++] = (s_traj_seg_t){t1, +j_dir}; // 加加速度段
	out->segs[i++] = (s_traj_seg_t){t2, 0};      // 匀加速段
	out->segs[i++] = (s_traj_seg_t){t3, -j_dir}; // 减加速度段

	// 匀速段
	out->segs[i++] = (s_traj_seg_t){t4, 0};

	// 减速段
	out->segs[i++] = (s_traj_seg_t){t5, -j_dir}; // 加减速段
	out->segs[i++] = (s_traj_seg_t){t6, 0};      // 匀减速段
	out->segs[i++] = (s_traj_seg_t){t7, +j_dir}; // 减减速段

	out->seg_cnt = i;

	return true;
}
// static bool traj_plan_seven_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
// 									float dir)
// {
// 	float j = in->j_max;
// 	float aL = in->a_max;
// 	float vL = in->v_max;

// 	float v0 = fabsf(in->v0);
// 	float a0 = fabsf(in->a0);

// 	/* === 1. 加速侧 === */
// 	float t1 = (aL - a0) / j;
// 	float t3 = t1;

// 	// 计算 t1 段结束时的速度
// 	float v1 = v0 + a0 * t1 + 0.5f * j * t1 * t1;
// 	// 计算 t3 段开始时的速度（如果没有 t2 段）
// 	float v_without_t2 = v1 + aL * t3 - 0.5f * j * t3 * t3;
// 	// 计算需要的 t2 段
// 	float t2 = (vL - v_without_t2) / aL;
// 	/* === 3. 位移计算 === */
// 	float s1 = v0 * t1 + 0.5f * a0 * t1 * t1 + (1.0f / 6.0f) * j * t1 * t1 * t1;
// 	float s2 = v1 * t2 + 0.5f * aL * t2 * t2;
// 	float v2 = v1 + aL * t2;
// 	float s3 = v2 * t3 + 0.5f * aL * t3 * t3 - (1.0f / 6.0f) * j * t3 * t3 * t3;

// 	float s_acc = s1 + s2 + s3;
// 	float s_dec = s_acc;
// 	float dp = (in->target_pos - in->p0) * dir;
// 	float t4 = (dp - s_acc - s_dec) / vL;

// 	/* === 4. 填充段 === */
// 	int i = 0;
// 	float j_dir = j * dir;
// 	out->segs[i++] = (s_traj_seg_t){t1, +j_dir};
// 	out->segs[i++] = (s_traj_seg_t){t2, 0};
// 	out->segs[i++] = (s_traj_seg_t){t3, -j_dir};
// 	out->segs[i++] = (s_traj_seg_t){t4, 0};
// 	out->segs[i++] = (s_traj_seg_t){t1, -j_dir};
// 	out->segs[i++] = (s_traj_seg_t){t2, 0};
// 	out->segs[i++] = (s_traj_seg_t){t3, +j_dir};

// 	out->seg_cnt = i;
// 	return true;
// }

static void traj_plan_init(s_traj_plan_data_t *d, float acc_max, float v_max, float jerk_max,
			   float exex_cycle)
{
	d->plan_in.a_max = acc_max;
	d->plan_in.v_max = v_max;
	d->plan_in.j_max = jerk_max;
	d->plan_in.t_min = exex_cycle * 4.0f;
}
static void traj_exec_init(s_traj_exec_data_t *d, float start_pos, float start_vel, float start_acc)
{
	memset(d, 0, sizeof(*d));

	d->pos = start_pos;
	d->vel = start_vel;
	d->acc = start_acc;

	d->state = TRAJ_EXEC_IDLE;
}

static void traj_exec_load(s_traj_exec_data_t *d, const s_traj_seg_t *segs, int seg_cnt)
{
	d->segs = segs;
	d->seg_cnt = seg_cnt;
	d->cur_seg = 0;
	d->seg_time = 0.0f;

	d->state = (seg_cnt > 0) ? TRAJ_EXEC_RUNNING : TRAJ_EXEC_END;
}

static void traj_exec_step(s_traj_exec_data_t *d, const float dt)
{
	if (d->state != TRAJ_EXEC_RUNNING) {
		return;
	}

	float remain_dt = dt;

	while (remain_dt > 0.0f && d->state == TRAJ_EXEC_RUNNING) {

		/* 所有段执行完毕 */
		if (d->cur_seg >= d->seg_cnt) {
			d->state = TRAJ_EXEC_END;
			break;
		}

		const s_traj_seg_t *s = &d->segs[d->cur_seg];

		/* 跳过空段 */
		if (s->t <= 0.0f) {
			d->cur_seg++;
			d->seg_time = 0.0f;
			continue;
		}

		/* 本段剩余时间 */
		float remain_seg = s->t - d->seg_time;
		float dt_use = (remain_dt < remain_seg) ? remain_dt : remain_seg;

		/* 保存旧状态 */
		float acc0 = d->acc;
		float vel0 = d->vel;

		/* 三阶积分公式（jerk 常量） */
		d->acc = acc0 + s->j * dt_use;
		d->vel = vel0 + acc0 * dt_use + 0.5f * s->j * dt_use * dt_use;
		d->pos = d->pos + vel0 * dt_use + 0.5f * acc0 * dt_use * dt_use +
			 (1.0f / 6.0f) * s->j * dt_use * dt_use * dt_use;

		d->seg_time += dt_use;
		remain_dt -= dt_use;

		/* 切换到下一段 */
		if (d->seg_time >= s->t) {
			d->cur_seg++;
			d->seg_time = 0.0f;
		}
	}
}

float s_planner_get_pos(const struct device *dev)
{
	const s_tarj_data_t *data = dev->data;
	return data->exec_data.pos;
}

float s_planner_get_vel(const struct device *dev)
{
	const s_tarj_data_t *data = dev->data;
	return data->exec_data.vel;
}

float s_planner_get_acc(const struct device *dev)
{
	const s_tarj_data_t *data = dev->data;
	return data->exec_data.acc;
}
#if 1

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void test_solve_unconstrained_peak_velocity(void)
{
}

#endif
