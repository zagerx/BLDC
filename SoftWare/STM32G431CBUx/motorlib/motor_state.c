#include <stddef.h>
#include "foc_data.h"
#include "motor.h"
#include <statemachine.h>
#include <stdint.h>
#include "motor_cfg.h"
#include "motor_carible.h"
#include "coord_transform.h"
#include "currsmp.h"
#include "feedback.h"
#include "inverter.h"
#include "svpwm.h"
#include "foc_pid.h"
#include "t_trajectory.h"
#include "foc.h"
static fsm_rt_t motor_encoder_openloop_state(fsm_cb_t *obj);
static fsm_rt_t motor_running_state(fsm_cb_t *obj);

fsm_rt_t motor_idle_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};

	switch (obj->phase) {
	case ENTER:
		obj->phase = RUNING;
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

	switch (obj->phase) {
	case ENTER:
		obj->phase = RUNING;
		break;

	case RUNING: {
		if (++obj->count > 10000) {
			obj->count = 0;
			TRAN_STATE(obj, motor_running_state);
		}
	} break;

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

	switch (obj->phase) {
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

	switch (obj->phase) {
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

	switch (obj->phase) {
	case ENTER:
		obj->phase = RUNNING;
		break;
	case RUNNING:
		if (calibration_modules_update(&m_data->calib, PWM_CYCLE) == 1) {
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

static fsm_rt_t motor_encoder_openloop_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
		IDLE,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	struct motor_config *m_cfg = motor->config;
	struct foc_data *f_data = &(m_data->foc_data);
	struct inverter_t *inverter = m_cfg->inverter;

	switch (obj->phase) {
	case ENTER:
		obj->count = 0;
		obj->phase = IDLE;
		break;
	case IDLE: {
	}; break;
	case RUNING: {
		float ud, uq;
		ud = 0.0f;
		uq = -0.4f;
		openloop_voltage_apply_dq(inverter, ud, uq, f_data->meas.eangle, 24.0f);
	} break;
	case EXIT:
		inverter_set_3phase_voltages(inverter, 0.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}

	return 0;
}

static fsm_rt_t motor_running_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
		WAIT,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	struct motor_config *m_cfg = motor->config;
	struct inverter_t *inverter = m_cfg->inverter;
	struct foc_data *f_data = &(m_data->foc_data);
	struct device *scp = f_data->scp;
	switch (obj->phase) {
	case ENTER:
		s_planner_init(scp, f_data->meas.position / 1000.0f, 0.0f, 0.0f, 0.0f);
		foc_currentloop_init(f_data);
		foc_velocityloop_init(f_data);
		foc_posloop_init(f_data);
		obj->count = 0;
		obj->phase = RUNING;
		break;
	case RUNING: {

		float pos, vel;
		if (((obj->count) % POSITION_LOOP_DIVIDER == 0)) {
			s_planner_action(scp, 0.01f);
			pos = s_planner_get_pos(scp) * 1000.0f;
			vel = s_planner_get_vel(scp) * 1000.0f;
			foc_posloop(f_data, pos, POS_LOOP_CYCLE);
			f_data->ref.velocity += vel;
		}
		if (((obj->count) % VELOCITY_LOOP_DIVIDER == 0)) {
			foc_velocity_loop(f_data, f_data->ref.velocity, SPEED_LOOP_CYCLE);
		}
		obj->count++;

		float ud_final, uq_final;
		foc_currentloop(f_data, &ud_final, &uq_final, PWM_CYCLE);

		float ualpha, ubeta;
		foc_modulate_dq(f_data->meas.eangle, f_data->meas.vbus, ud_final, uq_final, &ualpha,
				&ubeta);
		float duty[3];
		svpwm_seven_segment(ualpha, ubeta, &duty[0], &duty[1], &duty[2]);
		inverter_set_3phase_voltages(inverter, duty[0], duty[1], duty[2]);
	} break;

	case EXIT:
		foc_currentloop_deinit(f_data);
		foc_velocityloop_deinit(f_data);
		foc_posloop_deinit(f_data);
		inverter_set_3phase_voltages(inverter, 0.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}

	return 0;
}
