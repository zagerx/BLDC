#include "board.h"
#include "initmodule.h"
#include "debuglog.h"
/************************传感器*****************************/
#include "sensor.h"
#undef NULL
#define NULL 0


#ifndef ENCODER_TYPE_HALL    
#error "ENCODER_TYPE_HALL is not defined! Compilation stopped."
#else

#endif

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

#include "tim.h"
void motor_enable(void)
{
    gpio_setencoder_power(); 
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

#include "motorctrl.h"
#include "adc.h"
static void _convert_current(uint16_t* adc_buf,float *i_abc)
{
    i_abc[0]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[0] - (1 << 11)) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE); 
    i_abc[1]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[1] - (1 << 11)) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE);          //shunt_conductance_ = 1/0.001采样电阻;
    i_abc[2]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[2] - (1 << 11)) * (1/AMPLIFICATION_FACTOR)) * (1/SAMPLING_RESISTANCE);
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
            mc_hightfreq_task(iabc);
        // }            
    }
}

#include "board.h"
#include "hall_sensor.h"
#include "gpio.h"
#include "motorctrl_common.h"
extern mc_param_t mc_param;
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
void mc_hallencoder_init(void)
{
    hall_init(&(mc_param.encoder_handle.sensor),hall_get_sectionnumb,hall_gettick);
    mc_param.encoder_handle.update = hall_update;
    mc_param.encoder_handle.cacle = hall_cale;
}






void user_softresetsystem(void)
{
	HAL_NVIC_SystemReset();
}
board_init(user_board_init)
