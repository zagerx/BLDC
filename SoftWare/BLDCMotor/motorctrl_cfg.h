#ifndef __BLDCMOTOR_CFG__H
#define __BLDCMOTOR_CFG__H

__attribute__((weak)) void motor_enable(void);
__attribute__((weak)) void motor_disable(void);
__attribute__((weak)) void motor_set_pwm(float _a,float _b,float _c);
__attribute__((weak)) void _bsp_protransmit(unsigned char* pdata,unsigned short len);
__attribute__((weak)) void user_softresetsystem(void);
void mc_hallencoder_init(void);

#endif
