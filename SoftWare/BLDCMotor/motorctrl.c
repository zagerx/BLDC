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
/*==========================================================================================
 * @brief        注册电机控制状态机
 * @FuncName     
 * @param        obj 
 * @param        pdata 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void motorfsm_register(void *obj,void *pdata)
{
    fsm_cb_t* motorfsm = (fsm_cb_t*)obj;
    motorfsm->fsm = (fsm_t *)motor_normalmode;
    motorfsm->count = 0;
    motorfsm->chState = ENTER;
    motorfsm->pdata = pdata;
}
/*==========================================================================================
 * @brief        执行当前状态机   
 * @FuncName     
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void motortctrl_process(void *obj)
{
    fsm_cb_t* fsm = (fsm_cb_t*)obj;
    DISPATCH_FSM(fsm);
    return;
}
/*==========================================================================================
 * @brief        更新编码器基准
 * @FuncName     
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void motorctrl_encoder_update(void *obj)
{
    motor_t *motor = (motor_t *)obj;
    mc_encoder_update(&(motor->encoder_handle));
}
/*==========================================================================================
 * @brief        更新电流
 * @FuncName     
 * @param        obj 
 * @param        iabc 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void motorctrl_currment_update(void *obj,float *iabc)
{
#ifdef MOTOR_OPENLOOP
    mc_openloop(iabc,obj);
#else    
    float theta,next_theta,speed;
    motor_t *motor = (motor_t *)obj;
    //更新当前电流值
    motor->currment_handle.i_abc[0] = iabc[0];
    motor->currment_handle.i_abc[1] = iabc[1];
    motor->currment_handle.i_abc[2] = iabc[2];
    //更新角度
    mc_encoder_read(&(motor->encoder_handle));
    theta = motor->encoder_handle.ele_theta;
    speed = motor->encoder_handle.speed;
    next_theta = theta;// + 1.5f * CURRMENT_PERIOD * speed;
    //更新电流环
    motor->currment_handle.theta = theta;
    motor->currment_handle.next_theta = next_theta;
    duty = currment_loop(&(motor->currment_handle));
    motor->setpwm(duty._a,duty._b,duty._c);           
#endif
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

/*==========================================================================================
 * @brief        电机控制传感器的注册
 * @FuncName     
 * @param        motor 
 * @param        ... 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void motor_encoder_register(motor_t* motor,...)
{
    va_list args;
    va_start(args, motor);    
    motor->encoder_handle.sensor = va_arg(args,void *);
    va_end(args);
}
/*==========================================================================================
 * @brief        电控模块执行器的注册
 * @FuncName     
 * @param        motor 
 * @param        ... 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
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
