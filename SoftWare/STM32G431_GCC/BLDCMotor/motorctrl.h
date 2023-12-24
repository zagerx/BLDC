#ifndef __MOTORCTRL__H
#define __MOTORCTRL__H
#include "./bldcmotor_cfg.h"
void motortctrl_process(void);
void motorctrl_foccalc(unsigned int *abc_vale,float _elec_theta);
#endif
