#include <stddef.h>
#include "motor.h"
#include <statemachine.h>
#include "_current_calib.h"
#include "_pp_ident.h"
#include "motor_cfg.h"
#include "motor_carible.h"
#include "coord_transform.h"
#include "currsmp.h"
#include "feedback.h"
#include "inverter.h"
#include "svpwm.h"
#include "arm_math.h"

fsm_rt_t motor_encoder_openloop_state(fsm_cb_t *obj);

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
	enum {
		RUNNING = USER_STATUS,
	};
	struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;

	switch (obj->chState) {
	case ENTER:
		obj->chState = RUNNING;
		break;
	case RUNNING:
		if (motor_calib_update(m_data->calib, PWM_CYCLE) == 1) {
			TRAN_STATE(m_data->state_machine, motor_encoder_openloop_state);
		}
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
	struct motor_data *m_data = motor->data;
	struct motor_config *m_cfg = motor->config;
	struct device *feedback = m_cfg->feedback;
	struct foc_parameters *foc_param = &(m_data->foc_data);
	struct device *currsmp = m_cfg->currsmp;
	struct device *inverer = m_cfg->inverter;
	switch (obj->chState) {
	case ENTER:
		if (feedback_init(feedback)) {
			break;
		}
		obj->chState = RUNING;
		break;
	case RUNING: {
		feedback_update(feedback, PWM_CYCLE);
		float elec_angle = read_feedback_elec_angle(feedback);

		float i_abc[3];
		float i_alpha, i_beta;
		currsmp_update_currents(currsmp, i_abc);
		clarke_f32(i_abc[0], i_abc[1], &i_alpha, &i_beta);
		update_focparam_idq(foc_param, i_alpha, i_beta, elec_angle);

		float sin_val, cos_val;
		sin_cos_f32(elec_angle * (180.0f / M_PI), &sin_val, &cos_val);
		float ud, uq;
		float ualpha, ubeta;
		ud = 0.0f;
		uq = -0.02f;
		inv_park_f32(ud, uq, &ualpha, &ubeta, sin_val, cos_val);
		float duty[3];
		svm_set(ualpha, ubeta, duty);
		inverter_set_3phase_voltages(inverer, duty[0], duty[1], duty[2]);
	} break;
	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}
