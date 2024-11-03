#include "board.h"
#include "initmodule.h"
#include "taskmodule.h"
#include "debuglog.h"
#include "stm32g4xx_hal.h"
// #include "usart.h"
// #include "string.h"
// void _bsp_protransmit(unsigned char* pdata,unsigned short len)
// {
//     static unsigned char sg_uartsend_buf[125];
//     memcpy(sg_uartsend_buf,pdata,len);
//     HAL_UART_Transmit_DMA(&huart3,sg_uartsend_buf,len);
// }

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




void baord_process(void)
{
    float a;
    a = adc_getval()*(3.3f/4096);
}

board_task(baord_process)