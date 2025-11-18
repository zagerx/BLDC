#include <stddef.h>

#include "device.h"
#include "inverter.h"
#include "motor.h"
#include "motor_pp_ident.h"
#include "motor_currment_carible.h"
#include "statemachine.h"
#include "motor_mode.h"
#include "motor_state.h"

#include "currsmp.h"
#include "inverter.h"
#include "stm32_pwm.h"

#include "feedback.h"
#include "stm32_mt6816.h"

#include "motor_pp_ident.h"
extern struct device motor1;

static struct inverter_config inverter1_cfg = {
	.tim_pwm_start = tim1_pwm_start,
	.tim_pwm_stop = tim1_pwm_stop,
	.tim_pwm_set = tim1_pwm_set,
};

static struct device inverter1 = {
	.config = &inverter1_cfg,
};

static struct currsmp_data currsmp1_data = {0};

static struct device currsmp1 = {
	.data = &currsmp1_data,
};

static struct feedback_config feedback1_cfg = {
	.get_raw = as5047_read_raw,
};

static struct feedback_data feedback1_data = {0};

static struct device feedback1 = {
	.config = &feedback1_cfg,
	.data = &feedback1_data,
};
static struct motor_config m1_cfg = {
	.currsmp = &currsmp1,
	.feedback = &feedback1,
	.inverter = &inverter1,
};

static fsm_cb_t m1_statemachine = {
	.current_state = motor_init_state,
	.sub_state_machine =&NULL,
	.p1 = &motor1,
};

static struct pp_ident_config pp_cfg = {
	.encoder_max = 16383,
	.openloop_speed = 5.0f,
	.openloop_voltage = 0.03f,
	.duration = 6.0f,
};
static struct pp_ident_data pp_data = {0};
static struct device pp1 = {
	.config = &pp_cfg,
	.data = &pp_data,
};
static struct curr_calib_data curr_calib_data1 = {0};
static struct device curr_calib = {
	.data = &curr_calib_data1,
};
static struct motor_calibration_modules m1_calib_modul = {
	.pp_ident = &pp1,
	.current_calibration = &curr_calib,
};

static struct motor_data m1_data = {
	.statue = MOTOR_STATE_IDLE,
	.mode = MOTOR_MODE_IDLE,
	.faultcode = MOTOR_FAULTCODE_NOERR,
	.state_machine = &m1_statemachine,
	.calib = &m1_calib_modul,
};

struct device motor1 = {
	.config = &m1_cfg,
	.data = &m1_data,
};
