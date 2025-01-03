#ifndef __MC_CURRMENTLOOP__H
#define __MC_CURRMENTLOOP__H

#include "motorctrl_common.h"
duty_t currment_loop(void *obj);
float speed_loop(mc_speed_t *pthis);

#endif

