#include "./pid.h"
#include "../method/method_cfg.h"
void pid_init(pid_cb_t *pid,float kp,float ki,float kc,\
                            float outmax,float outmin)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kc = kc;
    pid->u_i = 0.0f;
    pid->satErr = 0.0f;
    pid->out_max = outmax;
    pid->out_min = outmin;
}

void pid_reset(pid_cb_t *pid)
{
    pid->u_i = 0.0f;
    pid->satErr = 0.0f;
}
float pid_contrl(pid_cb_t *pid,float tar,float cur)
{
    float err = 0.0f,u_p = 0.0f,v_out = 0.0f,presat = 0.0f;
    err = tar - cur;
    u_p = pid->kp*err;
    pid->u_i = pid->u_i + pid->ki*u_p + pid->kc*pid->satErr;
    presat = u_p + pid->u_i;
    v_out = presat;


    if(v_out > pid->out_max)
    {
        v_out = pid->out_max;
    }
    if(v_out < pid->out_min)
    {
        v_out = pid->out_min;
        // pid->satErr = v_out - presat;
    }

    pid->satErr = v_out - presat;
    
    return v_out;
}
