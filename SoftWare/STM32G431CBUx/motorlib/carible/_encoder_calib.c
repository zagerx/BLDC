#include "_encoder_calib.h"
#include <stdint.h>
#include <math.h>
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "svpwm.h"
#include <stdlib.h> // for abs
#include "coord_transform.h"

#undef M_PI
#define M_PI 3.14159265358979323846f

#undef M_TWOPI
#define M_TWOPI    (2.0f * M_PI)
#define RAD_TO_DEG (180.0f / M_PI)

#define ALIGN_DURATION 2.0f // 对齐时间S

// 扫描范围：从 -PI 转到 +PI，确保经过 0 点
#define SCAN_RANGE_START (-M_PI)
#define SCAN_RANGE_END   (M_PI)

/* 辅助：unwrap 处理 */
static inline int32_t unwrap_raw(uint32_t current, uint32_t *prev, uint32_t max)
{
	int32_t diff = current - *prev;
	int32_t half = max / 2;
	if (diff > half) {
		diff -= max;
	} else if (diff < -half) {
		diff += max;
	}
	*prev = current;
	return diff;
}

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

	// 电压限幅
	float v_mag = cfg->voltage;
	if (v_mag > 0.577f) {
		v_mag = 0.577f;
	}
	if (v_mag < 0.01f) {
		v_mag = 0.01f;
	}

	switch (ed->state) {

	/* -----------------------------------------------------
	 * Step 1: 定位到扫描起点 (-PI)
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_ALIGN_START: {
		// 施加起始角度电压
		float angle_deg = ed->driver_elec_angle * RAD_TO_DEG;
		float s, c;
		sin_cos_f32(angle_deg, &s, &c);

		float abc[3];
		svm_set(v_mag * c, v_mag * s, abc);
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

		ed->time_acc += dt;
		if (ed->time_acc > ALIGN_DURATION) { // 等待转子稳定在 -PI
			ed->time_acc = 0.0f;
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
		float s, c;
		sin_cos_f32(angle_deg, &s, &c);
		float abc[3];
		svm_set(v_mag * c, v_mag * s, abc);
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

		// 4. 【关键】检测过零点 (Zero Crossing)
		// 如果上一刻小于0，这一刻大于等于0，说明跨过了0度
		uint32_t raw_curr = read_feedback_raw(fb);
		if (prev_angle < 0.0f && ed->driver_elec_angle >= 0.0f) {
			// 简单的最近邻采样 (因为 dt 很小，插值提升有限且复杂)
			// 记录当前的 raw 值作为 FWD 零点
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

		// 2. SVM 输出
		float angle_deg = ed->driver_elec_angle * RAD_TO_DEG;
		float s, c;
		sin_cos_f32(angle_deg, &s, &c);
		float abc[3];
		svm_set(v_mag * c, v_mag * s, abc);
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

		// 3. 更新 raw_prev (用于 unwrap 连续性)
		uint32_t raw_curr = read_feedback_raw(fb);

		// 4. 【关键】检测过零点
		// 如果上一刻大于0，这一刻小于等于0
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

			write_feedback_offset(fb, (uint32_t)avg);
			ed->state = ENC_CALIB_STATE_COMPLETE;
		} else {
			// 如果没捕获到 (速度过快或配置错误)，报错
			ed->state = ENC_CALIB_STATE_ERROR;
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
