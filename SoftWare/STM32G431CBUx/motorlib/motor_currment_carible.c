#include "currsmp.h"
#include "inverter.h"
#include "motor.h"
#include "motor_currment_carible.h"
#include "device.h"
#include "inverter.h"
#include "feedback.h"
#include "main.h"
#include <stdint.h>

/* ---------------------------------------------------------
 * 初始化
 * --------------------------------------------------------- */
void curr_calib_start(struct device *motor, uint16_t ample_count)
{
	struct motor_data *m_data = (struct motor_data *)motor->data;
	struct device *cc = m_data->calib->current_calibration;
	struct curr_calib_data *cd = cc->data;
	struct curr_calib_config *cc_cfg = cc->config;
	cc_cfg->sample_count = ample_count;

	cd->state = CURR_CALIB_STATE_ALIGN;
	cd->done = false;

	cd->sample_index = 0;
	cd->offset_a_acc = 0;
	cd->offset_b_acc = 0;
	cd->offset_c_acc = 0;
}

/* ---------------------------------------------------------
 * 主更新
 * --------------------------------------------------------- */
void curr_calib_update(struct device *motor)
{
	struct motor_config *mc = (struct motor_config *)motor->config;
	struct motor_data *m_data = (struct motor_data *)motor->data;

	struct device *inv = mc->inverter;
	// struct device *fb = mc->feedback;
	struct device *cc = m_data->calib->current_calibration;

	struct curr_calib_data *cd = cc->data;
	struct curr_calib_config *cfg = cc->config;

	struct device *currsmp = mc->currsmp;
	struct currsmp_data *currsmp_data = currsmp->data;
	switch (cd->state) {

	/* -----------------------------------------------------
	 * Step 1: ALIGN（关闭PWM 或 输出50%）
	 * ----------------------------------------------------- */
	case CURR_CALIB_STATE_ALIGN: {
		float abc[3] = {0.0f, 0.0f, 0.0f};
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

		// 等待 ADC 稳定，留一点时间
		static uint32_t align_count = 0;
		if (++align_count > 2000) { // 大约2~3ms
			align_count = 0;
			cd->state = CURR_CALIB_STATE_SAMPLING;
		}
	} break;

	/* -----------------------------------------------------
	 * Step 2: 多次采样电流值
	 * ----------------------------------------------------- */
	case CURR_CALIB_STATE_SAMPLING: {
		if (cd->sample_index >= cfg->sample_count) {
			cd->state = CURR_CALIB_STATE_PROCESSING;
			break;
		}

		cd->offset_a_acc += currsmp_data->channle_raw_a;
		cd->offset_b_acc += currsmp_data->channle_raw_b;
		cd->offset_c_acc += currsmp_data->channle_raw_c;

		cd->sample_index++;
	} break;

	/* -----------------------------------------------------
	 * Step 3: 求平均值作为offset
	 * ----------------------------------------------------- */
	case CURR_CALIB_STATE_PROCESSING: {
		if (cfg->sample_count == 0) {
			cd->state = CURR_CALIB_STATE_ERROR;
			break;
		}

		cd->offset_a = cd->offset_a_acc / cfg->sample_count;
		cd->offset_b = cd->offset_b_acc / cfg->sample_count;
		cd->offset_c = cd->offset_c_acc / cfg->sample_count;

		cd->state = CURR_CALIB_STATE_DONE;
	} break;

	/* -----------------------------------------------------
	 * Step 4: 完成
	 * ----------------------------------------------------- */
	case CURR_CALIB_STATE_DONE: {
		cd->done = true;
		currsmp_data->offset_a = cd->offset_a;
		currsmp_data->offset_b = cd->offset_b;
		currsmp_data->offset_c = cd->offset_c;
		float abc[3] = {0, 0, 0};
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);
	} break;

	/* -----------------------------------------------------
	 * Step 5: ERROR
	 * ----------------------------------------------------- */
	case CURR_CALIB_STATE_ERROR: {
		cd->done = true;
		// mc->ia_offset = mc->ib_offset = mc->ic_offset = 0;
	} break;

	default:
		break;
	}
}


enum curr_calib_state curr_calib_get_state(struct device* motor)
{
	struct motor_data *m_data = (struct motor_data *)motor->data;

	struct device *cc = m_data->calib->current_calibration;

	struct curr_calib_data *cd = cc->data;	
	return cd->state;
}