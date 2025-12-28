#include <stddef.h>
#include "motor_cfg.h"
#include "_encoder_calib.h"
#include "device.h"
#include "inverter.h"
#include "motor.h"
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

static struct inverter_config inverter1_cfg = {
	.tim_pwm_enable = tim1_pwm_start,
	.tim_pwm_disable = tim1_pwm_stop,
	.tim_pwm_set = tim1_pwm_set,
};
static struct inverter_t inverter1 = {
	.config = &inverter1_cfg,
};

static struct currsmp_config currsmp1_conf = {
	.adc_rang = BOARD_ADC_RES,
	.vol_ref = BOARD_ADC_VREF_V,
	.rs = BOARD_SHUNT_R_OHM,
	.opm = BOARD_OPAMP_GAIN,
	.r2 = 75000.0f,
	.r1 = 2670.0f,
	.params = NULL,
};
static struct currsmp_data currsmp1_data = {0};
static struct currsmp_t currsmp1 = {
	.config = &currsmp1_conf,
	.data = &currsmp1_data,
};

static struct feedback_config feedback1_cfg = {
	.get_raw = as5047_read_raw,
	.radius = 50.0f,
	.params = NULL,
};
static struct feedback_data feedback1_data = {0};
static struct feedback_t feedback1 = {
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

#include "t_trajectory.h"
static t_tarj_config_t scp1_config = {
	.acc = 3.0f,  // 2 m/s²
	.vmax = 5.0f, // 1 m/s
};
static t_tarj_data_t scp1_data;
static struct device traj_plan1 = {
	.config = &scp1_config,
	.data = &scp1_data,
};

static struct motor_data m1_data = {
	.statue = MOTOR_STATE_IDLE,
	.mode = MOTOR_MODE_IDLE,
	.faultcode = MOTOR_FAULTCODE_NOERR,
	.state_machine = &m1_statemachine,
	.scp = &traj_plan1,
};

struct device motor1 = {
	.config = &m1_cfg,
	.data = &m1_data,
};
