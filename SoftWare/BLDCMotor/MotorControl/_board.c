
// #include "MyProject.h"


#include "stm32f4xx.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "utils.h"
#include "arm_cos_f32.h"
#include "board.h"
#include "sensor.h"
#include "motorctrl_common.h"
#include "tim.h"



 float  vbus_voltage=24.0f;

// abc_t
static float  Ialpha_beta[2];
static alpbet_t  motor_Ialphabeta;
// static dq_t  motor_Idq;

static float  pi_gains_[2];
static float  v_current_control_integral_d = 0.0f; // [V]
static float  v_current_control_integral_q = 0.0f; // [V]
static float effective_current_lim_ = 10.0f; // [A]
static float max_allowed_current_ = 0.0f;    // [A] set in setup()
 mt_param_t sgmc_param = {0};


float input_torque_ = 0.05f;  // [Nm]

void arm(void);

#define enable_torque_mode_vel_limit     (1)
           //速度I参数
#define VBUS_S_DIVIDER_RATIO             18.73f    //电源分压电阻2.2k+39k




static void foc_reset(void)
{
	v_current_control_integral_d = 0.0f;  //积分清零，否则闭环时电机误动作
	v_current_control_integral_q = 0.0f;
	pid_reset(&(sgmc_param.daxis_pi));
	pid_reset(&(sgmc_param.qaxis_pi));
}

 void motor_para_init(void)
{
	pi_gains_[0] = 0.0273f;
	pi_gains_[1] = 166.2507f;
	effective_current_lim_ = 30.0f;
	max_allowed_current_ = 60.75f;
	
    pid_init(&(sgmc_param.daxis_pi),0.0273f,166.2507f * 0.000125f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(sgmc_param.qaxis_pi),0.0273f,166.2507f * 0.000125f,1.0f,D_MAX_VAL,D_MIN_VAL);
}

 void arm(void)
{
	foc_reset();

}





static void vbus_sense_adc_cb(uint32_t adc_value) 
{
	float voltage_scale = 3.3f * VBUS_S_DIVIDER_RATIO / 4096;
	vbus_voltage = adc_value * voltage_scale;
	vbus_voltage = 24.0F;
}













void __loopcurrment(uint16_t a,uint16_t b,uint16_t c)
{
	((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_COV))[0];

	dq_t idq;
	abc_t  motor_Iabc;
	// _convert_current(&motor_Iabc,0,b,c);
	// FOC_current(motordebug.id_targe, motordebug.iq_targe,motor_Iabc);  //highcurrent电机用电流模式	
}




