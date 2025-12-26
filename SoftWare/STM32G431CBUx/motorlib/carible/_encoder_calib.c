#include "_encoder_calib.h"
#include <math.h>
#include <stdint.h>
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "openloop_voltage.h"
#include "motor_params.h"
#include <stdlib.h>
/* 常量定义 */
#define ENCODER_MAX_COUNT 16383.0f
#define MIN_MECH_ROUNDS   0.02f // 最小机械圈数阈值
#define MIN_ELEC_ROUNDS   0.20f // 最小电角度圈数阈值
#undef M_TWOPI
#define M_TWOPI (2.0f * M_PI)

#define OFFSET_ROTATE_SPEED         (1.0f * M_PI) // rad/s（电角度）
#define POS_ROTATE_START_ELEC_ANGLE M_PI
#define NEG_ROTATE_START_ELEC_ANGLE M_TWOPI
#define OFFSET_ZERO_ELEC_ANGLE      (3 * M_PI / 2.0F)
static void encoder_calib_free_offset_align(struct carlib_encoder *ec);

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
		ec->offset_align_num = 1;

		ec->offset_align_pos =
			malloc(sizeof(struct op_align_position) * ec->offset_align_num);

		if (!ec->offset_align_pos) {
			ec->state = ENC_CALIB_ERROR;
			ec->error_code = ENC_ERR_NO_MEM;
			break;
		}

		ec->offset_align_pos[0].angle = 0.0f;
		ec->offset_align_pos[0].duration = 2.0f;

		struct op_align_config align_cfg = {
			.positions = ec->offset_align_pos,
			.num_positions = ec->offset_align_num,
			.voltage = 0.02f,
		};

		openloop_voltage_align_start(op, &align_cfg);
		ec->state = ENC_CALIB_ALIGN;
	} break;
	case ENC_CALIB_ALIGN:
		if (!openloop_voltage_align_update(op, dt)) {
			encoder_calib_free_offset_align(ec);
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

		openloop_voltage_rotate_start(op, &cfg_rotate);
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
		ec->state = ENC_CALIB_OFFSET_ALIGN;
	} break;
	case ENC_CALIB_OFFSET_ALIGN: {

		ec->offset_align_num = 1;
		ec->offset_align_pos = malloc(sizeof(struct op_align_position));

		if (!ec->offset_align_pos) {
			ec->state = ENC_CALIB_ERROR;
			ec->error_code = ENC_ERR_NO_MEM;
			break;
		}

		ec->offset_align_pos[0].angle = POS_ROTATE_START_ELEC_ANGLE;
		ec->offset_align_pos[0].duration = 1.0f;

		struct op_align_config align_cfg = {
			.positions = ec->offset_align_pos,
			.num_positions = 1,
			.voltage = 0.02f,
		};

		openloop_voltage_align_start(op, &align_cfg);
		ec->state = ENC_CALIB_OFFSET_ALIGN_RUN;
	} break;
	case ENC_CALIB_OFFSET_ALIGN_RUN:
		if (!openloop_voltage_align_update(op, dt)) {
			ec->state = ENC_CALIB_OFFSET_INIT;
		}
		break;

	case ENC_CALIB_OFFSET_INIT: {

		ec->prev_elec_angle = POS_ROTATE_START_ELEC_ANGLE;
		ec->hit_pos_valid = 0;
		ec->hit_neg_valid = 0;

		float duration = M_PI / OFFSET_ROTATE_SPEED;

		struct op_rotate_config cfg_rotate = {
			.voltage = 0.02f,
			.speed = OFFSET_ROTATE_SPEED,
			.start_angle = POS_ROTATE_START_ELEC_ANGLE,
			.duration = duration,
		};

		openloop_voltage_rotate_start(op, &cfg_rotate);
		ec->state = ENC_CALIB_OFFSET_ROTATE_POS;
	} break;

		/* ---------- 正转采样 ---------- */
	case ENC_CALIB_OFFSET_ROTATE_POS: {

		if (openloop_voltage_rotate_update(op, dt) == 1) {

			float curr = openloop_voltage_get_total_elec_rad(op);
			float prev = ec->prev_elec_angle;

			if (prev < OFFSET_ZERO_ELEC_ANGLE && curr > OFFSET_ZERO_ELEC_ANGLE) {

				ec->raw_hit_pos = read_feedback_raw(feedback);
				ec->hit_pos_valid = 1;
			}
			ec->prev_elec_angle = curr;
			break;
		}

		/* 正转结束，开始反转 */
		ec->prev_elec_angle = NEG_ROTATE_START_ELEC_ANGLE;

		struct op_rotate_config cfg_rotate = {
			.voltage = 0.02f,
			.speed = -OFFSET_ROTATE_SPEED,
			.start_angle = NEG_ROTATE_START_ELEC_ANGLE,
			.duration = M_PI / OFFSET_ROTATE_SPEED,
		};

		openloop_voltage_rotate_start(op, &cfg_rotate);
		ec->state = ENC_CALIB_OFFSET_ROTATE_NEG;
	} break;

		/* ---------- 反转采样 ---------- */
	case ENC_CALIB_OFFSET_ROTATE_NEG: {

		if (openloop_voltage_rotate_update(op, dt) != 0) {

			float curr = openloop_voltage_get_total_elec_rad(op);
			float prev = ec->prev_elec_angle;

			if (prev > OFFSET_ZERO_ELEC_ANGLE && curr < OFFSET_ZERO_ELEC_ANGLE) {

				ec->raw_hit_neg = read_feedback_raw(feedback);
				ec->hit_neg_valid = 1;
			}

			ec->prev_elec_angle = curr;
			break;
		}

		ec->state = ENC_CALIB_OFFSET_PROCESS;
	} break;

		/* ---------- OFFSET 处理阶段 ---------- */
	case ENC_CALIB_OFFSET_PROCESS: {

		if (!ec->hit_pos_valid || !ec->hit_neg_valid) {
			ec->state = ENC_CALIB_ERROR;
			ec->error_code = ENC_ERR_CALC_FAILED;
			break;
		}

		int32_t r1 = (int32_t)ec->raw_hit_pos;
		int32_t r2 = (int32_t)ec->raw_hit_neg;
		int32_t max = 16383;

		/* 处理环绕 */
		if (abs(r1 - r2) > max / 2) {
			if (r1 < r2) {
				r1 += max;
			} else {
				r2 += max;
			}
		}
		uint32_t avg_raw = (uint32_t)((r1 + r2) / 2) % max;

		/* ---------- 按电角度周期归一化 ---------- */
		uint32_t elec_period = max / 7.0f;
		uint32_t offset = avg_raw % elec_period;
		// uint32_t offset = (uint32_t)((r1 + r2) / 2) % max;

		_update_feedback_offset_params(m_parms, offset);
		_update_feedback_cpr_params(m_parms, 16383);

		ec->state = ENC_CALIB_DONE;
	} break;

	case ENC_CALIB_DONE:
		encoder_calib_free_offset_align(ec);

		return 1;

	case ENC_CALIB_ERROR:
	default:
		encoder_calib_free_offset_align(ec);
		return -1;
	}

	return 0;
}

static void encoder_calib_free_offset_align(struct carlib_encoder *ec)
{
	if (ec->offset_align_pos) {
		free(ec->offset_align_pos);
		ec->offset_align_pos = NULL;
		ec->offset_align_num = 0;
	}
}
