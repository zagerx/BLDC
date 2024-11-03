#ifndef __BLDCMOTOR_CFG__H
#define __BLDCMOTOR_CFG__H

// __attribute__((weak)) void motor_enable(void);
// __attribute__((weak)) void motor_disable(void);
// __attribute__((weak)) void motor_setpwm(float _a,float _b,float _c);
// __attribute__((weak)) void _bsp_protransmit(unsigned char* pdata,unsigned short len);
// __attribute__((weak)) void user_softresetsystem(void);
__attribute__((weak)) void mc_MotorModule_init(void);

#endif
