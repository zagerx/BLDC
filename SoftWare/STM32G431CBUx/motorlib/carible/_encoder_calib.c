#include "_encoder_calib.h"
#include <math.h>
#include <stdint.h>
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "openloop_voltage.h"
#include "motor_params.h"

/* 常量定义 */
#define ENCODER_MAX_COUNT 16383.0f
#define MIN_MECH_ROUNDS   0.02f // 最小机械圈数阈值
#define MIN_ELEC_ROUNDS   0.20f // 最小电角度圈数阈值
#undef M_TWOPI
#define M_TWOPI (2.0f * M_PI)

/* ---------------------------------------------------------
 * 内部辅助：处理编码器过零点 (Unwrap)
 * --------------------------------------------------------- */
static inline int32_t unwrap_raw(uint32_t current, uint32_t *prev, uint32_t max_count)
{
	int32_t diff = current - *prev;
	int32_t half = max_count / 2;

	if (diff > half) {
		diff -= max_count;
	} else if (diff < -half) {
		diff += max_count;
	}

	*prev = current;
	return diff;
}

void carlib_encoder_init(struct carlib_encoder *ec, struct carlib_config *cfg)
{
	if (!ec) {
		return;
	}
	ec->cfg = cfg;

	ec->state = ENC_CALIB_INIT;
	ec->elapsed = 0.0f;
	ec->error = 0;
	ec->error_code = ENC_ERR_NONE;

	ec->total_elec_rad = 0.0f;
	ec->raw_prev = 0;
}

int32_t encoder_calib_update(struct carlib_encoder *ec, float dt)
{
	if (!ec || !ec->cfg || !ec->cfg->op || !ec->cfg->params || !ec->cfg->inverter) {
		ec->error = -1;
		ec->error_code = ENC_ERR_PARAM;
		return -1;
	}

	struct carlib_config *cfg = ec->cfg;
	struct device *feedback = cfg->feedback;
	struct device *inverter = cfg->inverter;
	struct motor_parameters *m_parms = cfg->params;
	struct openloop_voltage *op = cfg->op;

	switch (ec->state) {
	case ENC_CALIB_INIT: {
		inverter_set_3phase_enable(inverter);
		struct op_align_config align_cfg = {
			.align_angle = 0.0f,
			.align_tim = 1.0f,
			.voltage = 0.02f,
		};
		openloop_voltage_align_start(op, &align_cfg);
		ec->state = ENC_CALIB_ALIGN;
	} break;
	case ENC_CALIB_ALIGN:
		if (!openloop_voltage_align_update(op, dt)) {
			ec->state = ENC_CALIB_DIR_CHECK;
			break;
		}
		break;

	case ENC_CALIB_DIR_CHECK:
		ec->raw_prev = read_feedback_raw(feedback);

		struct op_rotate_config cfg_rotate = {
			.voltage = 0.02f,
			.speed = 2.0f * M_PI,
			.start_angle = 0.0f,
			.duration = 3.0f,
		};

		openloop_voltage_roate_start(op, &cfg_rotate);
		ec->state = ENC_CALIB_ROTATE;
		break;

	case ENC_CALIB_ROTATE: {
		if (openloop_voltage_rotate_update(op, dt) != 0) {
			uint32_t current_raw = read_feedback_raw(feedback);
			ec->raw_delta_acc += unwrap_raw(current_raw, &ec->raw_prev, 16383);
			break;
		}

		uint32_t current_raw = read_feedback_raw(feedback);
		ec->raw_delta_acc += unwrap_raw(current_raw, &ec->raw_prev, 16383);

		ec->total_elec_rad = openloop_voltage_get_total_elec_rad(op);

		ec->state = ENC_CALIB_PROCESS;
	} break;

	case ENC_CALIB_PROCESS: {
		float mech_rounds = (float)ec->raw_delta_acc / 16383.0f;
		float elec_rounds = ec->total_elec_rad / M_TWOPI;

		if (fabsf(mech_rounds) < MIN_MECH_ROUNDS || fabsf(elec_rounds) < MIN_ELEC_ROUNDS) {
			ec->state = ENC_CALIB_ERROR;
			ec->error_code = ENC_ERR_ANGLE_TOO_SMALL;
			break;
		}

		float ratio = fabsf(elec_rounds / mech_rounds);
		int pole_pairs = (int)roundf(ratio);
		_update_feedback_pairs_params(m_parms, pole_pairs);
		int encoder_dir;
		if ((ec->total_elec_rad > 0 && ec->raw_delta_acc > 0) ||
		    (ec->total_elec_rad < 0 && ec->raw_delta_acc < 0)) {
			encoder_dir = 1;
		} else {
			encoder_dir = -1;
		}
		_update_feedback_dir_params(m_parms, encoder_dir);
		ec->state = ENC_CALIB_OFFSET_INIT;
	} break;

	case ENC_CALIB_OFFSET_INIT: {
		struct op_align_config align_cfg = {
			.align_angle = -(M_PI / 2.0f),
			.align_tim = 2.0f,
			.voltage = 0.02f,
		};
		openloop_voltage_align_start(op, &align_cfg);
		ec->state = ENC_CALIB_OFFSET_RUNING;
	} break;
	case ENC_CALIB_OFFSET_RUNING: {
		if (!openloop_voltage_align_update(op, dt)) {
			uint32_t encoder_offset = read_feedback_raw(feedback);
			_update_feedback_offset_params(m_parms, encoder_offset);
			_update_feedback_cpr_params(m_parms, 16383);
			ec->state = ENC_CALIB_DONE;
			break;
		}
		break;
	} break;
	case ENC_CALIB_DONE:
		return 1;

	case ENC_CALIB_ERROR:
	default:
		// openloop_voltage_disable(cfg->op);
		return -1;
	}

	return 0;
}
