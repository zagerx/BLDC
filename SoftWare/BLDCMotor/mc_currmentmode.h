#ifndef __MC_CURRMENTMODE__H
#define __MC_CURRMENTMODE__H

#include "motorctrl_common.h"

duty_t currment_loop(uint16_t a,uint16_t b,uint16_t c,float theta,float pre_theta);

#endif

