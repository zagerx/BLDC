#ifndef __MOTORCTRL__H
#define __MOTORCTRL__H
#include "./motorctrl_cfg.h"

void motorctrl_init(void);
void motortctrl_process(void *obj);
void mc_hightfreq_task(float *iabc,motor_t *motor);
void motorfsm_register(void *obj,void *pdata);
void motor_encoder_register(motor_t* motor,...);
void motor_actor_register(motor_t* motor,...);

#endif
