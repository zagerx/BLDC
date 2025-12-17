#include "_pp_ident.h"
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "svpwm.h"
#include "coord_transform.h"
#include <stddef.h>
#include <stdint.h>
#include "foc.h"
#undef M_PI
#define M_PI 3.14159265358979323846f

#undef M_TWOPI
#define M_TWOPI (2.0f * M_PI)

#define RAD_TO_DEG (180.0f / M_PI)

#define MIN_MECH_ROUNDS 0.02f // 最小机械圈数阈值
#define MIN_ELEC_ROUNDS 0.20f // 最小电角度圈数阈值

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

/* ---------------------------------------------------------
 * 启动/复位
 * --------------------------------------------------------- */
void pp_ident_start(struct device *pp)
{
	if (!pp || !pp->data || !pp->config) {
		return;
	}

	struct pp_ident_data *pd = pp->data;

	pd->state = PP_CALIB_STATE_ALIGN;

	pd->time_acc = 0.0f;
	pd->drive_angle = 0.0f;
	pd->total_elec_rad = 0.0f;
	pd->raw_delta_acc = 0;
}

/* ---------------------------------------------------------
 * 主更新循环
 * 返回值: 0=进行中, 1=完成, -1=错误
 * --------------------------------------------------------- */
int32_t pp_ident_update(struct device *pp, float dt)
{
	// 防御性检查
	if (!pp || !pp->data || !pp->config) {
		return -1;
	}

	int32_t ret = 0;
	struct pp_ident_config *cfg = pp->config;
	struct pp_ident_data *pd = pp->data;
	struct device *inv = cfg->inverter;
	struct device *fb = cfg->feedback;

	switch (pd->state) {

	/* -----------------------------------------------------
	 * Step 1: ALIGN (预定位)
	 * ----------------------------------------------------- */
	case PP_CALIB_STATE_ALIGN: {
		_open_loop_voltage_vector_drive(inv, 0.0, cfg->openloop_voltage);

		pd->time_acc += dt;

		if (pd->time_acc >= cfg->align_duration) {
			pd->time_acc = 0.0f;
			// 记录起始位置
			pd->raw_prev = read_feedback_raw(fb);
			pd->raw_delta_acc = 0;
			pd->state = PP_CALIB_STATE_ROTATING;
		}
	} break;

	/* -----------------------------------------------------
	 * Step 2: ROTATING (开环旋转)
	 * ----------------------------------------------------- */
	case PP_CALIB_STATE_ROTATING: {
		// 检查是否完成旋转时间
		if (pd->time_acc >= cfg->rotate_duration) {
			pd->state = PP_CALIB_STATE_DATA_PROCESSING;
			inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
			break;
		}

		pd->time_acc += dt;

		// 更新电角度
		float angle_step = cfg->openloop_speed * dt;
		pd->total_elec_rad += angle_step;

		pd->drive_angle += angle_step;
		if (pd->drive_angle > M_TWOPI) {
			pd->drive_angle -= M_TWOPI;
		} else if (pd->drive_angle < 0.0f) {
			pd->drive_angle += M_TWOPI;
		}
		float angle = pd->drive_angle * RAD_TO_DEG;

		// SVM 输出
		float v_mag = cfg->openloop_voltage;
		if (v_mag > 0.577f) {
			v_mag = 0.577f;
		}

		_open_loop_voltage_vector_drive(inv, angle, v_mag);

		// 累计机械角度
		uint32_t current_raw = read_feedback_raw(fb);
		pd->raw_delta_acc +=
			unwrap_raw(current_raw, &pd->raw_prev, (uint32_t)cfg->encoder_max);

	} break;

	/* -----------------------------------------------------
	 * Step 3: PROCESSING (计算)
	 * ----------------------------------------------------- */
	case PP_CALIB_STATE_DATA_PROCESSING: {
		float mech_rounds = (float)pd->raw_delta_acc / (float)cfg->encoder_max;
		float elec_rounds = pd->total_elec_rad / M_TWOPI;

		if (fabsf(mech_rounds) < MIN_MECH_ROUNDS || fabsf(elec_rounds) < MIN_ELEC_ROUNDS) {
			pd->state = PP_CALIB_STATE_ERROR;
			break;
		}

		float ratio = fabsf(elec_rounds / mech_rounds);
		uint16_t pole_pairs = (uint16_t)roundf(ratio);
		// 应用结果
		update_feedback_pole_pairs(fb, pole_pairs);
		// **方向判断**
		// 如果电角度和机械角度的累计方向一致，则方向正确（1），否则方向错误（-1）。
		// 注意：total_elec_rad 的符号取决于 openloop_speed
		int16_t mech_direction;
		if ((pd->total_elec_rad > 0 && pd->raw_delta_acc > 0) ||
		    (pd->total_elec_rad < 0 && pd->raw_delta_acc < 0)) {
			mech_direction = 1; // 编码器方向与开环驱动方向一致
		} else {
			mech_direction = -1; // 编码器方向与开环驱动方向相反，需要反转
		}
		update_feedback_direction(fb, mech_direction);
		update_feedback_cpr(fb, cfg->encoder_max);
		pd->state = PP_CALIB_STATE_COMPLETE;
	} break;

	/* -----------------------------------------------------
	 * Step 4: COMPLETE (完成)
	 * ----------------------------------------------------- */
	case PP_CALIB_STATE_COMPLETE:
		ret = 1;
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
		break;

	/* -----------------------------------------------------
	 * Step 5: ERROR (错误)
	 * ----------------------------------------------------- */
	case PP_CALIB_STATE_ERROR:
		ret = -1;
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
		break;

	default:
		pd->state = PP_CALIB_STATE_IDLE;
		break;
	}

	return ret;
}
