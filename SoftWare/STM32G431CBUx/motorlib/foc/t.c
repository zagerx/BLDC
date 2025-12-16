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
static void traj_plan_init(s_traj_plan_data_t *d, float acc_max, float v_max, float exex_cycle);
static bool traj_plan_three_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				    float dir);
static void calculate_trapezoidal_profile(float p0, float v0, float target_pos, float v_max,
					  float a_max, float *t_acc, float *t_const, float *t_dec,
					  float *peak_velocity);

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
	if (in->a_max <= 0.0f || in->v_max <= 0.0f) {
		return TRAJ_PLAN_ERR_PARAM;
	}
	test_flag = 20;

	float dp = in->target_pos - in->p0;
	if (fabsf(dp) < 1e-6f && fabsf(in->v0) < 1e-6f && fabsf(in->a0) < 1e-6f) {
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

	/* ================= 5. 清空输出 ================= */
	out->seg_cnt = 0;

	/* ================= 6. 调用对应规划器 ================= */
	traj_plan_three_segment(in, out, dir);

	if (out->seg_cnt <= 0) {
		return TRAJ_PLAN_ERR_NO_SOLUTION;
	}

	return TRAJ_PLAN_OK;
}

/* 修正后的梯形速度规划算法 - 确保总是有三段 */
static bool traj_plan_three_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
				    float dir)
{
	float dp = in->target_pos - in->p0;
	float v0 = in->v0;
	float a0 = in->a0;
	float v_max = in->v_max;
	float a_max = in->a_max;

	// 调试信息
	printf("Planning: dp=%.2f, v0=%.2f, a0=%.2f, v_max=%.2f, a_max=%.2f, dir=%.2f\n", dp, v0,
	       a0, v_max, a_max, dir);

	// 将问题转换到正方向
	float v0_dir = v0 * dir;
	float dp_dir = fabsf(dp);

	float t_acc = 0.0f, t_const = 0.0f, t_dec = 0.0f;
	float v_peak = 0.0f;

	/* ============ 情况1：当前速度方向与位移方向相同 ============ */
	if (dp * v0 >= -1e-6f) // 同向或速度为零
	{
		if (v0_dir < 0) {
			v0_dir = 0; // 确保速度非负
		}

		/* 计算加速到最大速度所需时间和距离 */
		float t_to_vmax = (v_max - v0_dir) / a_max;
		float s_to_vmax = v0_dir * t_to_vmax + 0.5f * a_max * t_to_vmax * t_to_vmax;

		/* 计算从最大速度减速到零所需时间和距离 */
		float t_from_vmax_to_zero = v_max / a_max;
		float s_from_vmax_to_zero =
			v_max * t_from_vmax_to_zero -
			0.5f * a_max * t_from_vmax_to_zero * t_from_vmax_to_zero;

		/* 总距离需求 */
		float s_total_at_vmax = s_to_vmax + s_from_vmax_to_zero;

		if (s_total_at_vmax <= dp_dir) {
			/* 标准梯形：可以达到最大速度 */
			// 加速段
			t_acc = t_to_vmax;
			// 减速段
			t_dec = t_from_vmax_to_zero;
			// 匀速段
			float s_const = dp_dir - s_total_at_vmax;
			t_const = s_const / v_max;
			v_peak = v_max;

			printf("Case 1: Trapezoidal profile: t_acc=%.2f, t_const=%.2f, t_dec=%.2f, "
			       "v_peak=%.2f\n",
			       t_acc, t_const, t_dec, v_peak);
		} else {
			/* 三角形：无法达到最大速度 */
			// 解方程求峰值速度 v_peak
			// 总距离 = (v_peak² - v0²)/(2a) + (v_peak²)/(2a) = (2v_peak² - v0²)/(2a)
			// 所以：2a * dp = 2v_peak² - v0²
			// v_peak = sqrt((2a*dp + v0²)/2)

			float discriminant = 2.0f * a_max * dp_dir + v0_dir * v0_dir;
			if (discriminant < 0) {
				printf("Error: Negative discriminant in triangular profile\n");
				return false;
			}

			v_peak = sqrtf(discriminant / 2.0f);

			// 确保峰值速度不超过最大速度
			if (v_peak > v_max) {
				v_peak = v_max;
			}

			// 加速段时间
			t_acc = (v_peak - v0_dir) / a_max;
			// 减速段时间
			t_dec = v_peak / a_max;
			// 无匀速段
			t_const = 0.0f;

			printf("Case 2: Triangular profile: t_acc=%.2f, t_dec=%.2f, v_peak=%.2f\n",
			       t_acc, t_dec, v_peak);

			// 验证距离
			float s_acc = v0_dir * t_acc + 0.5f * a_max * t_acc * t_acc;
			float s_dec = v_peak * t_dec - 0.5f * a_max * t_dec * t_dec;
			printf("Verification: s_acc=%.2f, s_dec=%.2f, total=%.2f, dp_dir=%.2f\n",
			       s_acc, s_dec, s_acc + s_dec, dp_dir);
		}
	} else {
		/* ============ 情况2：当前速度方向与位移方向相反 ============ */
		// 需要先减速到零，然后反向加速

		printf("Case 3: Velocity opposite direction, need to decelerate first\n");

		// 先减速到零的时间和距离
		float t_to_zero = fabsf(v0) / a_max;
		float s_to_zero = fabsf(v0) * t_to_zero * 0.5f;

		// 剩余距离（从零速度开始）
		float remaining_distance = dp_dir - s_to_zero;

		if (remaining_distance < 0) {
			// 只需要减速段，但需要调整
			// 计算刚好减速到目标位置的加速度
			float required_acc = (v0_dir * v0_dir) / (2.0f * dp_dir);
			if (required_acc > a_max) {
				printf("Error: Cannot stop in time with max acceleration\n");
				return false;
			}

			t_acc = 0.0f;
			t_const = 0.0f;
			t_dec = v0_dir / required_acc;
			v_peak = v0_dir;
		} else {
			// 从零速度开始的梯形规划
			// 计算加速到最大速度所需距离
			float s_to_vmax_from_zero = v_max * v_max / (2.0f * a_max);
			float s_total_from_zero = 2.0f * s_to_vmax_from_zero;

			if (remaining_distance >= s_total_from_zero) {
				// 标准梯形
				t_acc = v_max / a_max; // 从零加速到v_max
				t_dec = v_max / a_max; // 从v_max减速到零
				t_const = (remaining_distance - s_total_from_zero) / v_max;
				v_peak = v_max;
			} else {
				// 三角形
				float v_peak_tri = sqrtf(a_max * remaining_distance);
				if (v_peak_tri > v_max) {
					v_peak_tri = v_max;
				}

				t_acc = v_peak_tri / a_max;
				t_dec = v_peak_tri / a_max;
				t_const = 0.0f;
				v_peak = v_peak_tri;
			}

			// 总的第一段是减速到零
			out->seg_cnt = 0;

			// 第一段：减速到零
			out->segs[out->seg_cnt].t = t_to_zero;
			out->segs[out->seg_cnt].a = -dir * a_max * (v0 > 0 ? 1.0f : -1.0f);
			out->seg_cnt++;

			// 第二段：加速段
			out->segs[out->seg_cnt].t = t_acc;
			out->segs[out->seg_cnt].a = dir * a_max;
			out->seg_cnt++;

			// 第三段：匀速段
			if (t_const > 1e-6f) {
				out->segs[out->seg_cnt].t = t_const;
				out->segs[out->seg_cnt].a = 0.0f;
				out->seg_cnt++;
			}

			// 第四段：减速段
			out->segs[out->seg_cnt].t = t_dec;
			out->segs[out->seg_cnt].a = -dir * a_max;
			out->seg_cnt++;

			printf("Case 3: 4-segment profile generated\n");
			return true;
		}
	}

	/* ============ 构建标准三段轨迹 ============ */
	out->seg_cnt = 0;

	// 加速段
	if (t_acc > 1e-6f) {
		out->segs[out->seg_cnt].t = t_acc;
		out->segs[out->seg_cnt].a = dir * a_max;
		out->seg_cnt++;
		printf("Segment %d: ACC, t=%.2f, a=%.2f\n", out->seg_cnt - 1, t_acc, dir * a_max);
	}

	// 匀速段
	if (t_const > 1e-6f) {
		out->segs[out->seg_cnt].t = t_const;
		out->segs[out->seg_cnt].a = 0.0f;
		out->seg_cnt++;
		printf("Segment %d: CONST, t=%.2f, a=0.0\n", out->seg_cnt - 1, t_const);
	}

	// 减速段
	if (t_dec > 1e-6f) {
		out->segs[out->seg_cnt].t = t_dec;
		out->segs[out->seg_cnt].a = -dir * a_max;
		out->seg_cnt++;
		printf("Segment %d: DEC, t=%.2f, a=%.2f\n", out->seg_cnt - 1, t_dec, -dir * a_max);
	}

	// 如果只有一段，确保它是减速段
	if (out->seg_cnt == 1 && fabsf(dp) > 1e-6f) {
		// 这应该是减速段
		out->segs[0].a = -dir * a_max;
		printf("Corrected single segment to DEC\n");
	}

	printf("Total segments: %d\n", out->seg_cnt);

	return (out->seg_cnt > 0);
}

