#ifndef __PID__H
#define __PID__H

typedef struct pid_contrlblock
{
    float kp;
    float ki;
    float kd;
    float kc;
    float u_i;
    float satErr;
    unsigned short out_max;
    unsigned short out_min;
    
}pid_cb_t;

extern float pid_contrl(pid_cb_t *pid,float tar,float cur);
extern void pid_init(pid_cb_t *pid,float kp,float ki,float kc,float outmax,float outmin);
extern void pid_reset(pid_cb_t *pid);
#endif
