#ifndef __MOTORCTRL__H
#define __MOTORCTRL__H
#include "./motorctrl_cfg.h"
void motortctrl_process(void);
void _currentloop(unsigned int *abc_vale,float _elec_theta);
#endif
