#include <stddef.h>
#include "motor.h"
#include <statemachine.h>

fsm_rt_t motor_init_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;

	switch (obj->chState) {
	case ENTER:
		m_data->statue = MOTOR_STATE_INIT;
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
fsm_rt_t motor_aligh_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	// const struct device *svpwm = ((const struct motor_config *)motor->config)->pwm;
	// const struct device *fb = ((const struct motor_config *)motor->config)->feedback;
	switch (obj->chState) {
	case ENTER:
		m_data->statue = MOTOR_STATE_ALIGN;
		obj->chState = RUNING;
		// svpwm_enable_threephase_channle(svpwm);
		break;
	case RUNING:
		// feedback_calibration(fb);
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
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	// const struct device *svpwm = ((const struct motor_config *)motor->config)->pwm;
	// const struct device *foc = ((const struct motor_data *)motor->data)->foc_dev;

	switch (obj->chState) {
	case ENTER:
		m_data->statue = MOTOR_STATE_READY;
		if (m_data->mode == MOTOR_MODE_POSI) {
			// posloop_init(foc);
		} else if (m_data->mode == MOTOR_MODE_SPEED) {
			// speedloop_init(foc);
		} else if (m_data->mode == MOTOR_MODE_TORQUE) {
			// foc_currloop_init(foc);
		}
		// svpwm_enable_threephase_channle(svpwm);
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
	const struct device *motor = obj->p1;
	// const struct device *foc = ((const struct motor_data *)motor->data)->foc_dev;
	struct motor_data *m_data = motor->data;

	switch (obj->chState) {
	case ENTER:
		m_data->statue = MOTOR_STATE_CLOSED_LOOP;
	case RUNING:
		if (m_data->mode == MOTOR_MODE_POSI) {
			// posloop(foc);
		} else if (m_data->mode == MOTOR_MODE_SPEED) {
			// speedloop(foc);
		} else if (m_data->mode == MOTOR_MODE_TORQUE) {
			// foc_currloop(foc);
		}

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
	// const struct device *foc = ((const struct motor_data *)motor->data)->foc_dev;
	// const struct device *svpwm = ((const struct motor_config *)motor->config)->pwm;
	struct motor_data *m_data = motor->data;

	switch (obj->chState) {
	case ENTER:
		m_data->statue = MOTOR_STATE_STOP;
		// posloop_deinit(foc);
		// svpwm_disable_threephase_channle(svpwm);
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
	// const struct device *foc = ((const struct motor_data *)motor->data)->foc_dev;
	// const struct device *svpwm = ((const struct motor_config *)motor->config)->pwm;
	struct motor_data *m_data = motor->data;

	switch (obj->chState) {
	case ENTER:
		m_data->statue = MOTOR_STATE_FAULT;
		// posloop_deinit(foc);
		// svpwm_disable_threephase_channle(svpwm);
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

void motor_encoder_carible(struct device *motor)
{
		
}

