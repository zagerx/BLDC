#ifndef __MOTORCTRL__H
#define __MOTORCTRL__H
#include "./motorctrl_cfg.h"


typedef struct _motor_
{
    char *name;
    void (*enble)(void);
    void (*disable)(void);
    void (*setpwm)(void);
}motor_t;



void motortctrl_process(void);
void mc_hightfreq_task(float *iabc);
void motorprotocol_pause(char *cmd);
void motorprotocol_getdata(char *data);
void motorprotocol_process(void);
#endif
