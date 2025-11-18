#ifndef MOTOR_CARIBLE_H
#define MOTOR_CARIBLE_H

#include <stdbool.h>
#include "device.h"
#include "statemachine.h"
enum {
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
	bool done; // 整个模块校准完成
};

#endif
