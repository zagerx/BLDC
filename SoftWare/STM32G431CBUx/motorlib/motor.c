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
#include "motor_state.h"
extern struct device motor1;

void foc_curr_regulator(uint32_t *adc_raw)
{
	// struct device *motor = (struct device *)ctx;
	struct device *motor = &motor1;
	struct motor_data *m_data = motor->data;
	struct motor_config *m_cfg = motor->config;
	struct device *feedback = m_cfg->feedback;
	struct device *currsmp = m_cfg->currsmp;
	struct device *inverer = m_cfg->inverter;
	currsmp_update_raw(currsmp,adc_raw);

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
	fsm_cb_t *fsm = m_data->state_machine;
	if (!fsm->p1) {
		fsm->p1 = (void *)motor;
	}
}

void motor_task(struct device *motor)
{
	struct motor_data *m_data = motor->data;
	if (m_data->flag == PARAM_NEVER_LOADED) {
		TRAN_STATE(m_data->state_machine, motor_carible_state);
	} else {
	}
}