static void calculate_trapezoidal_profile(float p0, float v0, float target_pos, float v_max,
					  float a_max, float *t_acc, float *t_const, float *t_dec,
					  float *peak_velocity)
{
	// 简化的梯形规划计算，用于内部计算
	float dp = target_pos - p0;
	float dir = (dp >= 0.0f) ? 1.0f : -1.0f;

	float v0_dir = v0 * dir;
	if (v0_dir < 0) {
		v0_dir = 0;
	}

	float dp_dir = fabsf(dp);

	// 加速到最大速度
	float t_to_vmax = (v_max - v0_dir) / a_max;
	float s_to_vmax = v0_dir * t_to_vmax + 0.5f * a_max * t_to_vmax * t_to_vmax;

	// 从最大速度减速到零
	float t_from_vmax_to_zero = v_max / a_max;
	float s_from_vmax_to_zero = v_max * t_from_vmax_to_zero -
				    0.5f * a_max * t_from_vmax_to_zero * t_from_vmax_to_zero;

	float s_total_at_vmax = s_to_vmax + s_from_vmax_to_zero;

	if (s_total_at_vmax <= dp_dir) {
		// 标准梯形
		*t_acc = t_to_vmax;
		*t_dec = t_from_vmax_to_zero;
		float s_const = dp_dir - s_total_at_vmax;
		*t_const = s_const / v_max;
		*peak_velocity = v_max;
	} else {
		// 三角形
		float discriminant = 2.0f * a_max * dp_dir + v0_dir * v0_dir;
		if (discriminant < 0) {
			discriminant = 0;
		}

		*peak_velocity = sqrtf(discriminant / 2.0f);
		if (*peak_velocity > v_max) {
			*peak_velocity = v_max;
		}

		*t_acc = (*peak_velocity - v0_dir) / a_max;
		*t_dec = *peak_velocity / a_max;
		*t_const = 0.0f;
	}
}

