#include "_current_calib.h"
#include "motor_params.h"
#include "currsmp.h"
#include "inverter.h"
#include "device.h"
#include <stdint.h>
#include <stddef.h>

void carlib_current_init(struct carlib_current *carlib, struct carlib_config *cfg)
{
	if (carlib == NULL) {
		return;
	}
	carlib->cfg = cfg;
	/* 初始化 */
	carlib->a_channle_sum = 0;
	carlib->b_channle_sum = 0;
	carlib->c_channle_sum = 0;
	carlib->sample_conut = 0;

	carlib->align_timer = 0.0f;
	carlib->align_time = 0.005f;
	carlib->sample_target = 2048;
	carlib->state = CURR_CALIB_STATE_IDLE;
}

int32_t curr_calib_update(struct carlib_current *carlib, float dt)
{
	if (carlib == NULL || carlib->cfg == NULL) {
		return -1;
	}

	struct carlib_config *cfg = carlib->cfg;
	struct currsmp_t *currsmp = cfg->currsmp;
	struct inverter_t *inverter = cfg->inverter;
	struct motor_parameters *m_parms = cfg->params;
	switch (carlib->state) {

	case CURR_CALIB_STATE_IDLE:

		inverter_set_3phase_enable(inverter);
		carlib->state = CURR_CALIB_STATE_ALIGN;
		break;

	case CURR_CALIB_STATE_ALIGN:
		/* 强制逆变器零输出 */
		inverter_set_3phase_voltages(inverter, 0.0f, 0.0f, 0.0f);

		carlib->align_timer += dt;
		if (carlib->align_timer >= carlib->align_time) {
			carlib->state = CURR_CALIB_STATE_SAMPLING;
		}
		break;

	case CURR_CALIB_STATE_SAMPLING: {
		uint32_t iabc[3];

		_read_currsmp_raw(currsmp, iabc);

		carlib->a_channle_sum += iabc[0];
		carlib->b_channle_sum += iabc[1];
		carlib->c_channle_sum += iabc[2];

		carlib->sample_conut++;

		if (carlib->sample_conut >= carlib->sample_target) {
			carlib->state = CURR_CALIB_STATE_PROCESSING;
		}
	} break;

	case CURR_CALIB_STATE_PROCESSING:
		if (carlib->sample_conut == 0) {
			carlib->state = CURR_CALIB_STATE_ERROR;
			break;
		}

		/* 求均值 */
		uint32_t offset_abc[3];
		offset_abc[0] = (float)carlib->a_channle_sum / carlib->sample_conut;
		offset_abc[1] = (float)carlib->b_channle_sum / carlib->sample_conut;
		offset_abc[2] = (float)carlib->c_channle_sum / carlib->sample_conut;
		_update_currsmp_offset_params(m_parms, offset_abc[0], offset_abc[1], offset_abc[2]);
		inverter_set_3phase_disable(inverter);

		carlib->state = CURR_CALIB_STATE_DONE;
		break;

	case CURR_CALIB_STATE_DONE:
		return 1; // 校准完成

	case CURR_CALIB_STATE_ERROR:
	default:
		inverter_set_3phase_disable(inverter);
		return -1;
	}

	return 0; // 进行中
}
