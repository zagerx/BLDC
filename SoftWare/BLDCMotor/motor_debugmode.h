#ifndef __MOTOR_DEBUGMODE__H
#define __MOTOR_DEBUGMODE__H

#include "fsm.h"
fsm_rt_t motor_debugmode(fsm_cb_t *pthis);
void mc_test(float *iabc,float omega);
void mc_encoderopenlooptest(float *iabc,float omega);



#endif
