#include <stdint.h>
#include <math.h>

#include "_pp_ident.h"
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "coord_transform.h"
#include "svpwm.h"

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
void pp_ident_start(struct device *pp)
{
	struct pp_ident_data *pp_data = pp->data;
	struct pp_ident_config *cfg = pp->config;
	struct device *feeedback = cfg->feedback;
	struct feedback_config *fb_cfg = feeedback->config;
	if (!cfg) {
		pp_data->done = true;
		pp_data->pole_pairs = 0;
		return;
	}

	pp_data->done = false;
	pp_data->time_acc = 0.0f;
	pp_data->elec_angle = 0.0f;

	// 初始化编码器 unwrap 累积变量
	pp_data->raw_start = (int32_t)fb_cfg->get_raw();
	pp_data->raw_prev = pp_data->raw_start;
	pp_data->raw_delta_acc = 0;
}

/* ---------------------------------------------------------
 * 主更新周期
 * --------------------------------------------------------- */
void pp_ident_update(struct device *pp, float dt)
{
	struct pp_ident_config *cfg = pp->config; 
	struct device *fb = cfg->feedback;
	struct device *inv = cfg->inverter;
	struct feedback_config *fb_cfg = fb->config;
	struct pp_ident_data *pp_data = pp->data;

	if (!cfg) {
		return;
	}

	switch (pp_data->calibra_state) {
	case PP_CALIB_STATE_ALIGN: {
		static uint16_t count = 0;
		if (count++ < 20000) {
			float abc[3];
			svm_set(cfg->openloop_voltage * 1.0f, cfg->openloop_voltage * 0.0f, abc);
			inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);
		} else {
			count = 0;
			pp_data->raw_start = (int32_t)fb_cfg->get_raw();
			pp_data->raw_prev = pp_data->raw_start;
			pp_data->calibra_state = PP_CALIB_STATE_CCW_CALIBRATION;
		}
	} break;
	case PP_CALIB_STATE_CCW_CALIBRATION: {
		if (pp_data->time_acc > cfg->duration) {
			pp_data->calibra_state = PP_CALIB_STATE_DATA_PROCESSING;
			break;
		} else {
			/* -----------------------------------------------------
			 * Step 1: 累加电角度
			 * ----------------------------------------------------- */
			pp_data->time_acc += dt;
			pp_data->elec_angle += cfg->openloop_speed * dt;
			if (pp_data->elec_angle > MAX_ELEC_ANGLE) {
				pp_data->elec_angle -= MAX_ELEC_ANGLE;
			}

			float angle = pp_data->elec_angle * (180.0f / M_PI);
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
			int32_t delta =
				unwrap_raw(raw, &pp_data->raw_prev, (int32_t)cfg->encoder_max);
			pp_data->raw_delta_acc += delta;
		}
	} break;

	case PP_CALIB_STATE_DATA_PROCESSING: {
		/* -----------------------------------------------------
		 * Step 5：机械角圈数
		 * ----------------------------------------------------- */
		int32_t raw = (int32_t)fb_cfg->get_raw();
		pp_data->raw_end = raw; // 保留最终值
		pp_data->mech_rounds = (float)pp_data->raw_delta_acc / (float)cfg->encoder_max;

		if (fabsf(pp_data->mech_rounds) < MIN_MECH_ROT) {
			pp_data->calibra_state = PP_CALIB_STATE_ERROR;
			break;
			;
		}

		/* -----------------------------------------------------
		 * Step 6：电角度圈数
		 * ----------------------------------------------------- */
		pp_data->electrical_rounds = pp_data->elec_angle / (2.0f * M_PI);
		if (fabsf(pp_data->electrical_rounds) < MIN_ELEC_ROT) {
			pp_data->calibra_state = PP_CALIB_STATE_ERROR;
			break;
		}

		/* -----------------------------------------------------
		 * Step 7：极对数计算
		 * ----------------------------------------------------- */
		float pp_calc = fabsf(pp_data->electrical_rounds / pp_data->mech_rounds);
		pp_data->pole_pairs = (uint16_t)(roundf(pp_calc));
		feedback_set_pole_pairs(fb,pp_data->pole_pairs);
		pp_data->calibra_state = PP_CALIB_STATE_COMPLETE;
	} break;
	case PP_CALIB_STATE_COMPLETE: {
		/* -----------------------------------------------------
		 * Step 8：结束
		 * ----------------------------------------------------- */
		pp_data->done = true;
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);

	} break;
	case PP_CALIB_STATE_ERROR: {
		pp_data->pole_pairs = 0;
		pp_data->done = true;
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
	} break;
	default:
		break;
	}
}
enum pp_carible_state pp_ident_get_pp_state(struct device *pp)
{
	struct pp_ident_data *pp_data = pp->data;
	return pp_data->calibra_state;
}
