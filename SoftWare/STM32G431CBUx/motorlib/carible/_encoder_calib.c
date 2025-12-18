#include "_encoder_calib.h"
#include <stdint.h>
#include <math.h>
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "svpwm.h"
#include <stdlib.h> // for abs
#include "coord_transform.h"
#include "foc.h"
#undef M_PI
#define M_PI 3.14159265358979323846f

#undef M_TWOPI
#define M_TWOPI    (2.0f * M_PI)
#define RAD_TO_DEG (180.0f / M_PI)

#define ALIGN_DURATION 2.0f // 对齐时间S

// 扫描范围：从 -PI 转到 +PI，确保经过 0 点
#define SCAN_RANGE_START (-M_PI)
#define SCAN_RANGE_END   (M_PI)

/* ---------------------------------------------------------
 * 启动
 * --------------------------------------------------------- */
void encoder_calib_start(struct device *encoder_calib)
{
	if (!encoder_calib || !encoder_calib->data || !encoder_calib->config) {
		return;
	}

	struct encoder_calib_data *ed = encoder_calib->data;

	ed->state = ENC_CALIB_STATE_ALIGN_START;
	ed->driver_elec_angle = SCAN_RANGE_START; // 起点设为 -PI
	ed->fwd_captured = false;
	ed->bwd_captured = false;
	ed->time_acc = 0.0f;
}

/* ---------------------------------------------------------
 * 主更新
 * --------------------------------------------------------- */
