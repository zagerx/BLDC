#include "motor_carible.h"
#include "_current_calib.h"
#include "_encoder_calib.h"
#include "carlib_cfg.h"
#include "coord_transform.h"
#include "device.h"
#include "motor_cfg.h"
#include "openloop_voltage.h"
#include <stdint.h>

void calibration_modules_init(struct calibration_modules *calib, struct inverter_t *inverter,
			      struct currsmp_t *currsmp, struct feedback_t *feedback,
			      struct openloop_voltage *op, struct motor_parameters *params)
{
	if (calib == NULL) {
		return;
	}

	struct carlib_config *carlib_cfg = &calib->cfg;
	carlib_cfg->currsmp = currsmp;
	carlib_cfg->feedback = feedback;
	carlib_cfg->inverter = inverter;
	carlib_cfg->op = op;
	carlib_cfg->params = params;
	calib->state = CARIBLE_CURR_INIT;
	return;
}

int calibration_modules_update(struct calibration_modules *calib, float dt)
{
	struct carlib_current *curr_calib = &calib->curr_calib;
	struct carlib_encoder *ec_carlib = &calib->encoder_calib;
	struct carlib_config *carlib_cfg = &calib->cfg;
	int ret = 0;
	switch (calib->state) {
	case CARIBLE_CURR_INIT:
		carlib_current_init(curr_calib, carlib_cfg);
		calib->state = CARIBLE_CURR_RUNING;
		break;
	case CARIBLE_CURR_RUNING:
		if (curr_calib_update(curr_calib, dt) == 1) {
			calib->state = CARIBLE_CURR_DONE;
		}
		break;
	case CARIBLE_CURR_DONE:
		calib->state = CARIBLE_ENCODER_INIT;
		break;
	case CARIBLE_ENCODER_INIT: {
		carlib_encoder_init(ec_carlib, carlib_cfg);
		calib->state = CARIBLE_ENCODER_RUNING;

	} break;

	case CARIBLE_ENCODER_RUNING: {
		if (encoder_calib_update(ec_carlib, dt)) {
			calib->state = CARIBLE_ENCODER_DONE;
		}
	} break;
	case CARIBLE_ENCODER_DONE: {
		calib->state = ALL_CALIB_DONE;

	} break;

	case ALL_CALIB_DONE:
		ret = 1;
		break;
	case CARIBLE_ERR:
		ret = -1;
		break;

	default:
		break;
	}
	return ret;
}
