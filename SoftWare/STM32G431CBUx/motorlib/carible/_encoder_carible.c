#include <stdint.h>
#include <math.h>
#include "_encoder_carible.h"
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "coord_transform.h"
#include "svpwm.h"

#undef M_PI
#define M_PI 3.1415926f

#define ENC_ALIGN_COUNT       2000   // 对齐等待计数（近似 ms 级）
#define ENC_ROTATE_COUNT      20000  // 旋转采样计数（用于判断方向）
#define ENC_MIN_ROTATION_FRAC 0.005f // 最小机械圈数(0.5%) 判定旋转有效

/* 与其它模块一致的 unwrap_raw（处理编码器跨零点跳变） */
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

	struct device *enc_dev = encoder_calib;
	struct encoder_calib_data *ed = enc_dev->data;
	struct encoder_calib_config *cfg = enc_dev->config;
	struct device *fb = cfg->feedback;
	struct feedback_config *fb_cfg = fb->config;
	if (!cfg) {
		/* 配置缺失，标记为完成且失败 */
		ed->state = ENC_CALIB_STATE_ERROR;
		ed->done = true;
		ed->zero_offset = 0;
		return;
	}

	/* 初始化 */
	ed->state = ENC_CALIB_STATE_ALIGN;
	ed->done = false;

	ed->align_count = 0;
	ed->sample_count = 0;

	ed->raw_start = (int32_t)fb_cfg->get_raw();
	ed->raw_prev = ed->raw_start;
	ed->raw_delta_acc = 0;

	ed->zero_offset = 0;
	ed->polarity = 1; /* 1: unknown/assume +1 until measured */
}
#include "stdio.h"
/* ---------------------------------------------------------
 * 主更新周期（每个控制周期调用）
 * --------------------------------------------------------- */
void encoder_calib_update(struct device *encoder_calib, float dt)
{
	struct device *enc_dev = encoder_calib;
	struct encoder_calib_data *ed = enc_dev->data;
	struct encoder_calib_config *cfg = enc_dev->config;
	struct device *inv = cfg->inverter;
	struct device *fb = cfg->feedback;
	struct feedback_config *fb_cfg = fb->config;

	if (!cfg) {
		ed->state = ENC_CALIB_STATE_ERROR;
		ed->done = true;
		return;
	}

	switch (ed->state) {

	/* -----------------------------------------------------
	 * ALIGN：输出定向电压/电流，使转子拉到已知电角度（电角度 0）
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
		float ualpha = v_mag;
		float ubeta = 0.0f;
		float abc[3];
		svm_set(ualpha, ubeta, abc);
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

		/* 等待 ADC/力矩稳定 */
		if (++ed->align_count >= ENC_ALIGN_COUNT) {
			/* 记录对齐后编码器原始值作为候选零位 */
			ed->raw_start = (int32_t)fb_cfg->get_raw();
			ed->raw_prev = ed->raw_start;
			ed->raw_delta_acc = 0;
			ed->sample_count = 0;
			ed->align_count = 0;
			ed->state = ENC_CALIB_STATE_ROTATE_TEST;
		}
	} break;

	/* -----------------------------------------------------
	 * ROTATE_TEST：小幅开环旋转（单向），实时累积编码器增量以判断方向
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_ROTATE_TEST: {
		/* 在这里我们以配置的角速度做短时间的开环旋转来判断编码器方向 */
		/* 使用电压矢量以 openloop_speed 控制电角度 */
		ed->time_acc += dt;
		ed->elec_angle += cfg->openloop_speed * dt;
		/* 防止 elec_angle 过大 */
		if (ed->elec_angle > 2.0f * M_PI) {
			ed->elec_angle -= 2.0f * M_PI;
		}

		float angle = ed->elec_angle * (180.0f / M_PI);
		float v_mag = cfg->openloop_voltage;
		if (v_mag > 0.577f) {
			v_mag = 0.577f;
		}
		if (v_mag < 0.01f) {
			v_mag = 0.01f;
		}

		float sinv, cosv;
		sin_cos_f32(angle, &sinv, &cosv);
		float ualpha = v_mag * cosv;
		float ubeta = v_mag * sinv;
		float abc[3];
		svm_set(ualpha, ubeta, abc);
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

		/* 读取并 unwrap 编码器 raw 累积 diff */
		int32_t raw = (int32_t)fb_cfg->get_raw();
		int32_t delta = unwrap_raw(raw, &ed->raw_prev, (int32_t)cfg->encoder_max);
		ed->raw_delta_acc += delta;
		ed->sample_count++;

		/* 判断采样是否足够（使用计数而不是时间，风格与其它校准一致） */
		if (ed->sample_count >= ENC_ROTATE_COUNT) {
			/* 记录最终 raw_end */
			ed->raw_end = raw;

			/* 计算机械圈数（累积差 / encoder_max） */
			ed->mech_rounds = (float)ed->raw_delta_acc / (float)cfg->encoder_max;

			/* 如果旋转量太小，认为失败 */
			if (fabsf(ed->mech_rounds) < ENC_MIN_ROTATION_FRAC) {
				ed->state = ENC_CALIB_STATE_ERROR;
				break;
			}

			/* 判断方向：如果 raw_delta_acc 与施加电角度的方向一致，polarity 为
			 * +1，否则为 -1 */
			/* 这里 openloop_speed > 0 表示我们按配置向“正”方向旋转 */
			if ((ed->raw_delta_acc > 0 && cfg->openloop_speed > 0.0f) ||
			    (ed->raw_delta_acc < 0 && cfg->openloop_speed < 0.0f)) {
				ed->polarity = 1;
			} else {
				ed->polarity = -1;
			}
			feedback_set_direction(fb,ed->polarity);
			/* 进入处理阶段 */
			ed->state = ENC_CALIB_STATE_PROCESSING;
		}
	} break;

	/* -----------------------------------------------------
	 * PROCESSING：计算零偏（zero offset）并做最终验证
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_PROCESSING: {
		/* 我们在 ALIGN 阶段把转子拉到电角度 0，并在那一刻读取 raw_start，
		   因此零偏（encoder counts） = raw_start - 电角度映射(0)。
		   对于常见的情况，电角度 0 对应机械角 0（取决于电机接线），
		   这里直接把 raw_start 作为零偏，并保存 polarity。 */

		/* 保存结果 */
		ed->zero_offset = ed->raw_start; /* encoder 原始值对应电角度 0 */
		feedback_set_offset(fb,ed->zero_offset);
		ed->done = true;

		/* 停止输出 */
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);

		ed->state = ENC_CALIB_STATE_COMPLETE;
	} break;

	/* -----------------------------------------------------
	 * COMPLETE：完成
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_COMPLETE: {
		/* nothing to do，保持 done 标志 */
	} break;

	/* -----------------------------------------------------
	 * ERROR：失败
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_ERROR: {
		ed->done = true;
		ed->zero_offset = 0;
		ed->polarity = 1;
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
	} break;

	default:
		break;
	}
}

/* ---------------------------------------------------------
 * 查询状态
 * --------------------------------------------------------- */
enum encoder_calib_state encoder_calib_get_state(struct device *encoder_calib)
{
	struct device *enc_dev = encoder_calib;
	struct encoder_calib_data *ed = enc_dev->data;
	return ed->state;
}
