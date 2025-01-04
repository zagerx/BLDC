/*==========================================================================================
 * @file board.c
 * @author zager
 * @version      0.1
 * @date 2025-01-04
 * 
 * @copyright Copyright (c) 2025
 * 
 * @brief 
--------------------------------------------------------------------------------------------*/

#include "board.h"
#include "initmodule.h"
#include "debuglog.h"
#include "taskmodule.h"
#include "usart.h"
#include "string.h"
#include "tim.h"
#include "motorctrl.h"
#include "adc.h"
#include "fsm.h"
#include "abs_sensor.h"
#include "as5047.h"

#undef NULL
#define NULL 0

void user_softresetsystem(void)
{
	HAL_NVIC_SystemReset();
}
void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
    static unsigned char sg_uartsend_buf[125];
    memcpy(sg_uartsend_buf,pdata,len);
    HAL_UART_Transmit_DMA(&huart1,sg_uartsend_buf,len);
}
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
void motor_write(void *pdata,uint16_t datalen)
{
    // user_flash_earse(PID_PARSE_ADDR,datalen);
    // user_flash_write(PID_PARSE_ADDR,(uint8_t *)pdata,datalen);
}
void motor_read(void *pdata,uint16_t datalen)
{
    // user_flash_read(PID_PARSE_ADDR,(uint8_t *)pdata,datalen);
}




#include "motorctrl_common.h"
static fsm_cb_t Motor1Fsm;
motor_t motor1 = {0};
static abs_sensor_t abs_sensor;

/*==========================================================================================
 * @brief 
 * @FuncName     motorctrl_init
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void motorctrl_init(void)
{
    /*ABS传感器初始化  和电机模块无关*/
    abs_sensor_register((void*)&(abs_sensor),\
                                        as5047_get_data,\
                                        abs_sensor_cacle,\
                                        abs_sensor_update,\
                                        abs_sensor_init,\
                                        abs_sensor_deinit,\
                                        abs_sensor_get_firstpoint,\
                                        abs_sensor_set_cairbpoint);

    //初始化电机控制状态机
    motorfsm_register(&Motor1Fsm,&motor1);
    //注册编码器接口
    motor_encoder_register(Motor1Fsm.pdata,                      \
                                     &(abs_sensor));
    //注册电机动作回调
    motor_actor_register(Motor1Fsm.pdata,                         \
                              motor_enable,                       \
                              motor_disable,                      \
                              motor_set_pwm,                      \
                              user_softresetsystem,               \
                              motor_write,                        \
                              motor_read);
}
/*==========================================================================================
 * @brief 
 * @FuncName     
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void motorctrl_task(void)
{
    motortctrl_process(&Motor1Fsm);
}
/*==========================================================================================
 * @brief 
 * @FuncName     _convert_current
 * @param        adc_buf 
 * @param        i_abc 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
static void _convert_current(uint16_t* adc_buf,float *i_abc)
{
    i_abc[0]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[0] - (1 << 11)) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE); 
    i_abc[1]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[1] - (1 << 11)) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE);          //shunt_conductance_ = 1/0.001采样电阻;
    i_abc[2]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[2] - (1 << 11)) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE);
}
/*==========================================================================================
 * @brief        电流更新入口函数
 * @FuncName     HAL_ADCEx_InjectedConvCpltCallback
 * @param        hadc 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{    
    unsigned short adc_vale[3];
    float iabc[3]; 
    {
        adc_vale[1] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1);
        adc_vale[2] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2);
        adc_vale[0] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_3);
        _convert_current((uint16_t*)adc_vale,iabc);

        iabc[0] = -iabc[0];
        iabc[1] = -iabc[1];
        iabc[2] = -iabc[2];
        // __cycleof__("mc_hightfreq_task") {
        motorctrl_currment_update(&motor1,iabc);
        // }            
    }
}

/*==========================================================================================
 * @brief 
 * @FuncName     user_board_init
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void user_board_init(void)
{
    motorctrl_init();
}
void board_deinit(void)
{

}

board_task(motorctrl_task)
