#include "foc_trajectory_plan.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
int16_t test_ret;

static void traj_exec_init(s_traj_exec_data_t *d, float start_pos, float start_vel,
			   float start_acc);
static void traj_exec_step(s_traj_exec_data_t *d, const float dt);
static void traj_exec_load(s_traj_exec_data_t *d, const s_traj_seg_t *segs, int seg_cnt);
static s_traj_plan_status_t traj_plan(s_traj_plan_data_t *d);
static void traj_plan_init(s_traj_plan_data_t *d, float acc_max, float v_max, float jerk_max,
			   float exex_cycle);
static bool traj_plan_seven_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				    float dir);
static bool traj_plan_five_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				   float dir);
static bool traj_plan_three_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				    float dir);

trajectory_type_t determine_trajectory_type(const s_traj_plan_input_t *in, float dir);
static float calc_t1_from_jerk(const s_traj_plan_input_t *in);
static float calc_t2_from_acc(const s_traj_plan_input_t *in, float dir);
static float calc_t4_from_vel(const s_traj_plan_input_t *in, float dir);
static inline float traj_pos_jerk(float v0, float a0, float j, float t);
static float calc_dec_side_pos(float v1, float a1, float j, float t7, float t6, float t5);
static float calc_acc_side_pos(float v0, float a0, float j, float t1, float t2, float t3);

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

	if (fabsf(new_target_pos - plan->target_pos) < 1e-4f) {
		return TRAJ_PLAN_NOT_NEEDED;
	}
	plan->target_pos = new_target_pos;

	plan->plan_in.a0 = exec->acc;
	plan->plan_in.v0 = exec->vel;
	plan->plan_in.p0 = exec->pos;

	ret = traj_plan(plan);
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

	float dp = in->p1 - in->p0;
	if (fabsf(dp) < 1e-6f) {
		out->seg_cnt = 0;
		return TRAJ_PLAN_OK;
	}

	/* ================= 2. 方向统一 ================= */
	float dir = (dp >= 0.0f) ? 1.0f : -1.0f;

	float v0 = in->v0 * dir;
	float v1 = in->v1 * dir;
	float a0 = in->a0 * dir;
	float a1 = in->a1 * dir;

	/* ================= 3. 状态约束检查 ================= */
	if (fabsf(v0) > in->v_max + 1e-6f || fabsf(v1) > in->v_max + 1e-6f ||
	    fabsf(a0) > in->a_max + 1e-6f || fabsf(a1) > in->a_max + 1e-6f) {
		return TRAJ_PLAN_ERR_PARAM;
	}

	/* ================= 4. 轨迹类型判定（冻结逻辑） ================= */
	trajectory_type_t type = determine_trajectory_type(in, dir);

	/* ================= 5. 清空输出 ================= */
	out->seg_cnt = 0;

	/* ================= 6. 调用对应规划器 ================= */
	bool ok = false;

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

