#include "mc_posloop.h"
#include "stdint.h"
#include "mc_encoder.h"
#include "motorctrl_common.h"
void mc_posloop(mc_pos_t *pthis)
{
    mc_speed_t *speed;
    speed = &(mc_param.speed_handle);
    float real_pos = mc_param.encoder_handle.total_realmectheta;
    speed->tar = pid_contrl(&(pthis->pi_contrl),pthis->tar,real_pos);
    //更新到速度环
    speed->real = mc_param.encoder_handle.speed;
    // speed_loop(speed);
    mc_param.currment_handle.iq_tar = pid_contrl(&(speed->pid),speed->tar,speed->real);
    mc_param.currment_handle.id_tar = 0;
}

