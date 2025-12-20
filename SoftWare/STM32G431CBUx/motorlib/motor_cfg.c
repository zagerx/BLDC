#include <stddef.h>
#include "motor_cfg.h"
#include "_encoder_calib.h"
#include "device.h"
#include "inverter.h"
#include "motor.h"
#include "_pp_ident.h"
#include "_current_calib.h"
#include "motor_carible.h"
#include "statemachine.h"
#include "motor_state.h"

#include "currsmp.h"
#include "inverter.h"
#include "stm32_pwm.h"

#include "feedback.h"
#include "stm32_mt6816.h"
#include "stm32_as5047.h"

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
	.adc_rang = BOARD_ADC_RES,
	.vol_ref = BOARD_ADC_VREF_V,
	.rs = BOARD_SHUNT_R_OHM,
	.opm = BOARD_OPAMP_GAIN,
	.params = &(motor1_params.currsmp_params),
};
static struct currsmp_data currsmp1_data = {0};

static struct device currsmp1 = {
	.config = &currsmp1_conf,
	.data = &currsmp1_data,
};

static struct feedback_config feedback1_cfg = {
	.get_raw = as5047_read_raw,
	.params = &motor1_params.feedback_params,
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
	.encoder_max = ENC_CPR,
	.openloop_speed = OPENLOOP_SPEED_RPM,
	.openloop_voltage = OPENLOOP_VOLTAGE_V,
	.rotate_duration = ROTATE_DURATION_S,
	.align_duration = ALIGN_DURATION_S,
};
static struct pp_ident_data pp_data = {0};
static struct device pp1 = {
	.config = &pp_cfg,
	.data = &pp_data,
};
static struct curr_calib_config curr_calib_cfg1 = {
	.currsmp = &currsmp1,
	.inverter = &inverter1,
	.align_duration = ROTATE_DURATION_S,
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
	.encoder_max = ENC_CPR,
	.speed = OPENLOOP_SPEED_RPM,
	.voltage = OPENLOOP_VOLTAGE_V,

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
#include "t_trajectory.h"
static s_tarj_config_t scp1_config = {
	.acc = 5.0f,  // 2 m/s²
	.vmax = 4.0f, // 1 m/s
};
static s_tarj_data_t scp1_data;
static struct device traj_plan1 = {
	.config = &scp1_config,
	.data = &scp1_data,
};
struct foc_data foc1_data = {
	.id_pi_control.params = &motor1_params.d_pi_params,
	.iq_pi_control.params = &motor1_params.q_pi_params,
	.velocity_pi_control.params = &motor1_params.vel_pi_params,
	// .pos_pi_control.params = &motor1_params.pos_pi_params,
};
static struct motor_data m1_data = {
	.statue = MOTOR_STATE_IDLE,
	.mode = MOTOR_MODE_IDLE,
	.faultcode = MOTOR_FAULTCODE_NOERR,
	.state_machine = &m1_statemachine,
	.calib = &m1_calib_modul,
	.scp = &traj_plan1,
	.foc_data = &foc1_data,

};

struct device motor1 = {
	.config = &m1_cfg,
	.data = &m1_data,
};
