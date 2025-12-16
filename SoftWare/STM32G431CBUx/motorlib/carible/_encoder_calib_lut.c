#include "_encoder_calib.h"
#include <stdint.h>
#include <math.h>
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "svpwm.h"
#include <stdlib.h> // for abs
#include "coord_transform.h"
#include <stdlib.h> // for abs, malloc, free
#undef M_PI
#define M_PI 3.14159265358979323846f

#undef M_TWOPI
#define M_TWOPI    (2.0f * M_PI)
#define RAD_TO_DEG (180.0f / M_PI)

#define ALIGN_DURATION 2.0f // 对齐时间S

// 扫描范围：从 -PI 转到 +PI，确保经过 0 点
#define SCAN_RANGE_START (-M_PI)
#define SCAN_RANGE_END   (M_PI)
#undef LUT_SIZE
#define LUT_SIZE 256
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
		float angle_deg = 0.0f;
		float s, c;
		sin_cos_f32(angle_deg, &s, &c);

		float abc[3];
		svm_set(v_mag * c, v_mag * s, abc);
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

		ed->time_acc += dt;
		if (ed->time_acc > ALIGN_DURATION) { // 等待转子稳定在 -PI
			ed->time_acc = 0.0f;
			ed->state = ENC_CALIB_STATE_LUT_SETUP;
		}
	} break;
		/* -----------------------------------------------------
		 * Step 5: LUT 内存申请和准备
		 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_LUT_SETUP: {
		// 1. 申请内存 (在堆上申请，校准完后指针移交给 feedback 模块，不释放)
		ed->lut_buf_ptr = (uint32_t *)malloc(LUT_SIZE * sizeof(uint32_t));

		if (ed->lut_buf_ptr == NULL) {
			ed->state = ENC_CALIB_STATE_ERROR;
		} else {
			ed->lut_index = 0;
			ed->time_acc = 0.0f;
			ed->driver_elec_angle = 0.0f;
			// 使用非常小的电压进行软拖动，提高精度
			ed->state = ENC_CALIB_STATE_LUT_SCAN;
		}
	} break;

	/* -----------------------------------------------------
	 * Step 6: 扫描并填充 LUT (自基准线性误差计算)
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_LUT_SCAN: {
		struct feedback_config *fb_conf = fb->config;
		const float pp = (float)fb_conf->pole_pairs;
		const int32_t cpr_i = (int32_t)fb_conf->cpr;
		const int32_t cpr_half = cpr_i / 2;

		// 1. 驱动电机到目标位置
		// 目标机械角度 = (Index / Size) * 2PI
		// 目标电角度   = 目标机械角度 * 极对数
		float mech_step = M_TWOPI / (float)LUT_SIZE;
		float target_mech_angle = (float)ed->lut_index * mech_step;

		ed->driver_elec_angle = target_mech_angle * pp;

		// SVM 输出 (小电压，例如 0.02 ~ 0.1)
		float angle_deg = ed->driver_elec_angle * RAD_TO_DEG;
		float s, c;
		sin_cos_f32(angle_deg, &s, &c);
		float abc[3];
		// 注意：这里电压一定要小，防止电机震荡，但要足够克服摩擦
		float scan_voltage = 0.02f;
		svm_set(scan_voltage * c, scan_voltage * s, abc);
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

		// 2. 稳定与采样
		ed->time_acc += dt;
		if (ed->time_acc >= 0.2f) { // 等待 50ms 稳定
			ed->time_acc = 0.0f;

			// --- 采样平均 ---
			uint64_t raw_acc = 0;
			for (int j = 0; j < 64; j++) {
				raw_acc += read_feedback_raw(fb);
			}
			int32_t raw_avg = (int32_t)(raw_acc / 64);
			raw_avg %= cpr_i;

			// ======================================================
			// 核心修改：自基准计算，不依赖旧 Offset
			// ======================================================

			// 情况 A: 如果是第一个点 (Index 0)
			if (ed->lut_index == 0) {
				// 1. 定义偏差为 0 (Bias后为 CPR/2)
				ed->lut_buf_ptr[0] = (uint32_t)cpr_half;

				// 2. 【关键】将当前读数强制设定为系统的 Zero Offset
				// 这意味着：以后运行时的所有 Raw 值，都会先减去这个值
				update_feedback_offset(fb, (uint16_t)raw_avg);

			}
			// 情况 B: 后续点 (Index > 0)
			else {
				// 1. 获取刚刚设定好的基准 Offset
				int32_t base_offset = (int32_t)fb_conf->offset;

				// 2. 计算测量出的相对位置 (Measured Relative)
				int32_t meas_rel = raw_avg - base_offset;
				// 处理环绕，使其在 [0, CPR) 范围内
				if (meas_rel < 0) {
					meas_rel += cpr_i;
				}
				meas_rel %= cpr_i;

				// 3. 计算理想的相对位置 (Ideal Relative)
				// 理想位置 = (Index / Size) * CPR
				int32_t ideal_rel =
					(int32_t)(((float)ed->lut_index / (float)LUT_SIZE) *
						  (float)cpr_i);

				// 4. 计算误差 (Error = Measured - Ideal)
				int32_t error = meas_rel - ideal_rel;

				// 5. 修正误差环绕 (最短路径原则)
				// 例如：Meas=10, Ideal=4090 (CPR=4096) -> Error should be +16, not
				// -4080
				if (error > cpr_half) {
					error -= cpr_i;
				}
				if (error < -cpr_half) {
					error += cpr_i;
				}

				// 6. 存储误差 (加上 Bias: CPR/2 以存为无符号数)
				ed->lut_buf_ptr[ed->lut_index] = (uint32_t)(error + cpr_half);
			}

			// === 索引递增 ===
			ed->lut_index++;
			if (ed->lut_index >= LUT_SIZE) {
				ed->state = ENC_CALIB_STATE_LUT_FINALIZE;
			}
		}
	} break;

	/* -----------------------------------------------------
	 * Step 7: 提交 LUT
	 * ----------------------------------------------------- */
	case ENC_CALIB_STATE_LUT_FINALIZE: {
		// 将生成的表传递给 feedback 模块
		feedback_set_error_lut(fb, ed->lut_buf_ptr);

		// 停机
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
		ed->state = ENC_CALIB_STATE_COMPLETE;
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
