#include "foc_trajectory_plan.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

/* =========================================================
 * 内部函数声明
 * ========================================================= */
static void traj_exec_init(s_traj_exec_data_t *d, float start_pos, float start_vel,
						   float start_acc);

static void traj_exec_step(s_traj_exec_data_t *d, float dt);

static void traj_exec_load(s_traj_exec_data_t *d, const s_traj_seg_t *segs, int seg_cnt);

static s_traj_plan_status_t traj_plan(s_traj_plan_data_t *d);

static void traj_plan_init(s_traj_plan_data_t *d, float start, float acc_max, float v_max, float exec_cycle);

static bool traj_plan_three_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
									float dir);

/* =========================================================
 * Planner 初始化（系统上电仅调用一次）
 * ========================================================= */
int s_planner_init(struct device *dev, float start_pos, float start_vel, float start_acc,
				   float exec_cycle)
{
	s_tarj_data_t *data = dev->data;
	s_traj_plan_data_t *plan = &data->plan_data;
	s_traj_exec_data_t *exec = &data->exec_data;

	traj_exec_init(exec, start_pos, start_vel, start_acc);

	s_tarj_config_t *conf = dev->config;
	traj_plan_init(plan, start_pos, conf->acc, conf->vmax, exec_cycle);

	return 0;
}

/* =========================================================
 * Planner 执行（周期调用）
 * ========================================================= */
void s_planner_action(struct device *dev, float dt)
{
	s_traj_exec_data_t *exec = &((s_tarj_data_t *)dev->data)->exec_data;

	if (exec->state == TRAJ_EXEC_END)
	{
		exec->state = TRAJ_EXEC_IDLE;
		return;
	}

	if (exec->state != TRAJ_EXEC_RUNNING)
	{
		return;
	}

	traj_exec_step(exec, dt);
}

/* =========================================================
 * 更新目标位置（在线 replanning）
 * ========================================================= */
s_traj_plan_status_t s_planner_update_target(struct device *dev, float new_target_pos)
{
	s_tarj_data_t *d = dev->data;
	s_traj_plan_data_t *plan = &d->plan_data;
	s_traj_exec_data_t *exec = &d->exec_data;

	if (fabsf(new_target_pos - plan->pre_pos) < 1e-4f)
	{
		return TRAJ_PLAN_NOT_NEEDED;
	}
	plan->pre_pos = new_target_pos;

	plan->plan_in.p0 = exec->pos;
	plan->plan_in.v0 = exec->vel;
	plan->plan_in.a0 = exec->acc;
	plan->plan_in.target_pos = new_target_pos;

	s_traj_plan_status_t ret = traj_plan(plan);
	if (ret != TRAJ_PLAN_OK)
	{
		/* 保持当前轨迹继续跑 */
		return ret;
	}

	traj_exec_load(exec, plan->plan_out.segs, plan->plan_out.seg_cnt);
	return TRAJ_PLAN_OK;
}

/* =========================================================
 * 核心规划入口
 * ========================================================= */
static s_traj_plan_status_t traj_plan(s_traj_plan_data_t *d)
{
	s_traj_plan_input_t *in = &d->plan_in;
	s_traj_plan_output_t *out = &d->plan_out;

	if (!d || in->a_max <= 0.0f || in->v_max <= 0.0f)
	{
		return TRAJ_PLAN_ERR_PARAM;
	}

	float dp = in->target_pos - in->p0;
	if (fabsf(dp) < 1e-6f && fabsf(in->v0) < 1e-6f && fabsf(in->a0) < 1e-6f)
	{
		out->seg_cnt = 0;
		return TRAJ_PLAN_OK;
	}

	float dir = (dp >= 0.0f) ? 1.0f : -1.0f;

	if (fabsf(in->v0) > in->v_max * 1.1f || fabsf(in->a0) > in->a_max * 1.1f)
	{
		return TRAJ_PLAN_ERR_PARAM;
	}

	out->seg_cnt = 0;

	if (!traj_plan_three_segment(in, out, dir))
	{
		return TRAJ_PLAN_ERR_NO_SOLUTION;
	}

	return (out->seg_cnt > 0) ? TRAJ_PLAN_OK : TRAJ_PLAN_ERR_NO_SOLUTION;
}

