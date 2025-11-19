#include <stddef.h>
#include "motor.h"
#include <statemachine.h>
#include "_currment_carible.h"
#include "_pp_ident.h"
#include "motor_cfg.h"
#include "motor_carible.h"
#include "_encoder_carible.h"
#include "coord_transform.h"
#include "currsmp.h"
#include "feedback.h"
#include "inverter.h"
#include "svpwm.h"
#include "arm_math.h"
fsm_rt_t motor_idle_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;

	switch (obj->chState) {
	case ENTER:
		m_data->statue = MOTOR_STATE_IDLE;
		obj->chState = RUNING;
		break;
	case RUNING:
		break;
	case EXIT:
		break;
	default:
		break;
	}
	return 0;
}

fsm_rt_t motor_ready_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};

	switch (obj->chState) {
	case ENTER:
		obj->chState = RUNING;
		break;
	case RUNING:
		break;
	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}

fsm_rt_t motor_runing_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};

	switch (obj->chState) {
	case ENTER:
		break;
	case RUNING:
		break;
	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}

fsm_rt_t motor_stop_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;

	switch (obj->chState) {
	case ENTER:
		m_data->statue = MOTOR_STATE_STOP;
		break;
	case RUNING:
		break;
	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}

fsm_rt_t motor_falut_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;

	switch (obj->chState) {
	case ENTER:
		m_data->statue = MOTOR_STATE_FAULT;
		break;
	case RUNING:
		break;
	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}

fsm_rt_t motor_carible_state(fsm_cb_t *obj)
{
	struct device *motor = obj->p1;
	struct motor_config *m_cfg = motor->config;
	struct device *fb = m_cfg->feedback;
	struct motor_data *m_data = motor->data;
	struct device *pp_ident = m_data->calib->pp_ident;
	switch (obj->chState) {
	case ENTER:
		m_data->statue = MOTOR_STATE_CALIBRATION;
		curr_calib_start(motor, 10000);
		obj->chState = M_CARIBLE_CURR_RUNING;
		break;
	case M_CARIBLE_CURR_RUNING:
		curr_calib_update(motor);
		if (curr_calib_get_state(motor) == CURR_CALIB_STATE_DONE) {
			obj->chState = M_CARIBLE_CURR_DONE;
		} else if (curr_calib_get_state(motor) == CURR_CALIB_STATE_ERROR) {
			obj->chState = M_CARIBLE_ERR;
		} else {
		}
		break;
	case M_CARIBLE_CURR_DONE:
		pp_ident_start(pp_ident);
		obj->chState = M_CARIBLE_PP_RUNING;
		break;
	case M_CARIBLE_PP_RUNING:
		if (pp_ident_get_pp_state(pp_ident) == PP_CALIB_STATE_COMPLETE) {
			obj->chState = M_CARIBLE_PP_DONE;
		} else if (pp_ident_get_pp_state(pp_ident) == PP_CALIB_STATE_ERROR) {
			obj->chState = M_CARIBLE_ERR;
		}
		pp_ident_update(pp_ident, PWM_CYCLE); // 必须放在最后
		break;
	case M_CARIBLE_PP_DONE:
		encoder_calib_start(motor);
		obj->chState = M_CARIBLE_ENCODER_RUNING;
		break;
	case M_CARIBLE_ENCODER_RUNING:
		if (encoder_calib_get_state(motor) == ENC_CALIB_STATE_COMPLETE) {
			motor_set_calibstate(motor,M_ALL_CALIB_DONE);
			obj->chState = M_ALL_CALIB_DONE;
		} else if (encoder_calib_get_state(motor) == ENC_CALIB_STATE_ERROR) {
			obj->chState = M_CARIBLE_ERR;
		}
		encoder_calib_update(motor, PWM_CYCLE);
		break;
	case M_ALL_CALIB_DONE:
		break;
	case M_CARIBLE_ERR:
		break;
	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}

fsm_rt_t motor_encoder_openloop_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;
	struct motor_config *m_cfg = motor->config;
	struct device *feedback = m_cfg->feedback;
	struct feedback_data *fb_data = feedback->data;
	struct device *currsmp = m_cfg->currsmp;
	struct device *inverer = m_cfg->inverter;
	switch (obj->chState) {
	case ENTER:
		break;
	case RUNING:
		{
			feedback_update_angle_vel(feedback,PWM_CYCLE);
			currsmp_updata(currsmp);
			float sin_val, cos_val;
			sin_cos_f32(fb_data->elec_angle, &sin_val, &cos_val);			
			float ud, uq;
			float ualpha, ubeta;
			ud = 0.0f;
			uq = 0.02f;
			inv_park_f32(ud, uq, &ualpha, &ubeta, sin_val, cos_val);
			float duty[3];
			svm_set(ualpha, ubeta, duty);
			inverter_set_3phase_voltages(inverer, duty[0], duty[1], duty[2]);			
		}
		break;
	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}

