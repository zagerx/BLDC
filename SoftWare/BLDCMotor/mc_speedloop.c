#include "mc_speedloop.h"
#include "motorctrl_common.h"

float speed_loop(mc_speed_t *pthis)
{
    return pid_contrl(&(pthis->pid),pthis->tar,pthis->real);
}

void speed_loop_paraminit(pid_cb_t *pid)
{

}

void speed_loop_paramdeinit(void)
{

}