/* =========================================================
 * 三段式（梯形）速度规划 + 位移一致性修复
 * ========================================================= */
static bool traj_plan_three_segment(const s_traj_plan_input_t *in, s_traj_plan_output_t *out,
									float dir)
{
	out->seg_cnt = 0;

	float p0 = in->p0;
	float v0 = in->v0 * dir;
	float dp = (in->target_pos - p0) * dir;

	float a = in->a_max;
	float v_max = in->v_max;

	/* -----------------------------
	 * 1. 如果目标在反方向，先刹停
	 * ----------------------------- */
	if (v0 < 0.0f)
	{
		float t = -v0 / a;
		out->segs[out->seg_cnt++] = (s_traj_seg_t){.t = t, .a = dir * a};
		v0 = 0.0f;
	}

	/* -----------------------------
	 * 2. 计算刹停距离
	 * ----------------------------- */
	float d_stop = v0 * v0 / (2.0f * a);

	/* -----------------------------
	 * 3. 不够距离，直接减速到 0
	 * ----------------------------- */
	if (dp <= d_stop)
	{
		float t = v0 / a;
		out->segs[out->seg_cnt++] = (s_traj_seg_t){.t = t, .a = -dir * a};
		return true;
	}

	/* -----------------------------
	 * 4. 是否能跑到 vmax
	 * ----------------------------- */
	float d_acc = (v_max * v_max - v0 * v0) / (2.0f * a);
	float d_dec = v_max * v_max / (2.0f * a);

	if (dp >= d_acc + d_dec)
	{
		/* 加速到 vmax */
		if (v0 < v_max)
		{
			float t = (v_max - v0) / a;
			out->segs[out->seg_cnt++] = (s_traj_seg_t){.t = t, .a = dir * a};
		}

		/* 匀速 */
		float d_const = dp - d_acc - d_dec;
		float t_const = d_const / v_max;
		if (t_const > 0.0f)
		{
			out->segs[out->seg_cnt++] = (s_traj_seg_t){.t = t_const, .a = 0.0f};
		}

		/* 减速到 0 */
		float t_dec = v_max / a;
		out->segs[out->seg_cnt++] = (s_traj_seg_t){.t = t_dec, .a = -dir * a};
	}
	else
	{
		/* 达不到 vmax，三角形速度 */
		float v_peak = sqrtf(a * dp + 0.5f * v0 * v0);

		float t1 = (v_peak - v0) / a;
		float t2 = v_peak / a;

		if (t1 > 0.0f)
		{
			out->segs[out->seg_cnt++] = (s_traj_seg_t){.t = t1, .a = dir * a};
		}

		out->segs[out->seg_cnt++] = (s_traj_seg_t){.t = t2, .a = -dir * a};
	}

	return (out->seg_cnt > 0);
}

static void traj_plan_init(s_traj_plan_data_t *d, float start, float acc_max, float v_max, float exec_cycle)
{
	memset(d, 0, sizeof(*d));
	d->plan_in.a_max = acc_max;
	d->plan_in.v_max = v_max;
	d->plan_in.t_min = exec_cycle * 4.0f;
	d->pre_pos = start;
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

static void traj_exec_step(s_traj_exec_data_t *d, float dt)
{
	float remain = dt;

	while (remain > 0.0f && d->state == TRAJ_EXEC_RUNNING)
	{
		if (d->cur_seg >= d->seg_cnt)
		{
			d->state = TRAJ_EXEC_END;
			break;
		}

		const s_traj_seg_t *s = &d->segs[d->cur_seg];
		float t_left = s->t - d->seg_time;
		float use = (remain < t_left) ? remain : t_left;

		d->pos += d->vel * use + 0.5f * s->a * use * use;
		d->vel += s->a * use;
		d->acc = s->a;

		d->seg_time += use;
		remain -= use;

		if (d->seg_time >= s->t)
		{
			d->seg_time = 0.0f;
			d->cur_seg++;
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