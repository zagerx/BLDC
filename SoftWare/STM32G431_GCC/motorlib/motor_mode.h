#ifndef MOTOR_MODE_H
#define MOTOR_MODE_H
#include "statemachine.h"

fsm_rt_t motor_torque_control_mode(fsm_cb_t *obj);
fsm_rt_t motor_idle_control_mode(fsm_cb_t *obj);
fsm_rt_t motor_speed_control_mode(fsm_cb_t *obj);
fsm_rt_t motor_position_control_mode(fsm_cb_t *obj);
fsm_rt_t motor_calibration_mode(fsm_cb_t *obj);

#endif
