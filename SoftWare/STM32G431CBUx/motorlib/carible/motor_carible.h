#ifndef MOTOR_CARIBLE_H
#define MOTOR_CARIBLE_H

#include <stdbool.h>
#include "device.h"
#include "statemachine.h"
enum m_carible{
	M_CARIBLE_CURR_RUNING = USER_STATUS,
	M_CARIBLE_CURR_DONE,
	M_CARIBLE_PP_CURR_START,
	M_CARIBLE_PP_RUNING,
	M_CARIBLE_PP_DONE,
	M_CARIBLE_ENCODER_RUNING,
	M_CARIBLE_ENCODER_DONE,
	M_CARIBLE_ERR,
	M_ALL_CALIB_DONE,
};
struct motor_calibration_modules {
	struct device *pp_ident;
	struct device *encoder_calibration;
	struct device *current_calibration;
	struct device *rl_ident;
	enum m_carible state;
};

enum m_carible motor_get_calibstate(struct device *motor);
void motor_set_calibstate(struct device *motor,enum m_carible state);


#endif
