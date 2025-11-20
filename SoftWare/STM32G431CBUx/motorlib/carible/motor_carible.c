#include "motor_carible.h"
#include "_current_calib.h"
#include "_pp_ident.h"
#include "_encoder_calib.h"
#include "device.h"
#include "motor_cfg.h"
#include <stdint.h>
int32_t motor_calib_update(struct motor_calibration_modules *calib, float dt)
{
	struct device *curr_calib = calib->current_calibration;
	struct device *pp_ident = calib->pp_ident;
	struct device *encoder_carib = calib->encoder_calibration;
	int32_t ret = 0;
	switch (calib->state) {
	case M_CARIBLE_CURR_STATR:
		curr_calib_start(curr_calib, 10000);
		calib->state = M_CARIBLE_CURR_RUNING;
		break;
	case M_CARIBLE_CURR_RUNING:
		if (curr_calib_update(curr_calib, dt) == 1) {
			calib->state = M_CARIBLE_CURR_DONE;
		}
		break;
	case M_CARIBLE_CURR_DONE:
		pp_ident_start(pp_ident);
		calib->state = M_CARIBLE_PP_RUNING;
		break;
	case M_CARIBLE_PP_RUNING:
		if (pp_ident_update(pp_ident, dt) == 1) {
			calib->state = M_CARIBLE_PP_DONE;
		}
		break;
	case M_CARIBLE_PP_DONE:
		encoder_calib_start(encoder_carib);
		calib->state = M_CARIBLE_ENCODER_RUNING;
		break;
	case M_CARIBLE_ENCODER_RUNING:
		if (encoder_calib_update(encoder_carib, PWM_CYCLE) == 1) {
			calib->state = M_ALL_CALIB_DONE;
		}
		break;
	case M_ALL_CALIB_DONE:
		ret = 1;
		break;
	case M_CARIBLE_ERR:
		ret = -1;
		break;

	default:
		break;
	}
	return ret;
}
