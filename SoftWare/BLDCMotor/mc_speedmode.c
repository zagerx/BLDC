#include "mc_speedmode.h"
#include "motorctrl_common.h"
#include "pid.h"
extern float vel_estimate_;          //当前估算转速，单位[turn/s]
extern mc_param_t mc_param;

float speed_loop(float tar)
{
    return pid_contrl(&(mc_param.speedloop_pi),tar,vel_estimate_);// motordebug.real_speed);//
}

void speed_loop_paraminit(pid_cb_t *pid)
{

}

void speed_loop_paramdeinit(void)
{

}