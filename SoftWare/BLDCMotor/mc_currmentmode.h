#ifndef __MC_CURRMENTMODE__H
#define __MC_CURRMENTMODE__H

#include "motorctrl_common.h"

duty_t currment_loop(float *abc,float theta,float next_theta);

#endif

