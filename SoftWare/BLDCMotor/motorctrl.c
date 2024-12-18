#include "mc_protocol.h"
#include "mc_currmentloop.h"
#include "mc_utils.h"
#include "mc_encoder.h"
#include "motor_speedmode.h"
#include "motor_normalmode.h"
#include "motor_posmode.h"
#include "motorctrl_cfg.h"
#include "motorctrl_common.h"
#include "mc_openloop.h"
#include "board.h"
#include "fsm.h"
#include "taskmodule.h"
#include "initmodule.h"
#include "debuglog.h"
#include "string.h"
#include "stdarg.h"

void motorfsm_register(void *obj,void *pdata)
{
    fsm_cb_t* motorfsm = (fsm_cb_t*)obj;
    motorfsm->fsm = (fsm_t *)motor_normalmode;
    motorfsm->count = 0;
    motorfsm->chState = ENTER;
    motorfsm->pdata = pdata;
}

void motortctrl_process(void *obj)
{
    fsm_cb_t* fsm = (fsm_cb_t*)obj;
    DISPATCH_FSM(fsm);
    return;
}

void mc_hightfreq_task(float *iabc,motor_t *motor)
{
#ifdef MOTOR_OPENLOOP
    mc_openloop(iabc,motor);
#else
    duty_t duty = {0};
    /*获取角度 速度*/
	float theta = 0.0f;
    float next_theta = 0.0f;
    float speed = 0.0f;

    mc_encoder_read(&(motor->encoder_handle));
    theta = motor->encoder_handle.ele_theta;
    speed = motor->encoder_handle.speed;
    next_theta = theta;// + 1.5f * CURRMENT_PERIOD * speed;

    motor->currment_handle.i_abc[0] = iabc[0];
    motor->currment_handle.i_abc[1] = iabc[1];
    motor->currment_handle.i_abc[2] = iabc[2];
    motor->currment_handle.theta = theta;
    motor->currment_handle.next_theta = next_theta;
    duty = currment_loop(&(motor->currment_handle));
    motor->setpwm(duty._a,duty._b,duty._c);
#endif
}
void motor_encoder_register(motor_t* motor,...)
{
    va_list args;
    va_start(args, motor);    
    motor->encoder_handle.sensor = va_arg(args,void *);
    motor->encoder_handle.init = va_arg(args,void (*)(void*));
    motor->encoder_handle.deinit = va_arg(args,void (*)(void*));
    motor->encoder_handle.update = va_arg(args,uint8_t (*)(void*));
    motor->encoder_handle.cacle = va_arg(args,void (*)(void*));
    motor->encoder_handle.get_firstpos = va_arg(args,void (*)(void*));;
    motor->encoder_handle.set_calib_points = va_arg(args,void (*)(void*));;
    va_end(args);
}

void motor_actor_register(motor_t* motor,...)
{
    va_list args;
    va_start(args, motor);    
    motor->enable = va_arg(args,void (*)(void));
    motor->disable = va_arg(args,void (*)(void));
    motor->setpwm = va_arg(args,void (*)(float,float,float));
    motor->reset_system = va_arg(args,void (*)(void));
    motor->write = va_arg(args,void (*)(void*,uint16_t));
    motor->read = va_arg(args,void (*)(void*,uint16_t));;
    va_end(args);    
}
