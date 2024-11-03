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

#include "board.h"
#if (ENCODER_TYPE == ENCODER_TYPE_HALL)
    #include "hall_sensor.h"
    #include "gpio.h"
    #include "motorctrl_common.h"
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
#endif
