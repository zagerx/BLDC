#ifndef MOTOR_CARIBLE_H
#define MOTOR_CARIBLE_H

#include <stdbool.h>
#include "carlib_cfg.h"
#include "device.h"
#include "statemachine.h"
#include "openloop_voltage.h"
#include "_current_calib.h"
#include "_encoder_calib.h"
enum carible_state {
	CARIBLE_CURR_INIT = 1,
	CARIBLE_CURR_RUNING,
	CARIBLE_CURR_DONE,
	CARIBLE_ENCODER_INIT,
	CARIBLE_ENCODER_RUNING,
	CARIBLE_ENCODER_DONE,

	CARIBLE_ERR,
	ALL_CALIB_DONE,
};
typedef uint32_t calib_error_t;
struct calibration_modules {
	struct carlib_config cfg;
	/* ---------- 子模块 ---------- */
	struct carlib_current curr_calib;
	// struct carlib_rslsflux rslsflux_calib;
	struct carlib_encoder encoder_calib;

	/* ---------- 状态 ---------- */
	enum carible_state state;
	calib_error_t error;
};

void calibration_modules_init(struct calibration_modules *calib, struct device *inverter,
			      struct device *currents, struct device *feedback,
			      struct openloop_voltage *op, struct motor_parameters *params);

int calibration_modules_update(struct calibration_modules *calib, float dt);
#endif
