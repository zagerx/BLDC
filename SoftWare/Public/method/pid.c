#include "./pid.h"
#include "../method/method_cfg.h"
void pid_init(pid_cb_t *pid,float kp,float ki,float kc,\
                            float outmax,float outmin)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kc = kc;
    pid->u_i = 0.0f;
    pid->satErr = 0;
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
#if 0
        u_p1 = u_p;
        ud = kd*(u_p-u_p1);
        pid->presat = u_p + u_i + u_d;
#endif
    return v_out;
}
