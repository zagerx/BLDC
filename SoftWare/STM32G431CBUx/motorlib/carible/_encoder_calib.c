#include "_encoder_calib.h"
#include <math.h>
#include <stdint.h>
#include "device.h"
#include "feedback.h"
#include "inverter.h"

/* 常量定义 */
#define ENCODER_MAX_COUNT 16384.0f
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

	ec->state = ENC_CALIB_ALIGN;
	ec->elapsed = 0.0f;
	ec->error = 0;
	// ec->error_code = ENC_ERR_NONE;

	/* 使用更保守但有效的参数 */
	ec->align_voltage = 0.02f;      /* 增大对齐电压，确保对齐 */
	ec->align_time = 1.0f;          /* 对齐时间 */
	ec->rotate_speed = 2.0f * M_PI; /* 2电转/秒 */
	ec->rotate_time = 3.0f;         /* 旋转3秒 */

	// /* 初始化新增字段 */
	// ec->last_mech_counts = 0;
	// ec->delta_counts = 0;
	// ec->raw_delta_acc = 0;
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
	openloop_voltage_t *op = cfg->op;

	/* 更新开环控制 */
	openloop_voltage_update(op, dt);

	switch (ec->state) {
	case ENC_CALIB_ALIGN:
		inverter_set_3phase_enable(inverter);

		/* 电机轴对齐到初始位置 */
		openloop_voltage_set_mode(cfg->op, OPENLOOP_MODE_HOLD_ANGLE);
		openloop_voltage_set_voltage(cfg->op, ec->align_voltage);
		openloop_voltage_set_angle(cfg->op, 0.0f);
		openloop_voltage_enable(cfg->op);

		ec->elapsed = 0.0f;
		ec->raw_delta_acc = 0;
		ec->total_elec_rad = 0.0f;
		ec->state = ENC_CALIB_DIR_CHECK;
		break;

	case ENC_CALIB_DIR_CHECK:
		ec->elapsed += dt;
		if (ec->elapsed < ec->align_time) {
			break;
		}

		/* 记录起始编码器值 */
		ec->raw_prev = read_feedback_raw(cfg->feedback);

		/* 开始旋转 - 使用更高的电压确保转动 */
		openloop_voltage_set_mode(cfg->op, OPENLOOP_MODE_CONST_SPEED);
		openloop_voltage_set_speed(cfg->op, ec->rotate_speed);

		/* 增加旋转电压，确保电机转动 */
		openloop_voltage_set_voltage(cfg->op, ec->align_voltage);

		ec->elapsed = 0.0f;
		ec->state = ENC_CALIB_ROTATE;
		break;

	case ENC_CALIB_ROTATE: {
		/* 检查是否完成旋转时间 */
		if (ec->elapsed >= ec->rotate_time) {
			openloop_voltage_set_voltage(cfg->op, ec->align_voltage);
			ec->state = ENC_CALIB_PROCESS;
			break;
		}

		ec->elapsed += dt;

		/* 累计电角度变化 */
		float angle_step = ec->rotate_speed * dt;
		ec->total_elec_rad += angle_step;

		/* 累计机械角度变化（使用类似pp_ident的方法） */
		uint32_t current_raw = read_feedback_raw(cfg->feedback);
		ec->raw_delta_acc += unwrap_raw(current_raw, &ec->raw_prev, 16384);
	} break;

	case ENC_CALIB_PROCESS: {
		/* 计算机械圈数和电角度圈数 */
		float mech_rounds = (float)ec->raw_delta_acc / 16384.0f;
		float elec_rounds = ec->total_elec_rad / M_TWOPI;

		/* 检查是否有足够的旋转 */
		if (fabsf(mech_rounds) < MIN_MECH_ROUNDS || fabsf(elec_rounds) < MIN_ELEC_ROUNDS) {
			ec->state = ENC_CALIB_ERROR;
			ec->error = -1;
			ec->error_code = ENC_ERR_ANGLE_TOO_SMALL;
			break;
		}

		/* 计算极对数 */
		float ratio = fabsf(elec_rounds / mech_rounds);
		ec->pole_pairs = (int)(roundf(ratio));
		update_feedback_pole_pairs(feedback, ec->pole_pairs);

		/* 方向判断 */
		if ((ec->total_elec_rad > 0 && ec->raw_delta_acc > 0) ||
		    (ec->total_elec_rad < 0 && ec->raw_delta_acc < 0)) {
			ec->encoder_dir = 1; /* 编码器方向与开环驱动方向一致 */
		} else {
			ec->encoder_dir = -1; /* 编码器方向与开环驱动方向相反 */
		}
		update_feedback_direction(feedback, ec->encoder_dir);

		ec->state = ENC_CALIB_OFFSET;
	} break;

	case ENC_CALIB_OFFSET: {
		/* 回到电角 0 */
		openloop_voltage_set_mode(cfg->op, OPENLOOP_MODE_HOLD_ANGLE);
		openloop_voltage_set_angle(cfg->op, 0.0f);

		/* 等待一段时间确保回到位置 */
		ec->elapsed += dt;
		if (ec->elapsed < 0.5f) {
			break;
		}

		/* 读取当前原始计数并转换为机械角度 */
		ec->encoder_offset = read_feedback_raw(cfg->feedback);

		/* 写入最终参数 */
		update_feedback_offset(feedback, ec->encoder_offset);
		update_feedback_cpr(feedback, 16384);

		openloop_voltage_disable(cfg->op);
		ec->state = ENC_CALIB_DONE;
	} break;

	case ENC_CALIB_DONE:
		return 1;

	case ENC_CALIB_ERROR:
	default:
		openloop_voltage_disable(cfg->op);
		return -1;
	}

	return 0;
}