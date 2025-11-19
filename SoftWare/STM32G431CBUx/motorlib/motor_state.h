#ifndef MOTOR_STATE_H
#define MOTOR_STATE_H
#include "statemachine.h"
fsm_rt_t motor_idle_state(fsm_cb_t *obj);
fsm_rt_t motor_ready_state(fsm_cb_t *obj);
fsm_rt_t motor_runing_state(fsm_cb_t *obj);
fsm_rt_t motor_stop_state(fsm_cb_t *obj);
fsm_rt_t motor_aligh_state(fsm_cb_t *obj);
fsm_rt_t motor_carible_state(fsm_cb_t *obj);
fsm_rt_t motor_encoder_openloop_state(fsm_cb_t *obj);

#endif

