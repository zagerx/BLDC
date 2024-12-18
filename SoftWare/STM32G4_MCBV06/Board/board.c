#include "board.h"
#include "initmodule.h"
#include "debuglog.h"
#include "taskmodule.h"
/************************传感器*****************************/

#include "motorctrl.h"
#include "hall_sensor.h"
#include "motorctrl_common.h"
#include "adc.h"
#include "usart.h"
#include "string.h"
#include "tim.h"
#include "flash.h"
#include "gpio.h"
#include "tim.h"
#include "fsm.h"
void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
    static unsigned char sg_uartsend_buf[125];
    memcpy(sg_uartsend_buf,pdata,len);
    HAL_UART_Transmit_DMA(&hlpuart1,sg_uartsend_buf,len);
}

void user_softresetsystem(void)
{
	HAL_NVIC_SystemReset();
}


static void motor_enable(void)
{
    gpio_setencoder_power();
    tim_abzencoder_enable();
    tim_pwm_enable();
    tim_tigger_adc();
    adc_start();
}
static void motor_disable(void)
{
    tim_abzencoder_disable();
    tim_pwm_disable();
    adc_stop();
}
static void motor_set_pwm(float _a,float _b,float _c)
{
    tim_set_pwm(_a ,_b,_c);
}




static void _convert_current(uint16_t* adc_buf,float *i_abc)
{
    i_abc[2]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[0] - 2120) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE); 
    i_abc[1]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[1] - 2120) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE);          //shunt_conductance_ = 1/0.001采样电阻;
    i_abc[0]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[2] - 2130) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE);
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






static uint8_t hall_get_sectionnumb(void)
{
    uint8_t u,v,w;
    u = HAL_GPIO_ReadPin(HALL_U1_GPIO_Port,HALL_U1_Pin);
    v = HAL_GPIO_ReadPin(HALL_V1_GPIO_Port,HALL_V1_Pin);
    w = HAL_GPIO_ReadPin(HALL_W1_GPIO_Port,HALL_W1_Pin);
    return v | (u<<1) | (w<<2);
}
static uint32_t hall_gettick()
{
    return 0;
}

static fsm_cb_t Motor1Fsm;
motor_t motor1 = {0};
static hall_sensor_t hall_sensor;
/**
 * 电机初始化
 */
void motorctrl_init(void)
{

    /*HALL_ABZ传感器初始化  和电机模块无关*/
    hall_register((void*)&(hall_sensor),\
                                        hall_get_sectionnumb,\
                                        hall_gettick,\
                                        tim_abzencoder_getcount,\
                                        tim_abzencoder_setcount,\
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
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{    
    unsigned short adc_vale[3];
    float iabc[3]; 
    {
        adc_vale[1] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc4,ADC_INJECTED_RANK_1);
        adc_vale[2] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc4,ADC_INJECTED_RANK_2);
        adc_vale[0] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc4,ADC_INJECTED_RANK_3);
        _convert_current((uint16_t*)adc_vale,iabc);

        iabc[0] = -iabc[0];
        iabc[1] = -iabc[1];
        iabc[2] = -iabc[2];
        // __cycleof__("mc_hightfreq_task") {
            mc_hightfreq_task(iabc,&motor1);
        // }            
    }
}

void motorctrl_task(void)
{
    motortctrl_process(&Motor1Fsm);
}

void user_board_init(void)
{
    motorctrl_init();
}
void board_deinit(void)
{

}
board_task(motorctrl_task)


