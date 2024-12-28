#include "board.h"
#include "initmodule.h"
#include "taskmodule.h"
#include "debuglog.h"
#include "stm32g4xx_hal.h"

#include "adc.h"
#include "opamp.h"
void Board_init(void)
{
    // adc_start();
    // opamp_start();
}
#include "voft.h"

#include "tim.h"
static void motor_enable(void)
{
    tim_pwm_enable();
    tim_tigger_adc();
    adc_start();
    opamp_start();
    tim_hallmode_enable();

}
static void motor_disable(void)
{
    tim_hallmode_disable();
    tim_pwm_disable();
    adc_stop();
}
static void motor_set_pwm(float _a,float _b,float _c)
{
    tim_set_pwm(_a ,_b,_c);
}
uint32_t test_cout = 0;
uint32_t test_c1,test_c2,test_c3;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        test_cout++;
        test_c1 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
    }
}

static void user_softresetsystem(void)
{
	HAL_NVIC_SystemReset();
}
#include "flash.h"
void motor_write(void *pdata,uint16_t datalen)
{
    user_flash_earse(PID_PARSE_ADDR,datalen);
    user_flash_write(PID_PARSE_ADDR,(uint8_t *)pdata,datalen);
}
void motor_read(void *pdata,uint16_t datalen)
{
    user_flash_read(PID_PARSE_ADDR,(uint8_t *)pdata,datalen);
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
#include "fsm.h"
static fsm_cb_t Motor1Fsm;
motor_t motor1 = {0};
static hall_sensor_t hall_sensor;

void  HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  static float iabc[3];
  if (hadc->Instance == ADC1)
  {
    iabc[0] = -((int16_t)(hadc->Instance->JDR1) -1970)*0.02197f;
    iabc[2] = -((int16_t)(hadc->Instance->JDR2) - 1980)*0.02197f;
    mc_hightfreq_task(iabc,&motor1);
    // votf_sendf(iabc,3);
  }
  if (hadc->Instance == ADC2)
  {
    iabc[1] = -((int16_t)((&hadc2)->Instance->JDR1 - 2040))*0.02197f;
  }
}

static uint8_t hall_get_sectionnumb(void)
{
    uint8_t u,v,w;
    u = HAL_GPIO_ReadPin(HALL_U1_GPIO_Port,HALL_U1_Pin);
    v = HAL_GPIO_ReadPin(HALL_V1_GPIO_Port,HALL_V1_Pin);
    w = HAL_GPIO_ReadPin(HALL_W1_GPIO_Port,HALL_W1_Pin);
    return u | (w<<1) | (v<<2);
}
static uint32_t hall_gettick()
{
    return 0;
}

/**
 * 电机初始化
 */
void motorctrl_init(void)
{

    /*HALL_ABZ传感器初始化  和电机模块无关*/
    hall_register((void*)&(hall_sensor),\
                                        hall_get_sectionnumb,\
                                        hall_gettick,\
                                        hall_cale,\
                                        hall_update,\
                                        hall_init,\
                                        hall_deinit,\
                                        hall_get_initpos,\
                                        hall_set_calib_points);
    //初始化电机控制状态机
    motorfsm_register(&Motor1Fsm,&motor1);
    //注册编码器接口
    motor_encoder_register(Motor1Fsm.pdata,                      \
                                     &(hall_sensor));
    //注册电机动作回调
    motor_actor_register(Motor1Fsm.pdata,                         \
                              motor_enable,                       \
                              motor_disable,                      \
                              motor_set_pwm,                      \
                              user_softresetsystem,               \
                              motor_write,                        \
                              motor_read);
}

void motorctrl_task(void)
{
    motortctrl_process(&Motor1Fsm);
}



void baord_process(void)
{
    float a;
    // a = adc_getval()*(3.3f/4096);
}

board_task(baord_process)