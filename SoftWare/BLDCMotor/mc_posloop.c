#include "mc_posloop.h"
#include "stdint.h"
#include "mc_encoder.h"
#include "motorctrl_common.h"
void mc_posloop(mc_pos_t *pthis)
{
    float tar_pos,real_pos;
    tar_pos = pthis->tar;
    real_pos = pthis->real;
    /*position loop*/
    float tar_speed;
    tar_speed = pid_contrl(&(pthis->pi_contrl),tar_pos,real_pos);
    /*speed loop*/
    mc_speed_t *speed;
    float real_speed;
    speed = pthis->p_speed;
    real_speed = speed->real;
    motor1.currment_handle.iq_tar = pid_contrl(&(speed->pid),tar_speed,real_speed);
    motor1.currment_handle.id_tar = 0;
}

