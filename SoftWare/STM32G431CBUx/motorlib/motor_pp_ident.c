#include "motor_pp_ident.h"
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "math.h"
#include "motor.h"
#include "coord_transform.h"
#include "svpwm.h"
#include <stdint.h>

#undef M_PI
#define M_PI 3.1415926f

#define MAX_ELEC_ANGLE 10000.0f // 防止 float 精度丢失
#define MIN_MECH_ROT   0.02f    // 最小机械角圈数
#define MIN_ELEC_ROT   0.20f    // 最小电角度圈数

// 处理编码器跨零点跳变
static inline int32_t unwrap_raw(int32_t current, int32_t *prev, int32_t max)
{
	int32_t diff = current - *prev;
	int32_t half = max / 2;

	if (diff > half) {
		diff -= max;
	}
	if (diff < -half) {
		diff += max;
	}

	*prev = current;
	return diff;
}

/* ---------------------------------------------------------
 * 启动：初始化运行状态和累积变量
 * --------------------------------------------------------- */
void pp_ident_start(struct device *motor)
{
	struct motor_config *mc = (struct motor_config *)motor->config;
	struct feedback_config *fb_cfg = mc->feedback->config;

	struct device *ppdev = mc->pp_ident;
	struct pp_ident_data *pp = ppdev->data;
	struct pp_ident_config *cfg = ppdev->config;

	if (!cfg) {
		pp->running = false;
		pp->done = true;
		pp->pole_pairs = 0;
		return;
	}

	pp->done = false;
	pp->running = true;
	pp->time_acc = 0.0f;
	pp->elec_angle = 0.0f;

	// 初始化编码器 unwrap 累积变量
	pp->raw_start = (int32_t)fb_cfg->get_raw();
	pp->raw_prev = pp->raw_start;
	pp->raw_delta_acc = 0;
}

/* ---------------------------------------------------------
 * 主更新周期
 * --------------------------------------------------------- */
void pp_ident_update(struct device *motor, float dt)
{
	struct motor_config *mc = (struct motor_config *)motor->config;
	struct feedback_config *fb_cfg = mc->feedback->config;
	struct device *inv = mc->inverter;
	struct device *ppdev = mc->pp_ident;

	struct pp_ident_data *pp = ppdev->data;
	struct pp_ident_config *cfg = ppdev->config;

	if (!pp->running || !cfg) {
		return;
	}

	/* -----------------------------------------------------
	 * Step 1: 累加电角度
	 * ----------------------------------------------------- */
	pp->time_acc += dt;
	pp->elec_angle += cfg->openloop_speed * dt;
	if (pp->elec_angle > MAX_ELEC_ANGLE) {
		pp->elec_angle -= MAX_ELEC_ANGLE;
	}

	float angle = pp->elec_angle;

	/* -----------------------------------------------------
	 * Step 2：生成 αβ 电压
	 * ----------------------------------------------------- */
	float v_mag = cfg->openloop_voltage;
	if (v_mag > 0.577f) {
		v_mag = 0.577f;
	}

	float sinv, cosv;
	sin_cos_f32(angle, &sinv, &cosv);
	float ualpha = v_mag * cosv;
	float ubeta = v_mag * sinv;

	float abc[3];
	svm_set(ualpha, ubeta, abc);
	inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

	/* -----------------------------------------------------
	 * Step 3：实时 unwrap 累积机械角度
	 * ----------------------------------------------------- */
	int32_t raw = (int32_t)fb_cfg->get_raw();
	int32_t delta = unwrap_raw(raw, &pp->raw_prev, (int32_t)cfg->encoder_max);
	pp->raw_delta_acc += delta;

	/* -----------------------------------------------------
	 * Step 4：检查时间是否结束
	 * ----------------------------------------------------- */
	if (pp->time_acc < cfg->duration) {
		return;
	}

	/* -----------------------------------------------------
	 * Step 5：机械角圈数
	 * ----------------------------------------------------- */
	pp->raw_end = raw; // 保留最终值
	pp->mech_rounds = (float)pp->raw_delta_acc / (float)cfg->encoder_max;

	if (fabsf(pp->mech_rounds) < MIN_MECH_ROT) {
		pp->pole_pairs = 0;
		pp->done = true;
		pp->running = false;
		return;
	}

	/* -----------------------------------------------------
	 * Step 6：电角度圈数
	 * ----------------------------------------------------- */
	pp->electrical_rounds = pp->elec_angle / (2.0f * M_PI);
	if (fabsf(pp->electrical_rounds) < MIN_ELEC_ROT) {
		pp->pole_pairs = 0;
		pp->done = true;
		pp->running = false;
		return;
	}

	/* -----------------------------------------------------
	 * Step 7：极对数计算
	 * ----------------------------------------------------- */
	float pp_calc = fabsf(pp->electrical_rounds / pp->mech_rounds);
	pp->pole_pairs = (uint16_t)(roundf(pp_calc));

	/* -----------------------------------------------------
	 * Step 8：结束
	 * ----------------------------------------------------- */
	pp->done = true;
	pp->running = false;
}
