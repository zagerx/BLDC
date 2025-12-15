#include "t_trajectory.h"
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
static void traj_plan_init(s_traj_plan_data_t *d, float acc_max, float v_max, float exex_cycle);
static bool traj_plan_three_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				    float dir);
static void calculate_trapezoidal_profile(float p0, float v0, float a0, float target_pos,
					  float v_max, float a_max, float *t_acc, float *t_const,
					  float *t_dec, float *peak_velocity);

// 该函数只会在系统上电之后调用一次
int s_planner_init(struct device *dev, float start_pos, float start_vel, float start_acc,
		   float exex_cycle)
{
	s_tarj_data_t *data = dev->data;
	s_traj_plan_data_t *plan = &data->plan_data;
	s_traj_exec_data_t *exec_data = &(data->exec_data);
	traj_exec_init(exec_data, start_pos, start_vel, start_acc);

	s_tarj_config_t *conf = dev->config;
	traj_plan_init(plan, conf->acc, conf->vmax, exex_cycle);
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

	float delta_pos = fabsf(new_target_pos - plan->pre_pos);
	d->test_delat_pos = delta_pos;
	if ((delta_pos) < 1e-4f) {
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
	if (in->a_max <= 0.0f || in->v_max <= 0.0f) {
		return TRAJ_PLAN_ERR_PARAM;
	}

	float dp = in->target_pos - in->p0;
	if (fabsf(dp) < 1e-6f && fabsf(in->v0) < 1e-6f && fabsf(in->a0) < 1e-6f) {
		out->seg_cnt = 0;
		return TRAJ_PLAN_OK;
	}

	/* ================= 2. 方向统一 ================= */
	float dir = (dp >= 0.0f) ? 1.0f : -1.0f;

	/* ================= 3. 状态约束检查 ================= */
	if (fabsf(in->v0) > in->v_max * 1.1f || fabsf(in->a0) > in->a_max * 1.1f) {
		return TRAJ_PLAN_ERR_PARAM;
	}

	/* ================= 4. 清空输出 ================= */
	out->seg_cnt = 0;

	/* ================= 5. 调用对应规划器 ================= */
	if (!traj_plan_three_segment(in, out, dir)) {
		return TRAJ_PLAN_ERR_NO_SOLUTION;
	}

	if (out->seg_cnt <= 0) {
		return TRAJ_PLAN_ERR_NO_SOLUTION;
	}

	return TRAJ_PLAN_OK;
}

/* 核心的梯形速度规划算法 */
static bool traj_plan_three_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				    float dir)
{
	float t_acc = 0.0f, t_const = 0.0f, t_dec = 0.0f;
	float peak_velocity = 0.0f;

	/* 计算梯形速度规划的参数 */
	calculate_trapezoidal_profile(in->p0, in->v0, in->a0, in->target_pos, in->v_max, in->a_max,
				      &t_acc, &t_const, &t_dec, &peak_velocity);

	/* 验证是否有解 */
	if (t_acc < 0.0f || t_dec < 0.0f || (t_acc == 0.0f && t_dec == 0.0f && t_const == 0.0f)) {
		return false;
	}

	/* 构建轨迹段 */
	out->seg_cnt = 0;

	/* 1. 加速段 */
	if (t_acc > 0.0f) {
		out->segs[out->seg_cnt].t = t_acc;
		out->segs[out->seg_cnt].a = dir * in->a_max; // 正向或负向加速度
		out->seg_cnt++;
	}

	/* 2. 匀速段 */
	if (t_const > 0.0f) {
		out->segs[out->seg_cnt].t = t_const;
		out->segs[out->seg_cnt].a = 0.0f; // 零加速度
		out->seg_cnt++;
	}

	/* 3. 减速段 */
	if (t_dec > 0.0f) {
		out->segs[out->seg_cnt].t = t_dec;
		out->segs[out->seg_cnt].a = -dir * in->a_max; // 反向加速度
		out->seg_cnt++;
	}

	return true;
}

