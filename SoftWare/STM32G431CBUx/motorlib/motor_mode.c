
#include <stddef.h>
#include "motor.h"
#include <statemachine.h>

fsm_rt_t motor_init_state(fsm_cb_t *obj);

/**
 * @brief Open loop control mode state machine
 * @param obj State machine control block
 * @return fsm_rt_cpl when complete
 *
 * States:
 * 1. ENTER: Initialize open loop mode
 * 2. IDLE: Main operational state
 * 3. EXIT: Cleanup when exiting mode
 */
fsm_rt_t motor_torque_control_mode(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	switch (obj->chState) {
	case ENTER:
		m_data->mode = MOTOR_MODE_TORQUE;
		statemachine_init(obj->sub_state_machine, NULL, motor_init_state, obj->p1, NULL, 0);
		obj->chState = RUNING;
		break;
	case RUNING:
		if (obj->sub_state_machine) {
			DISPATCH_FSM(obj->sub_state_machine);
		}
		break;
	case EXIT:
		break;

	default:
		break;
	}
	return fsm_rt_cpl;
}
fsm_rt_t motor_idle_control_mode(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	switch (obj->chState) {
	case ENTER:
		// motor_start(obj->p1);
		statemachine_init(obj->sub_state_machine, NULL, motor_init_state, obj->p1, NULL, 0);
		m_data->mode = MOTOR_MODE_IDLE;
		obj->chState = RUNING;
		break;
	case RUNING:
		if (obj->sub_state_machine) {
			DISPATCH_FSM(obj->sub_state_machine);
		}
		break;
	case EXIT:
		break;

	default:
		break;
	}
	return fsm_rt_cpl;
}

/**
 * @brief Speed control mode state machine
 * @param obj State machine control block
 * @return fsm_rt_cpl when complete
 *
 * States:
 * 1. ENTER: Initialize speed control
 * 2. IDLE: Main operational state
 * 3. EXIT: Cleanup when exiting mode
 */
fsm_rt_t motor_init_state(fsm_cb_t *obj);

fsm_rt_t motor_speed_control_mode(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;

	struct motor_data *m_data = motor->data;
	switch (obj->chState) {
	case ENTER:
		m_data->mode = MOTOR_MODE_SPEED;
		statemachine_init(obj->sub_state_machine, NULL, motor_init_state, obj->p1, NULL, 0);
		obj->chState = RUNING;
		break;
	case RUNING:

		if (obj->sub_state_machine) {
			DISPATCH_FSM(obj->sub_state_machine);
		}
		break;
	case EXIT:
		break;

	default:
		break;
	}
	return fsm_rt_cpl;
}

/**
 * @brief Position control mode (stub)
 * @param obj State machine control block
 * @return fsm_rt_cpl when complete
 *
 * TODO: Implement position control logic
 */
fsm_rt_t motor_position_control_mode(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	switch (obj->chState) {
	case ENTER:
		m_data->mode = MOTOR_MODE_POSI;
		statemachine_init(obj->sub_state_machine, NULL, motor_init_state, obj->p1, NULL, 0);
		obj->chState = RUNING;
		break;
	case RUNING:
		if (obj->sub_state_machine) {
			DISPATCH_FSM(obj->sub_state_machine);
		}
		break;
	case EXIT:
		break;

	default:
		break;
	}
	return fsm_rt_cpl;
}

fsm_rt_t motor_calibration_mode(fsm_cb_t *obj)
{
	enum {
		CALIBRATE_CURRMENT = USER_STATUS,
		CALIBRATE_RL,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	switch (obj->chState) {
	case ENTER:
		break;
	case CALIBRATE_CURRMENT:
		{
			
		}
		break;

	case CALIBRATE_RL:
		{
			
		}
		break;

	case EXIT:
		break;

	default:
		break;
	}
	return fsm_rt_cpl;
}

