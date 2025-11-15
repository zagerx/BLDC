#ifndef MOTOR_STATE_H
#define MOTOR_STATE_H
#include "statemachine.h"
fsm_rt_t motor_init_state(fsm_cb_t *obj);
fsm_rt_t motor_ready_state(fsm_cb_t *obj);
fsm_rt_t motor_runing_state(fsm_cb_t *obj);
fsm_rt_t motor_stop_state(fsm_cb_t *obj);
fsm_rt_t motor_aligh_state(fsm_cb_t *obj);
#endif

