#include "board.h"
#include "initmodule.h"
#include "taskmodule.h"
#include "debuglog.h"
#include "ina226.h"
#include "tac9539pwr.h"
#include "i2c_device.h"
#include "i2c_bus.h"
#include "i2c.h"
#include "mc_commend.h"
#include "usart.h"
#include "string.h"
#include "tim.h"
#include "motorctrl.h"
#include "hall_sensor.h"
#include "motorctrl_common.h"
#include "adc.h"
#include "flash.h"
#include "gpio.h"
#include "tim.h"
#include "stdarg.h"
#include "fsm.h"
#include "protocol_cmdmap.h"
/*************************************************************************************************
                                模块句柄                                                           
*************************************************************************************************/
static fsm_cb_t Motor1Fsm;
static motor_t motor1 = {0};
static hall_sensor_t hall_sensor;
static __attribute__((section(".D2_Area"))) uint8_t sg_uartreceive_buff[125];
static i2c_bus_t i2c_bus1 = {
    .init = i2c_bus1_init,
    .read = i2c_bus1_read,
    .write = i2c_bus1_write,
};
static i2c_device_t ina226 = {
    .bus = &i2c_bus1,
    .init = dev_ina226_init,
};
static i2c_device_t dev_tca9538 = {
    .bus = &i2c_bus1,
    .init = tca9539pwr_init,
};
/*************************************************************************************************
                                本地函数                                                           
*************************************************************************************************/
static void motor_enable(void);
static void motor_disable(void);
static void motor_set_pwm(float _a,float _b,float _c);
static void sys_softreset(void *obj,uint8_t* pdata,uint16_t datalen);
static uint8_t get_section_numb(void);
static uint32_t get_tick();
static void _convert_current(uint16_t* adc_buf,float *i_abc);
/*************************************************************************************************
                                中断接口                                                           
*************************************************************************************************/
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
  	uint32_t tmp;
    if(USART1 == huart1.Instance)                                   
    {
        if(RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))   
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart1);                     
            HAL_UART_DMAStop(&huart1);
            unsigned short data_length  = sizeof(sg_uartreceive_buff) - __HAL_DMA_GET_COUNTER(huart1.hdmarx);
            // USER_DEBUG_NORMAL("datalen %d %d\n",data_length,__HAL_DMA_GET_COUNTER(&hdma_usart1_rx));
            protocol_getdata_tofifo(sg_uartreceive_buff,data_length);
            memset(sg_uartreceive_buff,0,data_length);
            data_length = 0;
            HAL_UART_Receive_DMA(&huart1, (uint8_t*)sg_uartreceive_buff, sizeof(sg_uartreceive_buff));
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
    motorctrl_encoder_update(Motor1Fsm.pdata);
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{    
    unsigned short adc_vale[3];
    float iabc[3]; 
    {
        adc_vale[0] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_1);
        adc_vale[1] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_2);
        adc_vale[2] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_3);
        _convert_current((uint16_t*)adc_vale,iabc);
        // __cycleof__("mc_hightfreq_task") {
            motorctrl_currment_update(&motor1,iabc);
        // }
    }
}

/*************************************************************************************************
                                模块初始化                                                           
*************************************************************************************************/
void motorctrl_init(void)
{
    //初始化电机控制状态机
    motorfsm_register(&Motor1Fsm,&motor1);
    //注册编码器接口
    motor_encoder_register(Motor1Fsm.pdata,                      \
                                     &(hall_sensor));
    //注册电机动作回调
    motor_actor_register(Motor1Fsm.pdata,                         \
                              motor_enable,                       \
                              motor_disable,                      \
                              motor_set_pwm                       \
                              );
    //读取电机相关参数 TODO
}

void user_board_init(void)
{
    protocol_cmd_register(M_SET_START,              motor_get_motorstart          ,&motor1);
    protocol_cmd_register(M_SET_STOP,               motor_get_motorstop           ,&motor1);
    protocol_cmd_register(M_SET_NormalM,            motor_get_normolmode          ,&motor1);
    protocol_cmd_register(M_SET_SpeedM,             motor_get_speedmode           ,&motor1);
    protocol_cmd_register(M_SET_EncoderLoopM,       motor_get_encodermode         ,&motor1);
    protocol_cmd_register(M_SET_PIDTarge,           motot_get_pidtarge            ,&motor1);
    protocol_cmd_register(M_SET_PIDParam,           sys_softreset                 ,NULL);

    i2c_bus1.init();
    // i2c_device_init(&ina226);
    // i2c_device_init(&dev_tca9538);
    USER_DEBUG_NORMAL("board_init finish\n");

    /*HALL_ABZ传感器初始化  和电机模块无关*/
    hall_register((void*)&(hall_sensor),\
                                        get_section_numb,\
                                        get_tick,\
                                        tim_abzencoder_getcount,\
                                        tim_abzencoder_setcount,\
                                        hall_cale,\
                                        hall_update,\
                                        hall_init,\
                                        hall_deinit,\
                                        hall_get_initpos,\
                                        hall_set_calib_points);
    motorctrl_init();
}

void board_deinit(void)
{

}
void baord_process(void)
{
    motortctrl_process(&Motor1Fsm);
}
static void _convert_current(uint16_t* adc_buf,float *i_abc)
{
    i_abc[0]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[0] - 2020) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE); 
    i_abc[1]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[1] - 2040) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE);          //shunt_conductance_ = 1/0.001采样电阻;
    i_abc[2]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[2] - 2060) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE);
}
static void motor_enable(void)
{
    HAL_GPIO_WritePin(EBAKE_PWM_EN_GPIO_Port,EBAKE_PWM_EN_Pin,GPIO_PIN_SET);
    gpio_setencoder_power();
    tim_abzencoder_enable();
    tim_pwm_enable();
    tim_tigger_adc();
    adc_start();

    tim1_pwm_enable();
}
static void motor_disable(void)
{
    tim_abzencoder_disable();
    tim_pwm_disable();
    adc_stop();
    // HAL_GPIO_WritePin(EBAKE_PWM_EN_GPIO_Port,EBAKE_PWM_EN_Pin,GPIO_PIN_RESET);

    tim1_pwm_disable();
}
static void motor_set_pwm(float _a,float _b,float _c)
{
    tim_set_pwm(_a ,_b,_c);
    tim1_set_pwm(_a ,_b,_c);
}
static uint8_t get_section_numb(void)
{
    uint8_t u,v,w;
    v = HAL_GPIO_ReadPin(HALL_U1_GPIO_Port,HALL_U1_Pin);
    u = HAL_GPIO_ReadPin(HALL_V1_GPIO_Port,HALL_V1_Pin);
    w = HAL_GPIO_ReadPin(HALL_W1_GPIO_Port,HALL_W1_Pin);
    return u | (w<<1) | (v<<2);
}
static uint32_t get_tick()
{
    return 0;
}
static void sys_softreset(void *obj,uint8_t* pdata,uint16_t datalen)
{
    HAL_NVIC_SystemReset();
}

board_task(baord_process)
