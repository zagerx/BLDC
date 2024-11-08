#ifndef __MOTORCTRL__H
#define __MOTORCTRL__H
#include "./motorctrl_cfg.h"

void motorctrl_init(void);
void motortctrl_process(void);
void mc_hightfreq_task(float *iabc,motor_t *motor);

#endif
