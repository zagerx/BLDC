#include "motorctrl_cfg.h"

__attribute__((weak)) void motor_enable(void)
{
#ifdef BOARD_STM32H723
    gpio_setmotor_power();
#endif
    /*pwm 使能*/
    tim_pwm_enable();
    tim_tigger_adc();
    adc_start();
}
__attribute__((weak)) void motor_disable(void)
{
#ifdef BOARD_STM32H723
    gpio_setmotor_powerdown();
#endif
    tim_pwm_disable();
    adc_stop();
}
__attribute__((weak)) void motor_set_pwm(float _a,float _b,float _c)
{
    tim_set_pwm(_a ,_b,_c);
}
__attribute__((weak)) void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{

}

__attribute__((weak)) void user_softresetsystem(void)
{

}

