#include "mc_speedmode.h"
#include "motorctrl_common.h"
#include "pid.h"
extern mc_param_t mc_param;

float speed_loop(float tar,float real)
{
    return pid_contrl(&(mc_param.speedloop_pi),tar,real);
}

void speed_loop_paraminit(pid_cb_t *pid)
{

}

void speed_loop_paramdeinit(void)
{

}