#include "mc_loop.h"
#include "mc_utils.h"
#include "mc_encoder.h"
#include "motor_mode_speed.h"
#include "motor_mode_normal.h"
#include "motor_mode_speed.h"
#include "motor_mode_encoderop.h"

#include "motorctrl_common.h"
#include "mc_loop_open.h"
#include "board.h"
#include "fsm.h"
#include "taskmodule.h"
#include "initmodule.h"
#include "debuglog.h"
#include "string.h"
#include "stdarg.h"

#include "mc_commend.h"

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
    motorfsm->fsm = (fsm_t *)selfopenloop_mode;    
    motorfsm->count = 0;
    motorfsm->chState = ENTER;
    motorfsm->pdata = pdata;

    //TODO
    motor_t* motor = (motor_t*)(motorfsm->pdata);
    motor->lastmode = MODE_SELFOPENLOOP;
    motor->curmode = MODE_SELFOPENLOOP;
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
    motor_t *motor = (motor_t*)(fsm->pdata);
    if (motor->curmode != motor->lastmode)
    {
        /* 状态机迁移 */
        if (motor->curmode == MODE_SPEED)
        {
            TRAN_STATE(fsm,motor_speedmode);
            motor->lastmode = motor->curmode;
        }else if (motor->curmode == MODE_ENCODER)
        {
            TRAN_STATE(fsm,motor_encoder_ol_mode);
            motor->lastmode = motor->curmode;
        }else{
        }
    }
    /* 状态机执行 */
    DISPATCH_FSM(fsm);
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
    motor_t* motor = (motor_t*)obj;
    if (motor->curMotorstate != MOTOR_STATUS_RUN)
    {
        return;
    }
#if(MOTOR_WORK_MODE == MOTOR_DEBUG_SELF_MODE)
    mc_self_openloop_VF(motor,iabc);
#else
    float duty[3] = {0};
    /*获取角度 速度*/
    mc_encoder_read(&(motor->encoder_handle));
    motor->currment_handle.i_abc[0] = iabc[0];
    motor->currment_handle.i_abc[1] = iabc[1];
    motor->currment_handle.i_abc[2] = iabc[2];
    currment_loop(motor,duty);
    motor->setpwm(duty[0],duty[1],duty[2]);
#endif
}

/*==========================================================================================
 * @brief        电机控制传感器的注册
 * @FuncName     
 * @param        motor 
 * @param        ... 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void motor_encoder_register(void* obj,...)
{
    va_list args;
    va_start(args, obj);    
    motor_t* motor = (motor_t*)obj;
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
void motor_actor_register(void* obj,...)
{
    va_list args;
    va_start(args, obj);    
    motor_t* motor = (motor_t*)obj;    
    motor->enable = va_arg(args,void (*)(void));
    motor->disable = va_arg(args,void (*)(void));
    motor->setpwm = va_arg(args,void (*)(float,float,float));
    va_end(args);    
}
