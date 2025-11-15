#include <stddef.h>

#include "device.h"
#include "inverter.h"
#include "motor.h"
#include "statemachine.h"
#include "motor_mode.h"
#include "motor_state.h"

#include "currsmp.h"
#include "inverter.h"
#include "stm32_pwm.h"

#include "feedback.h"
#include "stm32_mt6816.h"

extern struct device motor1;

static struct inverter_config inverter1_cfg = {
    .tim_pwm_start = tim1_pwm_start,
    .tim_pwm_stop = tim1_pwm_stop,
    .tim_pwm_set = tim1_pwm_set,
};

static struct device inverter1 = {
    .config = &inverter1_cfg,
};

static struct currsmp_data currsmp1_data = {

};

static struct device currsmp1 = {
    .data = &currsmp1_data,
};

static struct feedback_config feedback1_cfg = {
    .get_raw = mt6816_get_raw,
};

static struct feedback_data feedback1_data = {};

static struct device feedback1 = {
    .config = &feedback1_cfg,
    .data = &feedback1_data,
};

static struct motor_config m1_cfg = {
    .currsmp = &currsmp1,
    .feedback = &feedback1,
    .inverter = &inverter1,
};

static fsm_cb_t m1_state_statemachine = {
    .current_state = motor_init_state,
    .sub_state_machine = NULL,
    .p1 = NULL,
};

static fsm_cb_t m1_mode_statemachine = {
    .current_state = motor_idle_control_mode,
    .sub_state_machine = &m1_state_statemachine,
    .p1 = &motor1,
};

static struct motor_data m1_data = {
    .statue = MOTOR_STATE_IDLE,
    .mode = MOTOR_MODE_IDLE,
    .faultcode = MOTOR_FAULTCODE_NOERR,
    .fsm_mode = &m1_mode_statemachine,
};

struct device motor1 = {
    .config = &m1_cfg,
    .data = &m1_data,
};
