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


void baord_process(void)
{
    float a;
    a = adc_getval()*(3.3f/4096);
}

board_task(baord_process)