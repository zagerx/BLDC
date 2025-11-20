#include "motor_carible.h"
#include "_current_calib.h"
#include "_pp_ident.h"
#include "_encoder_carible.h"
#include "device.h"
#include "motor_cfg.h"
enum m_carible motor_get_calibstate(struct motor_calibration_modules *m_calib)
{
	return m_calib->state;
}
void motor_set_calibstate(struct motor_calibration_modules *m_calib, enum m_carible state)
{
	m_calib->state = state;
}

void motor_calib_update(struct motor_calibration_modules *calib)
{
	struct device *curr_calib = calib->current_calibration;
	struct device *pp_ident = calib->pp_ident;
	struct device *encoder_carib = calib->encoder_calibration;
	switch (calib->state) {
	case M_CARIBLE_CURR_STATR:
		curr_calib_start(curr_calib, 10000);
		calib->state = M_CARIBLE_CURR_RUNING;
		break;
	case M_CARIBLE_CURR_RUNING:
		if (curr_calib_update(curr_calib, PWM_CYCLE) == 1) {
			calib->state = M_CARIBLE_CURR_DONE;
		}
		break;
	case M_CARIBLE_CURR_DONE:
		pp_ident_start(pp_ident);
		calib->state = M_CARIBLE_PP_RUNING;
		break;
	case M_CARIBLE_PP_RUNING:
		if (pp_ident_get_pp_state(pp_ident) == PP_CALIB_STATE_COMPLETE) {
			calib->state = M_CARIBLE_PP_DONE;
		} else if (pp_ident_get_pp_state(pp_ident) == PP_CALIB_STATE_ERROR) {
			calib->state = M_CARIBLE_ERR;
		}
		pp_ident_update(pp_ident, PWM_CYCLE); // 必须放在最后
		break;
	case M_CARIBLE_PP_DONE:
		encoder_calib_start(encoder_carib);
		calib->state = M_CARIBLE_ENCODER_RUNING;
		break;
	case M_CARIBLE_ENCODER_RUNING:
		if (encoder_calib_get_state(encoder_carib) == ENC_CALIB_STATE_COMPLETE) {
			motor_set_calibstate(calib, M_ALL_CALIB_DONE);
			calib->state = M_ALL_CALIB_DONE;
		} else if (encoder_calib_get_state(encoder_carib) == ENC_CALIB_STATE_ERROR) {
			calib->state = M_CARIBLE_ERR;
		}
		encoder_calib_update(encoder_carib, PWM_CYCLE);
		break;
	case M_ALL_CALIB_DONE:
		break;
	case M_CARIBLE_ERR:
		break;

	default:
		break;
	}
}
