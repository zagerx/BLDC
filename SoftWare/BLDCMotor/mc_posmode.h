#ifndef __MC_POSMODEL__H
#define __MC_POSMODEL__H
#include "fsm.h"


void mc_tarpos_update(float delte_dist);

fsm_rt_t motor_posmode(fsm_cb_t *pthis);

#endif