int32_t encoder_calib_update(struct device *encoder_calib, float dt)
{
	if (!encoder_calib || !encoder_calib->data || !encoder_calib->config) {
		return -1;
	}

	int32_t ret = 0;
	struct device *enc_dev = encoder_calib;
	struct encoder_calib_data *ed = enc_dev->data;
	struct encoder_calib_config *cfg = enc_dev->config;
	struct device *inv = cfg->inverter;
	struct device *fb = cfg->feedback;

	switch (ed->state) {

	/* -----------------------------------------------------
	 * Step 1: 定位到扫描起点 (-PI)
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_ALIGN_START: {
		// 施加起始角度电压
		float angle_deg = ed->driver_elec_angle * RAD_TO_DEG;
		_open_loop_voltage_vector_drive(inv, angle_deg, cfg->voltage);

		ed->time_acc += dt;
		if (ed->time_acc > ALIGN_DURATION) {
			ed->time_acc = 0.0f;
			ed->counter = 0;
			ed->avg_sum = 0;
			ed->state = ENC_CALIB_STATE_SCAN_FWD;
		}
	} break;

	/* -----------------------------------------------------
	 * Step 2: 正向扫描 (-PI -> +PI)
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_SCAN_FWD: {
		float prev_angle = ed->driver_elec_angle;

		// 1. 更新角度
		ed->driver_elec_angle += cfg->speed * dt; // 正向旋转

		// 2. SVM 输出
		float angle_deg = ed->driver_elec_angle * RAD_TO_DEG;
		_open_loop_voltage_vector_drive(inv, angle_deg, cfg->voltage);

		uint32_t raw_curr = read_feedback_raw(fb);
		// 4. 找到过零点 (Zero Crossing)
		if (prev_angle < 0.0f && ed->driver_elec_angle >= 0.0f) {
			ed->raw_fwd = (uint32_t)raw_curr;
			ed->fwd_captured = true;
		}

		// 5. 结束判断
		if (ed->driver_elec_angle >= SCAN_RANGE_END) {
			ed->driver_elec_angle = SCAN_RANGE_END; // 钳位
			ed->state = ENC_CALIB_STATE_SCAN_BWD;
		}
	} break;

	/* -----------------------------------------------------
	 * Step 3: 反向扫描 (+PI -> -PI)
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_SCAN_BWD: {
		float prev_angle = ed->driver_elec_angle;

		// 1. 更新角度 (反向)
		ed->driver_elec_angle -= cfg->speed * dt;

		// // 2. SVM 输出
		float angle_deg = ed->driver_elec_angle * RAD_TO_DEG;
		_open_loop_voltage_vector_drive(inv, angle_deg, cfg->voltage);

		// 3. 更新 raw_prev (用于 unwrap 连续性)
		uint32_t raw_curr = read_feedback_raw(fb);

		// 4. 找到过零点 (Zero Crossing)
		if (prev_angle > 0.0f && ed->driver_elec_angle <= 0.0f) {
			ed->raw_bwd = (uint32_t)raw_curr;
			ed->bwd_captured = true;
		}
		// 5. 结束判断
		if (ed->driver_elec_angle <= SCAN_RANGE_START) {
			inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f); // 停机
			ed->state = ENC_CALIB_STATE_CALCULATE;
		}
	} break;

	/* -----------------------------------------------------
	 * Step 4: 计算与平均
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_CALCULATE: {
		// 1. 计算平均零偏 (处理环绕问题)
		if (ed->fwd_captured && ed->bwd_captured) {
			int32_t r1 = (int32_t)ed->raw_fwd;
			int32_t r2 = (int32_t)ed->raw_bwd;
			int32_t max = (int32_t)cfg->encoder_max;

			// 关键：处理边界环绕 (例如 r1=10, r2=4090, max=4096)
			// 如果两点距离超过半圈，说明跨越了 wrap-around
			if (abs(r1 - r2) > max / 2) {
				if (r1 < r2) {
					r1 += max;
				} else {
					r2 += max;
				}
			}

			int32_t avg = (r1 + r2) / 2;
			// 归一化
			while (avg >= max) {
				avg -= max;
			}
			while (avg < 0) {
				avg += max;
			}

			if (ed->counter >= 3) {
				avg = ed->avg_sum / ed->counter;
				ed->offset = avg;
				update_feedback_offset(fb, avg);
				ed->state = ENC_CALIB_STATE_LUT_START;
				ed->counter = 0;
				ed->avg_sum = 0;
			} else {
				ed->avg_sum += avg;
				ed->counter++;
				ed->state = ENC_CALIB_STATE_SCAN_FWD;
			}
		} else {
			ed->state = ENC_CALIB_STATE_ERROR;
		}
	} break;
	case ENC_CALIB_STATE_LUT_START: {
		_open_loop_voltage_vector_drive(inv, 0.0f, cfg->voltage);
		ed->time_acc += dt;
		if (ed->time_acc > ALIGN_DURATION) {
			ed->time_acc = 0.0f;
			ed->state = ENC_CALIB_STATE_LUT_SCAN_DRIVE;
		}
	} break;
	case ENC_CALIB_STATE_LUT_SCAN_DRIVE: {
		// 1. 计算目标位置并驱动
		struct feedback_config *fb_conf = fb->config;
		const float pp = (float)fb_conf->pole_pairs;
		const int32_t cpr_i = (int32_t)fb_conf->cpr;

		float mech_step = M_TWOPI / (float)LUT_SIZE;
		float mech_angle = (float)ed->lut_index * mech_step;
		ed->driver_elec_angle = mech_angle * pp;

		_open_loop_voltage_vector_drive(inv, ed->driver_elec_angle * RAD_TO_DEG,
						cfg->voltage);

		// 记录理想值供后续使用
		ed->ideal_rel_temp =
			(int32_t)(((float)ed->lut_index / (float)LUT_SIZE) * (float)cpr_i);

		// 进入等待状态
		ed->time_acc = 0.0f;
		ed->state = ENC_CALIB_STATE_LUT_SCAN_WAIT;
		break;
	}

	case ENC_CALIB_STATE_LUT_SCAN_WAIT: {
		struct feedback_config *fb_conf = fb->config;

		// 2. 等待转子稳定（至少50ms）
		ed->time_acc += dt;
		if (ed->time_acc < 0.1f) // 50ms稳定时间
		{
			break;
		}

		// 3. 稳定后读取编码器值
		uint32_t raw = read_feedback_raw(fb);

		/*在feedback计算角度时，在使用用offset*/
		// int32_t adjusted_raw = (int32_t)raw - (int32_t)ed->offset;
		// adjusted_raw %= fb_conf->cpr;
		// if (adjusted_raw < 0)
		// 	adjusted_raw += fb_conf->cpr;

		int32_t error = raw - ed->ideal_rel_temp;
		const int32_t cpr_half = fb_conf->cpr / 2;

		if (error > cpr_half) {
			error -= fb_conf->cpr;
		}
		if (error < -cpr_half) {
			error += fb_conf->cpr;
		}

		fb_conf->lut[ed->lut_index] = (int16_t)error;

		// 5. 递增索引，准备下一个点
		ed->lut_index++;
		if (ed->lut_index >= LUT_SIZE) {
			ed->lut_index = 0;
			ed->driver_elec_angle = 2 * M_PI;
			ed->state = ENC_CALIB_STATE_REBACK_ZERO;
		} else {
			ed->state = ENC_CALIB_STATE_LUT_SCAN_DRIVE;
		}
		break;
	}
	case ENC_CALIB_STATE_REBACK_ZERO: {
		float prev_angle = ed->driver_elec_angle;

		// 1. 更新角度 (反向)
		ed->driver_elec_angle -= cfg->speed * dt;

		float angle_deg = ed->driver_elec_angle * RAD_TO_DEG;
		_open_loop_voltage_vector_drive(inv, angle_deg, cfg->voltage);

		if (ed->driver_elec_angle <= 0.0f) {
			inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f); // 停机
			ed->state = ENC_CALIB_STATE_DEBUG;
		}
	} break;
	case ENC_CALIB_STATE_DEBUG: {
		struct feedback_config *fb_conf = fb->config;

		ed->time_acc += dt;

		if (ed->time_acc < 0.1f) {
			break;
		}
		ed->time_acc = 0.0f;
		static int16_t test_value_error;
		static int16_t test_value_error_zero;
		test_value_error_zero = fb_conf->lut[0];
		test_value_error = fb_conf->lut[ed->lut_index];
		if (ed->lut_index++ >= 127) {
			ed->lut_index = 0;
			ed->state = ENC_CALIB_STATE_COMPLETE;
		}
	} break;
	case ENC_CALIB_STATE_COMPLETE:
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
		ret = 1;
		break;

	case ENC_CALIB_STATE_ERROR:
		ret = -1;
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
		break;

	default:
		ed->state = ENC_CALIB_STATE_IDLE;
		break;
	}

	return ret;
}
