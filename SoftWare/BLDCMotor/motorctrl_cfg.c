#include "motorctrl_cfg.h"
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

}
__attribute__((weak)) void user_softresetsystem(void)
{

}

#ifndef ENCODER_TYPE_HALL    
#else
#include "hall_sensor.h"
#include "gpio.h"
#include "motorctrl_common.h"
static uint8_t hall_get_sectionnumb(void)
{
  uint8_t u,v,w;
  u = HAL_GPIO_ReadPin(HALL_U1_GPIO_Port,HALL_U1_Pin);
  v = HAL_GPIO_ReadPin(HALL_V1_GPIO_Port,HALL_V1_Pin);
  w = HAL_GPIO_ReadPin(HALL_W1_GPIO_Port,HALL_W1_Pin);
  return u | (v<<1) | (w<<2);
}
static uint32_t hall_gettick()
{
    return 0;
}
void mc_hallencoder_init(void)
{
    hall_init(&(mc_param.encoder_handle.hallsensor),hall_get_sectionnumb,hall_gettick);    
}
#endif
