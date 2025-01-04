#ifndef __MC_CURRMENTLOOP__H
#define __MC_CURRMENTLOOP__H

#include "motorctrl_common.h"
void currment_loop(void *obj,float *duty);
float speed_loop(mc_speed_t *pthis);

#endif

