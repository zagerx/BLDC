#include "_encoder_calib.h"
#include <stdint.h>
#include <math.h>
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "coord_transform.h"
#include "svpwm.h"
#include <stddef.h>

#undef M_PI
#define M_PI 3.14159265358979323846f
#undef M_TWOPI
#define M_TWOPI    (2.0f * M_PI)
#define RAD_TO_DEG (180.0f / M_PI)

/* unwrap_raw（处理编码器跨零点跳变） */
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
 * 启动：初始化状态
 * --------------------------------------------------------- */
void encoder_calib_start(struct device *encoder_calib)
{
	if (!encoder_calib || !encoder_calib->data || !encoder_calib->config) {
		return;
	}

	struct device *enc_dev = encoder_calib;
	struct encoder_calib_data *ed = enc_dev->data;

	/* 初始化 */
	ed->state = ENC_CALIB_STATE_ALIGN;
	ed->time_acc = 0.0f;
	ed->elec_angle = 0.0f;
	ed->raw_delta_acc = 0;
}

/* ---------------------------------------------------------
 * 主更新周期（每个控制周期调用）
 * 返回值: 0=进行中, 1=完成, -1=错误
 * --------------------------------------------------------- */
int32_t encoder_calib_update(struct device *encoder_calib, float dt)
{
	// 【防御性检查】
	if (!encoder_calib || !encoder_calib->data || !encoder_calib->config) {
		return -1;
	}

	int32_t ret = 0;
	struct device *enc_dev = encoder_calib;
	struct encoder_calib_data *ed = enc_dev->data;
	struct encoder_calib_config *cfg = enc_dev->config;
	struct device *inv = cfg->inverter;
	struct device *fb = cfg->feedback;
	struct feedback_config *fb_cfg = fb->config;

	switch (ed->state) {

	/* -----------------------------------------------------
	 * ALIGN：施加固定电压/电流，使转子拉到已知电角度（电角度 0）
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_ALIGN: {
		/* 施加一个固定电压向量（电角度 = 0）使转子对齐 */
		float v_mag = cfg->align_voltage;
		if (v_mag > 0.577f) {
			v_mag = 0.577f;
		}
		if (v_mag < 0.01f) {
			v_mag = 0.01f;
		}

		/* 电角度 0 -> alpha = v_mag, beta = 0 */
		float abc[3];
		svm_set(v_mag, 0.0f, abc);
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

		ed->time_acc += dt;

		if (ed->time_acc >= cfg->align_duration) {
			/* 记录对齐后编码器原始值作为候选零位 */
			uint32_t raw_start_val = fb_cfg->get_raw();

			// 将 raw_start 直接存入
			feedback_set_offset(fb, raw_start_val);

			ed->raw_prev = (int32_t)raw_start_val;
			ed->raw_delta_acc = 0;
			ed->time_acc = 0.0f;
			ed->elec_angle = 0.0f; // 确保开环从 0 角度开始

			ed->state = ENC_CALIB_STATE_ROTATE_TEST;
		}
	} break;

	/* -----------------------------------------------------
	 * ROTATE_TEST：小幅开环旋转（单向），判断方向
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_ROTATE_TEST: {
		if (ed->time_acc >= cfg->rotate_duration) {
			ed->state = ENC_CALIB_STATE_PROCESSING;
			// 停止输出
			inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
			break;
		}

		ed->time_acc += dt;

		/* 使用电压矢量以 openloop_speed 控制电角度 */
		float angle_step = cfg->openloop_speed * dt;
		ed->elec_angle += angle_step;

		/* 限制 elec_angle 在 0~2PI 范围内 */
		if (ed->elec_angle > M_TWOPI) {
			ed->elec_angle -= M_TWOPI;
		} else if (ed->elec_angle < 0.0f) {
			ed->elec_angle += M_TWOPI;
		}

		float angle_deg = ed->elec_angle * RAD_TO_DEG;

		float v_mag = cfg->openloop_voltage;
		if (v_mag > 0.577f) {
			v_mag = 0.577f;
		}
		if (v_mag < 0.01f) {
			v_mag = 0.01f;
		}

		float sinv, cosv;
		sin_cos_f32(angle_deg, &sinv, &cosv);

		float abc[3];
		svm_set(v_mag * cosv, v_mag * sinv, abc);
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

		/* 读取并 unwrap 编码器 raw 累积 diff */
		int32_t raw = (int32_t)fb_cfg->get_raw();
		int32_t delta = unwrap_raw(raw, &ed->raw_prev, (int32_t)cfg->encoder_max);
		ed->raw_delta_acc += delta;

	} break;

	/* -----------------------------------------------------
	 * PROCESSING：计算零偏（zero offset）并做最终验证
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_PROCESSING: {

		/* 计算机械圈数（累积差 / encoder_max） */
		float mech_rounds = (float)ed->raw_delta_acc / (float)cfg->encoder_max;

		/* 如果旋转量太小，认为失败 */
		if (fabsf(mech_rounds) < cfg->min_rotation_frac) {
			ed->state = ENC_CALIB_STATE_ERROR;
			break;
		}

		/* 判断方向：如果 raw_delta_acc 与施加电角度的方向一致，polarity 为 +1，否则为 -1 */
		/* 注意：openloop_speed 正负决定了我们施加的旋转方向 */
		int16_t polarity;
		if ((ed->raw_delta_acc > 0 && cfg->openloop_speed > 0.0f) ||
		    (ed->raw_delta_acc < 0 && cfg->openloop_speed < 0.0f)) {
			polarity = 1;
		} else {
			polarity = -1;
		}

		// 【保存结果】
		feedback_set_direction(fb, polarity);

		ed->state = ENC_CALIB_STATE_COMPLETE;

	} break;

	/* -----------------------------------------------------
	 * COMPLETE：完成
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_COMPLETE: {
		ret = 1;
	} break;

	/* -----------------------------------------------------
	 * ERROR：失败
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_ERROR: {
		ret = -1;
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
	} break;

	default:
		ed->state = ENC_CALIB_STATE_IDLE;
		break;
	}

	return ret;
}
