#include "motorctrl_cfg.h"
#include "debuglog.h"

__attribute__((weak)) void motor_enable(void)
{
}
__attribute__((weak)) void motor_disable(void)
{
}
__attribute__((weak)) void motor_set_pwm(float _a,float _b,float _c)
{
}
__attribute__((weak)) void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
    USER_DEBUG_NORMAL("please write _bsp_protransmit(motorctrl_cfg.c)\n");    
}
__attribute__((weak)) void user_softresetsystem(void)
{

}
__attribute__((weak)) void mc_hallencoder_init(void)
{
    USER_DEBUG_NORMAL("please write mc_hallencoder_init(motorctrl_cfg.c)\n");    

}