trajectory_type_t determine_trajectory_type(const s_traj_plan_input_t *in, float dir)
{
	/* ================= 0. 工程参数 ================= */
	const float t_min = in->t_min; // 最小可执行时间（控制周期 / 插补周期）

	/* ================= 1. 理论时间计算（只算判定量） ================= */
	float t1 = calc_t1_from_jerk(in);     // a_max / j_max
	float t2 = calc_t2_from_acc(in, dir); // 是否存在匀加速
	float t4 = calc_t4_from_vel(in, dir); // 是否存在匀速

	/* ================= 2. jerk 可执行性判定 ================= */
	if (t1 < t_min) {
		/*
		 * jerk 斜坡在工程上不可执行
		 * S 曲线模型整体失效
		 */
		return TRAJ_TYPE_INVALID;
	}

	/* ================= 3. 是否退化为数学三段 ================= */
	if (t2 <= t_min) {
		/*
		 * 无法形成匀加速段
		 * 但 jerk 仍然存在
		 */
		return TRAJ_TYPE_THREE_SEGMENT;
	}

	/* ================= 4. 是否退化为五段 ================= */
	if (t4 <= t_min) {
		/*
		 * 无法形成匀速段
		 * t1 / t2 均成立
		 */
		return TRAJ_TYPE_FIVE_SEGMENT;
	}

	/* ================= 5. 完整七段 ================= */
	return TRAJ_TYPE_SEVEN_SEGMENT;
}
static float calc_t1_from_jerk(const s_traj_plan_input_t *in)
{
	/* t1 = a_max / j_max */
	return in->a_max / in->j_max;
}
static float calc_t2_from_acc(const s_traj_plan_input_t *in, float dir)
{
	float v0 = in->v0 * dir;

	/* jerk 斜坡带来的速度增量 */
	float dv_jerk = (in->a_max * in->a_max) / (2.0f * in->j_max);

	/* 剩余可用于匀加速的速度空间 */
	float dv_remain = in->v_max - v0 - dv_jerk;

	/* 匀加速时间 */
	return dv_remain / in->a_max;
}
static float calc_t4_from_vel(const s_traj_plan_input_t *in, float dir)
{
	float v0 = in->v0 * dir;
	float v1 = in->v1 * dir;

	float a = in->a_max;
	float j = in->j_max;
	float vmax = in->v_max;
	float dist = fabsf(in->p1 - in->p0);

	/* jerk 段时间 */
	float t1 = a / j;

	/* jerk 段速度增量 */
	float dv_jerk = a * a / (2.0f * j);

	/* 匀加速段时间（加速侧） */
	float t2 = (vmax - v0 - dv_jerk) / a;

	/* 加速侧位移 */
	float s_acc = v0 * (2.0f * t1 + t2) + a * (t1 * t1 + t1 * t2 + 0.5f * t2 * t2);

	/* 对称计算减速侧 */
	float t6 = (vmax - v1 - dv_jerk) / a;

	float s_dec = v1 * (2.0f * t1 + t6) + a * (t1 * t1 + t1 * t6 + 0.5f * t6 * t6);

	/* 剩余位移用于匀速 */
	float s_remain = dist - s_acc - s_dec;

	/* 匀速时间 */
	return s_remain / vmax;
}

static bool traj_plan_seven_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				    float dir)
{
	float j = in->j_max * dir;
	float aL = in->a_max;
	float vL = in->v_max;

	float v0 = in->v0 * dir;
	float a0 = in->a0 * dir;
	float v1 = in->v1 * dir;
	float a1 = in->a1 * dir;

	/* === 1. 加速侧 === */
	float t1 = (aL - a0) / j;
	float t3 = aL / j;

	if (t1 < 0.0f || t3 < 0.0f) {
		return false;
	}

	float v_after = v0 + a0 * t1 + 0.5f * j * t1 * t1 + aL * t3 - 0.5f * j * t3 * t3;

	float t2 = (vL - v_after) / aL;
	if (t2 < 0.0f) {
		return false;
	}

	/* === 2. 减速侧 === */
	float t7 = (aL - a1) / j;
	float t5 = aL / j;
	float t6 = (vL - v1) / aL;

	if (t6 < 0.0f) {
		return false;
	}

	/* === 3. 位移计算 === */
	float pos_acc = calc_acc_side_pos(v0, a0, j, t1, t2, t3);
	float pos_dec = calc_dec_side_pos(v1, a1, j, t7, t6, t5);

	float dp = (in->p1 - in->p0) * dir;
	float t4 = (dp - pos_acc - pos_dec) / vL;

	if (t4 < 0.0f) {
		return false;
	}

	/* === 4. 填充段 === */
	int i = 0;
	out->segs[i++] = (s_traj_seg_t){t1, +j};
	out->segs[i++] = (s_traj_seg_t){t2, 0};
	out->segs[i++] = (s_traj_seg_t){t3, -j};
	out->segs[i++] = (s_traj_seg_t){t4, 0};
	out->segs[i++] = (s_traj_seg_t){t5, -j};
	out->segs[i++] = (s_traj_seg_t){t6, 0};
	out->segs[i++] = (s_traj_seg_t){t7, +j};

	out->seg_cnt = i;
	return true;
}
/* ==================== 五段规划器 ==================== */
static bool traj_plan_five_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				   float dir)
{
	float v0 = in->v0 * dir;
	float v1 = in->v1 * dir;
	float a0 = in->a0 * dir;
	float a1 = in->a1 * dir;
	float amax = in->a_max;
	float jmax = in->j_max;
	float dist = fabsf(in->p1 - in->p0);

	/* 1. 计算各段时间（没有匀速 t4） */
	float t1 = (amax - a0) / jmax; // 加加
	float t2 = (amax * amax / (2.0f * jmax) + amax * (amax / jmax) +
		    amax / jmax);      // 可以根据实际算法重算
	float t3 = amax / jmax;        // 加减
	float t5 = amax / jmax;        // 减加
	float t6 = (amax - a1) / jmax; // 减减

	/* 2. 计算加速/减速段位移 */
	float s_acc = calc_acc_side_pos(v0, a0, jmax, t1, t2, t3);
	float s_dec = calc_dec_side_pos(v1, a1, jmax, t6, t5, 0.0f); // 五段没有匀速段

	/* 3. 检查总位移 */
	if (s_acc + s_dec > dist) {
		return false; // 不满足五段条件
	}

	/* 4. 填充输出段 */
	int idx = 0;
	out->segs[idx++] = (s_traj_seg_t){t1, dir * jmax};
	out->segs[idx++] = (s_traj_seg_t){t2, 0.0f};
	out->segs[idx++] = (s_traj_seg_t){t3, -dir * jmax};
	out->segs[idx++] = (s_traj_seg_t){t5, -dir * jmax};
	out->segs[idx++] = (s_traj_seg_t){t6, dir * jmax};

	out->seg_cnt = idx;
	return true;
}

