#include "board.h"
#include "initmodule.h"
#include "debuglog.h"
/************************传感器*****************************/

void user_board_init(void)
{
}
void board_deinit(void)
{

}

#include "usart.h"
#include "string.h"
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


#include "tim.h"
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



#include "motorctrl.h"
#include "hall_sensor.h"
#include "motorctrl_common.h"
extern motor_t motor1;
#include "adc.h"
static void _convert_current(uint16_t* adc_buf,float *i_abc)
{
    i_abc[2]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[0] - 2120) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE); 
    i_abc[1]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[1] - 2120) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE);          //shunt_conductance_ = 1/0.001采样电阻;
    i_abc[0]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[2] - 2130) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE);
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






#include "gpio.h"
#include "tim.h"
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
void motor_func_register(motor_t *motor)
{
    hall_register((void*)&(motor->encoder_handle.sensor),hall_get_sectionnumb,\
                                                         hall_gettick,\
                                                         tim_abzencoder_getcount,\
                                                         tim_abzencoder_setcount);
    motor->encoder_handle.deinit = hall_deinit;
    motor->encoder_handle.init = hall_init;
    motor->encoder_handle.update = hall_update;
    motor->encoder_handle.cacle = hall_cale;
    motor->encoder_handle.get_firstpos = hall_get_initpos;
    motor->encoder_handle.set_calib_points = hall_set_calib_points;

    motor->enable = motor_enable;
    motor->disable = motor_disable;
    motor->setpwm = motor_set_pwm;
    motor->reset_system = user_softresetsystem;
    // motor->bsptransmit = _bsp_protransmit;//TODO
    motor->write = motor_write;
    motor->read = motor_read;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
    // motor1.encoder_handle.update(&(motor1.encoder_handle.sensor));
}