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
static void motor_enable(void)
{
    tim_pwm_enable();
    tim_tigger_adc();
    adc_start();
}
static void motor_disable(void)
{
    tim_pwm_disable();
    adc_stop();
}
static void motor_set_pwm(float _a,float _b,float _c)
{
    tim_set_pwm(_a ,_b,_c);
}


static void user_softresetsystem(void)
{
	HAL_NVIC_SystemReset();
}

#include "usart.h"
#include "string.h"
void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
    static unsigned char sg_uartsend_buf[125];
    memcpy(sg_uartsend_buf,pdata,len);
    HAL_UART_Transmit_DMA(&huart3,sg_uartsend_buf,len);
}


#include "hall_sensor.h"
#include "motorctrl_common.h"
#include "voft.h"
#include "motorctrl.h"
extern motor_t motor1;
void  HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  static float iabc[3];
  if (hadc->Instance == ADC1)
  {
    iabc[0] = -((int16_t)(hadc->Instance->JDR1 - 2030-12))*0.02197f;
    iabc[1] = ((int16_t)(hadc->Instance->JDR2 - 2048+8))*0.02197f;
    mc_hightfreq_task(iabc,&motor1);
    // votf_sendf(iabc,3);
  }
  if (hadc->Instance == ADC2)
  {
    iabc[2] = ((int16_t)((&hadc2)->Instance->JDR1 - 2048 + 12))*0.02197f;
  }
}

void motor_func_register(motor_t *motor)
{
    motor->encoder_handle.init = hall_init;
    motor->encoder_handle.update = hall_update;
    motor->encoder_handle.cacle = hall_cale;

    motor->enable = motor_enable;
    motor->disable = motor_disable;
    motor->setpwm = motor_set_pwm;
    motor->reset_system = user_softresetsystem;
    // motor->bsptransmit = _bsp_protransmit;//TODO
}




void baord_process(void)
{
    float a;
    a = adc_getval()*(3.3f/4096);
}

board_task(baord_process)