/* ==================== 三段规划器 ==================== */
static bool traj_plan_three_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				    float dir)
{
	float v0 = in->v0 * dir;
	float v1 = in->v1 * dir;
	float a0 = in->a0 * dir;
	float a1 = in->a1 * dir;
	float amax = in->a_max;
	float jmax = in->j_max;
	float dist = fabsf(in->p1 - in->p0);

	/* 三段：只有加加/减减，没有匀加/匀速 */
	float t1 = (amax - a0) / jmax; // 加加
	float t3 = (amax - a1) / jmax; // 减减

	float s_acc = calc_acc_side_pos(v0, a0, jmax, t1, 0.0f, 0.0f);
	float s_dec = calc_dec_side_pos(v1, a1, jmax, t3, 0.0f, 0.0f);

	if (s_acc + s_dec > dist) {
		return false; // 三段不可行
	}

	int idx = 0;
	out->segs[idx++] = (s_traj_seg_t){t1, dir * jmax};
	out->segs[idx++] = (s_traj_seg_t){0.0f, 0.0f}; // 占位
	out->segs[idx++] = (s_traj_seg_t){t3, -dir * jmax};

	out->seg_cnt = idx;
	return true;
}

static float calc_dec_side_pos(float v1, float a1, float j, float t7, float t6, float t5)
{
	float pos = 0.0f;

	float v = v1;
	float a = a1;

	/* ===== S7: +j ===== */
	if (t7 > 0.0f) {
		pos += traj_pos_jerk(v, a, +j, t7);

		v += a * t7 + 0.5f * j * t7 * t7;
		a += j * t7;
	}

	/* ===== S6: 0 ===== */
	if (t6 > 0.0f) {
		pos += traj_pos_jerk(v, a, 0.0f, t6);

		v += a * t6;
	}

	/* ===== S5: -j ===== */
	if (t5 > 0.0f) {
		pos += traj_pos_jerk(v, a, -j, t5);

		v += a * t5 - 0.5f * j * t5 * t5;
		a -= j * t5;
	}

	return pos;
}
static float calc_acc_side_pos(float v0, float a0, float j, float t1, float t2, float t3)
{
	float pos = 0.0f;

	float v = v0;
	float a = a0;

	/* ===== S1: +j ===== */
	if (t1 > 0.0f) {
		pos += traj_pos_jerk(v, a, +j, t1);

		v += a * t1 + 0.5f * j * t1 * t1;
		a += j * t1;
	}

	/* ===== S2: 0 ===== */
	if (t2 > 0.0f) {
		pos += traj_pos_jerk(v, a, 0.0f, t2);

		v += a * t2;
		/* a 不变 */
	}

	/* ===== S3: -j ===== */
	if (t3 > 0.0f) {
		pos += traj_pos_jerk(v, a, -j, t3);

		v += a * t3 - 0.5f * j * t3 * t3;
		a -= j * t3;
	}

	/* 理论上：a → 0，v → v_max */
	return pos;
}
static inline float traj_pos_jerk(float v0, float a0, float j, float t)
{
	return v0 * t + 0.5f * a0 * t * t + (1.0f / 6.0f) * j * t * t * t;
}

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
