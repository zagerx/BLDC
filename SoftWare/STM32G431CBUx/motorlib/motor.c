#include "motor.h"
#include "coord_transform.h"
#include "currsmp.h"
#include "device.h"
#include "feedback.h"
#include "foc_data.h"
#include "inverter.h"
#include "svpwm.h"

#include "statemachine.h"

#include "motor_cfg.h"
#include <stdint.h>
#include "motor_state.h"
#include "motor_carible.h"
#include "openloop_voltage.h"
extern struct device motor1;

void foc_curr_regulator(uint32_t *adc_raw)
{
	struct device *motor = &motor1;
	struct motor_data *m_data = motor->data;
	struct motor_config *m_cfg = motor->config;
	struct device *currsmp = m_cfg->currsmp;
	currsmp_update_raw(currsmp, adc_raw);

	/*
		//电机状态调度
		0、校准功能
			1、电流偏置校准 2、极对数辨别，编码器校准(偏移量) 3、R/L参数识别
		1、编码器开环调试：
			1、运行编码器状态
		2、电流开环调试
			1、运行电流开环状态
		3、速度开环调试
			1、电流环闭环，不使用速度规划
		4、位置调试
			1、电流环闭环，速度闭环，不使用位置规划
		5、力矩模式
			1、电流闭环
		6、速度模式
			1、电流闭环 速度闭环 速度规划
		7、位置模式
			1、电流闭环 速度闭环 位置闭环 位置规划
	*/
	DISPATCH_FSM(m_data->state_machine);
}
#include "t_trajectory.h"
extern void test_solve_unconstrained_peak_velocity(void);

void motor_init(struct device *motor)
{
	struct motor_data *m_data = motor->data;
	struct motor_config *m_cfg = motor->config;
	struct device *currsmp = m_cfg->currsmp;
	struct device *feedback = m_cfg->feedback;
	struct device *inverer = m_cfg->inverter;
	/* 电流采样参数绑定 */
	struct currsmp_config *cs_cfg = currsmp->config;
	cs_cfg->params = &m_data->params.currsmp_params;

	/* 编码器参数绑定 */
	struct feedback_config *fb_cfg = feedback->config;
	fb_cfg->params = &m_data->params.feedback_params;

	/* FOC PID 参数绑定 */
	m_data->foc_data.controller.id.params = &m_data->params.d_pi_params;
	m_data->foc_data.controller.iq.params = &m_data->params.q_pi_params;
	m_data->foc_data.controller.velocity.params = &m_data->params.vel_pi_params;

	fsm_cb_t *fsm = m_data->state_machine;
	if (!fsm->p1) {
		fsm->p1 = (void *)motor;
	}

	struct calibration_modules *carlib = &m_data->calib;
	openloop_voltage_t *op = &m_data->op;
	struct motor_parameters *m_params = &m_data->params;
	calibration_modules_init(carlib, inverer, currsmp, feedback, op, m_params);
	currsmp_init(currsmp);
}

void motor_task(struct device *motor)
{
	struct motor_data *m_data = motor->data;
	static int16_t state = 0;
	switch (state) {
	case 0:
		TRAN_STATE(m_data->state_machine, motor_carible_state);
		state = 1;
		break;
	case 1:
		break;
	}
}

void update_motor_vel_pi_param(struct device *motor, float kp, float ki)
{
	struct motor_data *m_data = motor->data;
	struct motor_parameters *params = &m_data->params;
	params->vel_pi_params.kp = kp;
	params->vel_pi_params.ki = ki;
}
void update_motor_vel_target(struct device *motor, float tar)
{
	struct motor_data *m_data = motor->data;
	s_planner_update_target(m_data->scp, tar);
}
