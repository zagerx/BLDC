#include "motor.h"
#include "coord_transform.h"
#include "currsmp.h"
#include "device.h"
#include "feedback.h"
#include "foc_parameters.h"
#include "inverter.h"
#include "svpwm.h"

#include "statemachine.h"

#include "motor_cfg.h"
#include "_pp_ident.h"
#include <stdint.h>
#include "motor_state.h"
#include "motor_carible.h"
extern struct device motor1;

void foc_curr_regulator(uint32_t *adc_raw)
{
	// struct device *motor = (struct device *)ctx;
	struct device *motor = &motor1;
	struct motor_data *m_data = motor->data;
	struct motor_config *m_cfg = motor->config;
	// struct device *feedback = m_cfg->feedback;
	struct device *currsmp = m_cfg->currsmp;
	// struct device *inverer = m_cfg->inverter;
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
	fsm_cb_t *fsm = m_data->state_machine;
	if (!fsm->p1) {
		fsm->p1 = (void *)motor;
	}

	struct motor_config *m_cfg = motor->config;
	struct device *currsmp = m_cfg->currsmp;
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

void debug_update_foc_data(float *input, enum foc_parameters_index flag)
{
	struct device *motor = &motor1;
	struct motor_data *m_data = motor->data;
	struct foc_parameters *foc_param = &m_data->foc_data;

	if (flag == INDEX_ID_REF) // 目标参数更新
	{
		write_foc_param_(foc_param, INDEX_ID_REF, input);
	} else if (flag == INDEX_D_PI) { // PI参数更新
		write_foc_param_(foc_param, INDEX_D_PI, input);
		TRAN_STATE(m_data->state_machine, motor_debug_idle_state);
	} else if (flag == INDEX_VELOCITY_PI) {
		write_foc_param_(foc_param, INDEX_VELOCITY_PI, input);
		TRAN_STATE(m_data->state_machine, motor_debug_idle_state);
	} else if (flag == INDEX_VELOCITY_REG) {
		write_foc_param_(foc_param, INDEX_VELOCITY_REG, input);
		s_planner_update_target(m_data->scp, input[0]);
	}
}
