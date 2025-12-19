#include <stddef.h>

#include "_encoder_calib.h"
#include "device.h"
#include "inverter.h"
#include "motor.h"
#include "_pp_ident.h"
#include "_current_calib.h"
#include "motor_carible.h"
#include "statemachine.h"
#include "motor_mode.h"
#include "motor_state.h"

#include "currsmp.h"
#include "inverter.h"
#include "stm32_pwm.h"

#include "feedback.h"
#include "vce2755.h"

extern struct device motor1;
struct motor_parameters motor1_params;

static struct inverter_config inverter1_cfg = {
	.tim_pwm_start = tim1_pwm_start,
	.tim_pwm_stop = tim1_pwm_stop,
	.tim_pwm_set = tim1_pwm_set,
};

static struct device inverter1 = {
	.config = &inverter1_cfg,
};

static struct currsmp_config currsmp1_conf = {
	.adc_rang = 4096,
	.vol_ref = 3.3f,
	.rs = 0.001f,
	.opm = 50.0f,
	.r1 = 1000.0f,
	.r2 = 30000.0f,
	.params = &(motor1_params.currsmp_params),
};
static struct currsmp_data currsmp1_data = {0};

static struct device currsmp1 = {
	.config = &currsmp1_conf,
	.data = &currsmp1_data,
};

static struct feedback_config feedback1_cfg = {
	.get_raw = vce2755_read_raw_single,
	.params = &(motor1_params.feedback_params),
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
	.current_state = motor_idle_state,
	.previous_state = motor_idle_state,
	.sub_state_machine = NULL,
	.p1 = &motor1,
};

static struct pp_ident_config pp_cfg = {
	.feedback = &feedback1,
	.inverter = &inverter1,
	.encoder_max = 16383,
	.openloop_speed = 5.0f,
	.openloop_voltage = 0.03f,
	.rotate_duration = 6.0f,
	.align_duration = 1.0f,
};
static struct pp_ident_data pp_data = {0};
static struct device pp1 = {
	.config = &pp_cfg,
	.data = &pp_data,
};
static struct curr_calib_config curr_calib_cfg1 = {
	.currsmp = &currsmp1,
	.inverter = &inverter1,
	.align_duration = 1.0f,
	.sample_count = 1000,
};
static struct curr_calib_data curr_calib_data1 = {0};
static struct device curr_calib = {
	.config = &curr_calib_cfg1,
	.data = &curr_calib_data1,
};
static struct encoder_calib_config ec_carb_cfg1 = {
	.feedback = &feedback1,
	.inverter = &inverter1,
	.encoder_max = 16383,
	.speed = 5.0f,
	.voltage = 0.03f,

};
static struct encoder_calib_data ec_carb_data1 = {0};
static struct device encoder1_calib = {
	.config = &ec_carb_cfg1,
	.data = &ec_carb_data1,
};
static struct motor_calibration_modules m1_calib_modul = {
	.pp_ident = &pp1,
	.current_calibration = &curr_calib,
	.encoder_calibration = &encoder1_calib,
	.state = M_CARIBLE_CURR_STATR,
};
#include "foc_trajectory_plan.h"
s_tarj_config_t scp1_conf = {
	.acc = 9.8f,  // m/s²
	.vmax = 0.4f, // m/s
};
s_tarj_data_t scp1_data;
static struct device m1_planner_dev = {
	.config = &scp1_conf,
	.data = &scp1_data,
};
struct foc_parameters foc1_data = {
	.id_pi_control.params = &motor1_params.d_pi_params,
	.iq_pi_control.params = &motor1_params.q_pi_params,
	.velocity_pi_control.params = &motor1_params.vel_pi_params,
	.pos_pi_control.params = &motor1_params.pos_pi_params,
};
static struct motor_data m1_data = {
	.statue = MOTOR_STATE_IDLE,
	.mode = MOTOR_MODE_IDLE,
	.faultcode = MOTOR_FAULTCODE_NOERR,
	.state_machine = &m1_statemachine,
	.calib = &m1_calib_modul,
	.s_planner = &m1_planner_dev,
	.foc_data = &foc1_data,
};

struct device motor1 = {
	.config = &m1_cfg,
	.data = &m1_data,
};
