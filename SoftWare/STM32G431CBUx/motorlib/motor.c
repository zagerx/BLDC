#include "motor.h"
#include "coord_transform.h"
#include "currsmp.h"
#include "device.h"
#include "feedback.h"
#include "inverter.h"
#include "svpwm.h"

#include "motor_mode.h"
#include "statemachine.h"

#include "motor_cfg.h"
#include "motor_pp_ident.h"
#include <stdint.h>

#include "motor_currment_carible.h"
extern struct device motor1;

void foc_curr_regulator(uint32_t *adc_raw)
{
	// struct device *motor = (struct device *)ctx;
	struct device *motor = &motor1;
	struct motor_config *m_cfg = motor->config;
	struct device *feedback = m_cfg->feedback;
	struct device *currsmp = m_cfg->currsmp;
	struct device *inverer = m_cfg->inverter;
	curr_calib_update(motor, adc_raw);
	// pp_ident_update(motor, PWM_CYCLE);
	// struct currsmp_data *c_data = currsmp->data;
	// c_data->channle_raw_a = adc_raw[0];
	// c_data->channle_raw_c = adc_raw[2];
	// currsmp_update_calibration(currsmp);
	// feedback_update_angle_vec(feedback);
	// currsmp_updata(currsmp);

	// static float self_theta = 360.0f;
	// self_theta -= 0.002f;
	// if (self_theta < 0.0f) {
	//   self_theta = 360.0f;
	// }

	// float sin_val, cos_val;
	// sin_cos_f32(self_theta, &sin_val, &cos_val);

	// float ud, uq;
	// float ualpha, ubeta;
	// ud = 0.0f;
	// uq = 0.02f;
	// inv_park_f32(ud, uq, &ualpha, &ubeta, sin_val, cos_val);
	// float duty[3];
	// svm_set(ualpha, ubeta, duty);
	// inverter_set_3phase_voltages(inverer, duty[0], duty[1], duty[2]);
}
void motor_init(struct device *motor)
{
	struct motor_data *m_data = motor->data;
	fsm_cb_t *fsm = m_data->fsm_mode;
	if (!fsm->p1) {
		fsm->p1 = (void *)motor;
	}
	// struct motor_config *m_cfg = motor->config;
	// struct device *pp = m_cfg->pp_ident;
	pp_ident_start(motor);
	curr_calib_start(motor, 10000);
}
void motor_task(struct device *motor)
{
	struct motor_data *m_data = motor->data;
	if (m_data->flag == PARAM_NEVER_LOADED) {
		TRAN_STATE(m_data->fsm_mode, motor_calibration_mode);
	} else {
	}

	for (;;) {
		DISPATCH_FSM(m_data->fsm_mode);
	}
}