/* 梯形速度规划的核心计算函数 */
static void calculate_trapezoidal_profile(float p0, float v0, float a0, float target_pos,
					  float v_max, float a_max, float *t_acc, float *t_const,
					  float *t_dec, float *peak_velocity)
{
	float dp = target_pos - p0;
	float dir = (dp >= 0.0f) ? 1.0f : -1.0f;

	/* 将问题转化到正向运动 */
	float v0_dir = v0 * dir;
	float a0_dir = a0 * dir;
	float dp_dir = fabsf(dp);

	/* 计算最大可达速度（考虑当前状态） */
	float v_max_allowed = v_max;

	/* 情况1：当前速度已经超过最大速度，需要先减速 */
	if (v0_dir > v_max) {
		/* 先减速到最大速度 */
		float t_decel_to_vmax = (v0_dir - v_max) / a_max;
		float s_decel_to_vmax = (v0_dir + v_max) * 0.5f * t_decel_to_vmax;

		/* 如果减速段就已经走过了目标位置 */
		if (s_decel_to_vmax >= dp_dir) {
			/* 只有减速段，计算减速到零所需时间 */
			float t_stop = v0_dir / a_max;
			float s_stop = v0_dir * t_stop * 0.5f;

			if (s_stop > dp_dir) {
				/* 需要调整加速度，使刚好在目标位置减速到零 */
				float required_acc = (v0_dir * v0_dir) / (2.0f * dp_dir);
				if (required_acc > a_max) {
					/* 无法在目标位置减速到零，规划失败 */
					*t_acc = 0.0f;
					*t_const = 0.0f;
					*t_dec = 0.0f;
					*peak_velocity = v0_dir;
					return;
				}

				*t_acc = 0.0f;
				*t_const = 0.0f;
				*t_dec = v0_dir / required_acc;
				*peak_velocity = v0_dir;
				return;
			} else {
				/* 标准的减速到零 */
				*t_acc = 0.0f;
				*t_const = 0.0f;
				*t_dec = t_stop;
				*peak_velocity = v0_dir;
				return;
			}
		} else {
			dp_dir -= s_decel_to_vmax;
			v0_dir = v_max;
		}
	}

	/* 情况2：标准梯形速度规划 */
	/* 计算加速到最大速度所需时间和距离 */
	float t_acc_to_vmax = 0.0f;
	if (v0_dir < v_max) {
		t_acc_to_vmax = (v_max - v0_dir) / a_max;
	}
	float s_acc_to_vmax = (v0_dir + v_max) * 0.5f * t_acc_to_vmax;

	/* 计算从最大速度减速到零所需时间和距离 */
	float t_dec_from_vmax = v_max / a_max;
	float s_dec_from_vmax = v_max * 0.5f * t_dec_from_vmax;

	/* 计算总距离需求 */
	float s_total_needed = s_acc_to_vmax + s_dec_from_vmax;

	if (s_total_needed > dp_dir) {
		/* 无法达到最大速度，三角形速度规划 */
		/* 解二次方程：a*t_acc² + 2*v0*t_acc - dp = 0 */
		float a = a_max;
		float b = 2.0f * v0_dir;
		float c = -dp_dir;

		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0.0f) {
			/* 无实数解，规划失败 */
			*t_acc = 0.0f;
			*t_const = 0.0f;
			*t_dec = 0.0f;
			*peak_velocity = 0.0f;
			return;
		}

		float t_acc_total = (-b + sqrtf(discriminant)) / (2.0f * a);
		if (t_acc_total < 0.0f) {
			t_acc_total = (-b - sqrtf(discriminant)) / (2.0f * a);
		}

		/* 计算峰值速度 */
		float v_peak = v0_dir + a_max * t_acc_total;

		*t_acc = t_acc_total;
		*t_const = 0.0f;
		*t_dec = v_peak / a_max;
		*peak_velocity = v_peak;
	} else {
		/* 标准梯形速度规划 */
		float s_const = dp_dir - s_total_needed;
		float t_const_time = s_const / v_max;

		*t_acc = t_acc_to_vmax;
		*t_const = t_const_time;
		*t_dec = t_dec_from_vmax;
		*peak_velocity = v_max;
	}
}

static void traj_plan_init(s_traj_plan_data_t *d, float acc_max, float v_max, float exex_cycle)
{
	memset(d, 0, sizeof(*d));

	d->plan_in.a_max = acc_max;
	d->plan_in.v_max = v_max;
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

		float vel0 = d->vel;
		float pos0 = d->pos;

		float new_acc = s->a;

		/* 速度更新：v = v0 + a*t */
		float new_vel = vel0 + new_acc * dt_use;

		/* 位置更新：s = s0 + v0*t + 0.5*a*t² */
		float new_pos = pos0 + vel0 * dt_use + 0.5f * new_acc * dt_use * dt_use;

		/* 更新状态 */
		d->acc = new_acc;
		d->vel = new_vel;
		d->pos = new_pos;

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

/* 测试函数 */
void test_trapezoidal_planner(void)
{
	struct device test_dev;
	s_tarj_data_t data;
	s_tarj_config_t config = {
		.acc = 2.0f,  // 2 m/s²
		.vmax = 1.0f, // 1 m/s
	};

	test_dev.data = &data;
	test_dev.config = &config;

	/* 初始化 */
	s_planner_init(&test_dev, 0.0f, 0.0f, 0.0f, 0.001f);

	/* 设置目标位置 */
	s_planner_update_target(&test_dev, 10.0f);

	printf("=== 梯形速度规划测试 ===\n");
	printf("起始位置: 0.0, 目标位置: 10.0\n");
	printf("最大速度: %.2f m/s, 最大加速度: %.2f m/s²\n", config.vmax, config.acc);

	/* 模拟运行 */
	float total_time = 0.0f;
	for (int i = 0; i < 2000; i++) {
		s_planner_action(&test_dev, 0.001f);
		total_time += 0.001f;

		if (i % 200 == 0) {
			float pos = s_planner_get_pos(&test_dev);
			float vel = s_planner_get_vel(&test_dev);
			float acc = s_planner_get_acc(&test_dev);
			printf("t=%.3fs: pos=%.3f, vel=%.3f, acc=%.3f\n", total_time, pos, vel,
			       acc);
		}
	}

	printf("最终位置: %.3f\n", s_planner_get_pos(&test_dev));
}