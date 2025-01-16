/****************************************************************************
Filename: mc_commend.c
Functions list:
Notes:
Copyright (c) 2022, Syrius Robotics
All rights reserved.
****************************************************************************/
#include "mc_commend.h"
#include "motorctrl_common.h"
#include "debuglog.h"
#include "datatypecovert.h"
extern motor_t motor1;
/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        obj 
 * @param        pdata 
 * @param        datalen 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void motor_get_motorstop(void *obj,uint8_t *pdata,uint16_t datalen)
{
    motor_t *motor = (motor_t*)obj;
    motor->curmode = STATUS_STOP;
}
void motor_get_motorstart(void *obj,uint8_t *pdata,uint16_t datalen)
{
    motor_t *motor = (motor_t*)obj;
    motor->curmode = STATUS_START;
}

void motor_get_speedmode(void *obj,uint8_t *pdata,uint16_t datalen)
{
    motor_t *motor = (motor_t*)obj;
    motor->curmode = MODE_SPEED;
}

void motor_get_encodermode(void *obj,uint8_t *pdata,uint16_t datalen)
{
    motor_t *motor = (motor_t*)obj;
    motor->curmode = MODE_ENCODER;
}

void motor_get_normolmode(void *obj,uint8_t *pdata,uint16_t datalen)
{
    motor_t *motor = (motor_t*)obj;
    motor->curmode = MODE_NORMAL;
}

void motot_get_reset(void *obj,uint8_t *pdata,uint16_t datalen)
{
    motor_t *motor = (motor_t*)obj;
    // motor->reset_system();
}
void motot_get_pidtarge(void *obj,uint8_t *pdata,uint16_t datalen)
{
    motor_t *motor = (motor_t*)obj;
    float temp;
    convert_floats(pdata,datalen,&(temp));   
    motor->debug.pid_debug_target = temp;
    USER_DEBUG_NORMAL("PID Targe update = %.02f\n",temp);    
}

