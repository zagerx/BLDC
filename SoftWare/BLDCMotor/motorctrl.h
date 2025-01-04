#ifndef __MOTORCTRL__H
#define __MOTORCTRL__H


void motorctrl_init(void);
void motortctrl_process(void *obj);
void motorfsm_register(void *obj,void *pdata);
void motor_encoder_register(void* obj,...);
void motor_actor_register(void* obj,...);
void motorctrl_encoder_update(void *obj);
void motorctrl_currment_update(void *obj,float *iabc);

#endif
