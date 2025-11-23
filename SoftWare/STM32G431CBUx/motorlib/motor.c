#include "motor.h"
#include "coord_transform.h"
#include "currsmp.h"
#include "device.h"
#include "feedback.h"
#include "foc_parameters.h"
#include "inverter.h"
#include "svpwm.h"

#include "motor_mode.h"
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
	// if (m_data->flag == PARAM_NEVER_LOADED) {

	// } else {
	// }
	static int16_t state = 0;
	switch (state) {
	case 0:
		TRAN_STATE(m_data->state_machine, motor_carible_state);
		state = 1;
		break;
	case 1:
		// if (motor_get_calibstate(m_data->calib) == M_ALL_CALIB_DONE) {
		// 	TRAN_STATE(m_data->state_machine, motor_encoder_openloop_state);
		// 	state = 2;
		// }
		break;
	}
}
void debug_update_foc_data(float debug_data)
{
	struct device *motor = &motor1;
	struct motor_data *m_data = motor->data;
	struct foc_parameters *foc_param = &m_data->foc_data;
	debug_update_iq_ref(foc_param, debug_data);
}