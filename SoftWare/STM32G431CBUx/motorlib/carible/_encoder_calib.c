#include "_encoder_calib.h"
#include <math.h>
#include <stdint.h>
#include "device.h"
#include "feedback.h"
/* 工具函数：把角度限制到 [0, 2π) */
static float wrap_0_2pi(float angle)
{
	while (angle < 0.0f) {
		angle += 2.0f * M_PI;
	}
	while (angle >= 2.0f * M_PI) {
		angle -= 2.0f * M_PI;
	}
	return angle;
}

void carlib_encoder_init(struct carlib_encoder *ec)
{
	if (!ec || !ec->cfg) {
		return;
	}

	ec->state = ENC_CALIB_ALIGN;
	ec->elapsed = 0.0f;
	ec->error = 0;

	/* 推荐初值 */
	ec->align_voltage = 0.02f;
	ec->align_time = 0.5f;
	ec->rotate_speed = 10.0f; /* rad/s 电角 */
	ec->rotate_time = 2.0f;
}

int32_t encoder_calib_update(struct carlib_encoder *ec, float dt)
{
	if (!ec || !ec->cfg || !ec->cfg->op || !ec->cfg->params) {
		return -1;
	}

	struct carlib_config *cfg = ec->cfg;
	struct device *feedback = cfg->feedback;
	switch (ec->state) {
	case ENC_CALIB_ALIGN:
		/* 电机轴对齐到初始位置 */
		openloop_voltage_set_mode(cfg->op, OPENLOOP_MODE_HOLD_ANGLE);
		openloop_voltage_set_voltage(cfg->op, ec->align_voltage);
		openloop_voltage_set_angle(cfg->op, 0.0f);
		openloop_voltage_enable(cfg->op);

		ec->elapsed = 0.0f;
		ec->state = ENC_CALIB_DIR_CHECK;
		break;

	case ENC_CALIB_DIR_CHECK:
		ec->elapsed += dt;
		if (ec->elapsed < ec->align_time) {
			break;
		}

		/* 记录旋转起始原始计数 */
		ec->mech_angle_start = (float)read_feedback_raw(cfg->feedback);

		/* 小角度正向旋转 */
		openloop_voltage_set_mode(cfg->op, OPENLOOP_MODE_CONST_SPEED);
		openloop_voltage_set_speed(cfg->op, ec->rotate_speed);
		ec->elapsed = 0.0f;
		ec->state = ENC_CALIB_ROTATE;
		break;

	case ENC_CALIB_ROTATE:
		ec->elapsed += dt;
		if (ec->elapsed < ec->rotate_time) {
			break;
		}

		/* 记录旋转结束原始计数 */
		ec->mech_angle_end = (float)read_feedback_raw(cfg->feedback);

		/* 编码器方向判断 */
		ec->encoder_dir = (ec->mech_angle_end > ec->mech_angle_start) ? 1 : -1;

		ec->state = ENC_CALIB_PROCESS;
		break;

	case ENC_CALIB_PROCESS: {
		/* 计算极对数 */
		float mech_delta_counts = fabsf(ec->mech_angle_end - ec->mech_angle_start);
		if (mech_delta_counts < 1e-3f) {
			ec->state = ENC_CALIB_ERROR;
			ec->error = -1;
			break;
		}

		/* 将计数转换为机械角度（rad） */
		float mech_delta_rad = 2.0f * M_PI * mech_delta_counts / 16384;
		float elec_delta = ec->rotate_speed * ec->rotate_time;

		ec->pole_pairs = (int)(elec_delta / mech_delta_rad + 0.5f);

		ec->state = ENC_CALIB_OFFSET;
	} break;

	case ENC_CALIB_OFFSET: {
		/* 回到电角 0 */
		openloop_voltage_set_mode(cfg->op, OPENLOOP_MODE_HOLD_ANGLE);
		openloop_voltage_set_angle(cfg->op, 0.0f);

		/* 读取当前原始计数并转换为机械角度 */
		float mech = 2.0f * M_PI * read_feedback_raw(cfg->feedback) / 16384;
		float offset = -ec->pole_pairs * mech * ec->encoder_dir;
		ec->encoder_offset = wrap_0_2pi(offset);

		/* 写入最终参数 */
		update_feedback_offset(feedback, ec->encoder_offset);
		update_feedback_pole_pairs(feedback, ec->pole_pairs);
		update_feedback_direction(feedback, ec->encoder_dir);
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
