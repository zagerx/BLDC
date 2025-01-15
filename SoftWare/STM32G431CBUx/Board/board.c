#include "board.h"
#include "initmodule.h"
#include "taskmodule.h"
#include "debuglog.h"
#include "stm32g4xx_hal.h"
#include "adc.h"
#include "opamp.h"
#include "tim.h"
#include "usart.h"
#include "string.h"
#include "hall_sensor.h"
#include "motorctrl.h"
#include "fsm.h"
#include "mc_commend.h"
#include "motorctrl_common.h"
#include "protocol_cmdmap.h"
#include "flash.h"//TODO
static fsm_cb_t Motor1Fsm;
static motor_t motor1 = {0};
static hall_sensor_t hall_sensor;

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

static void user_softresetsystem(void)
{
	
}
void sys_softreset(void *obj,uint8_t* pdata,uint16_t datalen)
{
    HAL_NVIC_SystemReset();
}
void motor_write(void *pdata,uint16_t datalen)
{
    user_flash_earse(PID_PARSE_ADDR,datalen);
    user_flash_write(PID_PARSE_ADDR,(uint8_t *)pdata,datalen);
}
void motor_read(void *pdata,uint16_t datalen)
{
    user_flash_read(PID_PARSE_ADDR,(uint8_t *)pdata,datalen);
}
static uint8_t get_section_numb(void)
{
    uint8_t u,v,w;
    u = HAL_GPIO_ReadPin(HALL_U1_GPIO_Port,HALL_U1_Pin);
    v = HAL_GPIO_ReadPin(HALL_V1_GPIO_Port,HALL_V1_Pin);
    w = HAL_GPIO_ReadPin(HALL_W1_GPIO_Port,HALL_W1_Pin);
    return u | (w<<1) | (v<<2);
}
static uint32_t get_tick()
{
    return 0;
}
void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
    static unsigned char sg_uartsend_buf[125];
    memcpy(sg_uartsend_buf,pdata,len);
    HAL_UART_Transmit_DMA(&huart3,sg_uartsend_buf,len);
}



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
        // motorctrl_encoder_update(Motor1Fsm.pdata);
    }
}


void  HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  static float iabc[3];
  if (hadc->Instance == ADC1)
  {
    iabc[0] = -((int16_t)(hadc->Instance->JDR1) -1970)*0.02197f;
    iabc[2] = -((int16_t)(hadc->Instance->JDR2) - 1980)*0.02197f;
    motorctrl_currment_update(&motor1,iabc);
    // votf_sendf(iabc,3);
  }
  if (hadc->Instance == ADC2)
  {
    iabc[1] = -((int16_t)((&hadc2)->Instance->JDR1 - 2040))*0.02197f;
  }
}



void motorctrl_init(void)
{

    /*HALL_ABZ传感器初始化  和电机模块无关*/
    hall_register((void*)&(hall_sensor),\
                                        get_section_numb,\
                                        get_tick,\
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
void Board_init(void)
{
    protocol_cmd_register(M_SET_START,              motor_get_motorstart          ,&motor1);
    protocol_cmd_register(M_SET_STOP,               motor_get_motorstop           ,&motor1);
    protocol_cmd_register(M_SET_NormalM,            motor_get_normolmode          ,&motor1);
    protocol_cmd_register(M_SET_SpeedM,             motor_get_speedmode           ,&motor1);
    protocol_cmd_register(M_SET_EncoderLoopM,       motor_get_encodermode         ,&motor1);
    protocol_cmd_register(M_SET_PIDTarge,           motot_get_pidtarge            ,&motor1);
    protocol_cmd_register(M_SET_PIDParam,           sys_softreset                 ,NULL);

    motorctrl_init();
}

void motorctrl_task(void)
{
    motortctrl_process(&Motor1Fsm);
}

void baord_process(void)
{
}

board_task(motorctrl_task)
