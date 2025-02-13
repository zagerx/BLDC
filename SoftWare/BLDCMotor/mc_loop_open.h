#ifndef __MC_OPENLOOP__h
#define __MC_OPENLOOP__h
#include "fsm.h"
void mc_self_openloop_VF(motor_t* motor,float *iabc);
fsm_rt_t selfopenloop_mode(fsm_cb_t *pthis);
#endif
