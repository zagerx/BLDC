#ifndef __MOTORCTRL__H
#define __MOTORCTRL__H
#include "./motorctrl_cfg.h"
void motortctrl_process(void);
void mc_hightfreq_task(uint16_t a,uint16_t b,uint16_t c);
void _50uscycle_process(unsigned int *abc_vale,float _elec_theta);
void motorprotocol_pause(char *cmd);
void motorprotocol_getdata(char *data);
void motorprotocol_process(void);
#endif
