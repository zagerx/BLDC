#include "./pid.h"
void pid_init(pid_cb_t *pid,float kp,float ki,float kc,\
                            float outmax,float outmin)
{
    if (!pid)
    {
        return;
    }
    pid->kp = kp;
    pid->ki = ki;
    pid->kc = kc;
    pid->u_i = 0.0f;
    pid->satErr = 0.0f;
    pid->out_debug = 0.0f;
    pid->out_max = outmax;
    pid->out_min = outmin;
}

void pid_reset(pid_cb_t *pid)
{
    if (!pid)
    {
        return;
    }
    pid->out_debug = 0.0f;
    pid->u_i = 0.0f;
    pid->satErr = 0.0f;
}
float pid_contrl(pid_cb_t *pid,float tar,float cur)
{
    if (!pid)
    {
        return 0.0f;
    }
    float err,u_p,v_out,presat;
    err = tar - cur;
    u_p = pid->kp*err;
    pid->u_i = pid->u_i + pid->ki*u_p + pid->kc*pid->satErr;
    presat = u_p+pid->u_i;
    if(presat>pid->out_max)
    {
        v_out = pid->out_max;
    }else if(presat < pid->out_min)
    {
        v_out = pid->out_min;
    }else{
        v_out = presat;
    }
    pid->satErr = v_out - presat;
    pid->out_debug = v_out;
    return v_out;
}