static void traj_plan_init(s_traj_plan_data_t *d, float acc_max, float v_max, float exex_cycle)
{
	memset(d, 0, sizeof(*d));

	d->plan_in.a_max = acc_max;
	d->plan_in.v_max = v_max;
	d->plan_in.j_max = 0.0f;
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
			// 确保最终速度为0
			d->vel = 0.0f;
			d->acc = 0.0f;
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
		float pos0 = d->pos;

		/* 当前段的加速度是 s->a */
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

/* 测试函数：模拟您描述的问题 */
void test_replan_scenario(void)
{
	printf("=== 测试重归化场景 ===\n");

	struct device test_dev;
	s_tarj_data_t data;
	s_tarj_config_t config = {
		.acc = 2.0f,   // 2 m/s²
		.vmax = 10.0f, // 10 m/s
	};

	test_dev.data = &data;
	test_dev.config = &config;

	// 初始化
	s_planner_init(&test_dev, 0.0f, 0.0f, 0.0f, 0.001f);

	// 第一目标：200
	printf("\n设置第一目标：200\n");
	s_planner_update_target(&test_dev, 200.0f);

	// 模拟运行直到速度达到40，位置140
	float sim_time = 0.0f;
	while (1) {
		s_planner_action(&test_dev, 0.001f);
		sim_time += 0.001f;

		float pos = s_planner_get_pos(&test_dev);
		float vel = s_planner_get_vel(&test_dev);
		float acc = s_planner_get_acc(&test_dev);

		// 当位置接近140，速度接近40时停止
		if (pos >= 140.0f && vel >= 40.0f) {
			printf("达到条件：t=%.2fs, pos=%.2f, vel=%.2f, acc=%.2f\n", sim_time, pos,
			       vel, acc);
			break;
		}

		if (sim_time > 20.0f) {
			printf("超时！\n");
			break;
		}
	}

	// 获取当前状态
	float current_pos = s_planner_get_pos(&test_dev);
	float current_vel = s_planner_get_vel(&test_dev);
	printf("\n当前状态：pos=%.2f, vel=%.2f\n", current_pos, current_vel);

	// 检查是否在减速段
	if (current_vel > 0 && data.exec_data.cur_seg < data.exec_data.seg_cnt) {
		printf("当前段：%d/%d, seg_time=%.2f\n", data.exec_data.cur_seg + 1,
		       data.exec_data.seg_cnt, data.exec_data.seg_time);

		for (int i = 0; i < data.exec_data.seg_cnt; i++) {
			printf("Segment %d: t=%.2f, a=%.2f\n", i, data.exec_data.segs[i].t,
			       data.exec_data.segs[i].a);
		}
	}

	// 测试重归化到新目标
	printf("\n设置新目标：-300\n");
	s_planner_update_target(&test_dev, -300.0f);

	// 继续运行
	float additional_time = 0.0f;
	while (additional_time < 10.0f) {
		s_planner_action(&test_dev, 0.001f);
		sim_time += 0.001f;
		additional_time += 0.001f;

		if (fabsf(additional_time - 2.0f) < 0.001f ||
		    fabsf(additional_time - 5.0f) < 0.001f ||
		    fabsf(additional_time - 10.0f) < 0.001f) {
			printf("t=%.1fs: pos=%.2f, vel=%.2f, acc=%.2f\n", sim_time,
			       s_planner_get_pos(&test_dev), s_planner_get_vel(&test_dev),
			       s_planner_get_acc(&test_dev));
		}
	}

	printf("\n最终状态：pos=%.2f (目标: -300), vel=%.2f\n", s_planner_get_pos(&test_dev),
	       s_planner_get_vel(&test_dev));
}