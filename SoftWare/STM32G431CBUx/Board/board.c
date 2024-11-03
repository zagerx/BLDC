#include "board.h"
#include "initmodule.h"
#include "taskmodule.h"
#include "debuglog.h"
#include "stm32g4xx_hal.h"

#include "adc.h"
#include "opamp.h"
void Board_init(void)
{
    adc_start();
    opamp_start();
}
#include "voft.h"

#include "tim.h"
void motor_enable(void)
{
    tim_pwm_enable();
    tim_tigger_adc();
    adc_start();
}
void motor_disable(void)
{
    tim_pwm_disable();
    adc_stop();
}
void motor_set_pwm(float _a,float _b,float _c)
{
    tim_set_pwm(_a ,_b,_c);
}

#include "hall_sensor.h"
#include "motorctrl_common.h"
extern mc_param_t mc_param;

void mc_hallencoder_init(void)
{
    mc_param.encoder_handle.init = hall_init;
    mc_param.encoder_handle.update = hall_update;
    mc_param.encoder_handle.cacle = hall_cale;
}




void baord_process(void)
{
    float a;
    a = adc_getval()*(3.3f/4096);
}

board_task(baord_process)