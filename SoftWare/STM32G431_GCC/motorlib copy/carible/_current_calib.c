#include "_current_calib.h"
#include "currsmp.h"
#include "inverter.h"
#include "device.h"
#include <stdint.h>
#include <stddef.h>

void curr_calib_start(struct device *curr_calib, uint16_t sample_count)
{
	if (curr_calib == NULL)
	{
		return;
	}

	struct device *cc = curr_calib;
	struct curr_calib_data *cd = cc->data;
	struct curr_calib_config *cc_cfg = cc->config;

	cc_cfg->sample_count = sample_count;

	cd->state = CURR_CALIB_STATE_ALIGN;

	cd->tim_acc = 0.0f;
	cd->sample_index = 0;

	// 整数归零
	cd->offset_a_acc = 0;
	cd->offset_b_acc = 0;
	cd->offset_c_acc = 0;
}

int32_t curr_calib_update(struct device *curr_calib, float dt)
{
	if (curr_calib == NULL || curr_calib->data == NULL || curr_calib->config == NULL)
	{
		return -1;
	}

	int32_t ret = 0;
	struct device *cc = curr_calib;

	struct curr_calib_data *cd = cc->data;
	struct curr_calib_config *cfg = cc->config;
	struct device *inv = cfg->inverter;
	struct device *currsmp = cfg->currsmp;
	struct currsmp_data *currsmp_data = currsmp->data;

	switch (cd->state)
	{

	case CURR_CALIB_STATE_ALIGN:
	{
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);

		cd->tim_acc += dt;
		if (cd->tim_acc >= cfg->align_duration)
		{
			cd->tim_acc = 0.0f;
			cd->state = CURR_CALIB_STATE_SAMPLING;
		}
	}
	break;

	case CURR_CALIB_STATE_SAMPLING:
	{
		if (cd->sample_index >= cfg->sample_count)
		{
			cd->state = CURR_CALIB_STATE_PROCESSING;
			break;
		}

		// 整数累加
		// 假设 channle_raw_x 是 uint16_t 或 uint32_t
		uint32_t channle_raw[4];
		_read_currsmp_raw(currsmp, channle_raw);
		cd->offset_a_acc += channle_raw[0];
		cd->offset_b_acc += channle_raw[1];
		cd->offset_c_acc += channle_raw[2];

		cd->sample_index++;
	}
	break;

	case CURR_CALIB_STATE_PROCESSING:
	{
		if (cfg->sample_count == 0)
		{
			cd->state = CURR_CALIB_STATE_ERROR;
			break;
		}

		/* 整数除法
		 * 例如：累加和 20485，采样 10 次 -> 结果 2048
		 * 小数部分直接被舍弃 (向下取整)
		 */
		// currsmp_data->offset_a = cd->offset_a_acc / cfg->sample_count;
		// currsmp_data->offset_b = cd->offset_b_acc / cfg->sample_count;
		// currsmp_data->offset_c = cd->offset_c_acc / cfg->sample_count;
		uint32_t offset_buf[3];
		offset_buf[0] = cd->offset_a_acc / cfg->sample_count;
		offset_buf[1] = cd->offset_b_acc / cfg->sample_count;
		offset_buf[2] = cd->offset_c_acc / cfg->sample_count;
		_update_currsmp_offset(currsmp, offset_buf);
		cd->state = CURR_CALIB_STATE_DONE;
	}
	break;

	case CURR_CALIB_STATE_DONE:
	{
		ret = 1;
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
	}
	break;

	case CURR_CALIB_STATE_ERROR:
	{
		ret = -1;
	}
	break;

	default:
		break;
	}

	return ret;
}