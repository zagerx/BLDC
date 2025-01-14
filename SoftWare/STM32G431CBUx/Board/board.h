#ifndef __BOARD__H
#define __BOARD__H
/**************************************************************************************************
                                        电机选择
**************************************************************************************************/
#include "eulre_motor.h"

/**************************************************************************************************
                                        电机控制模块相关参数
**************************************************************************************************/
#define OPENLOOP_DEBUG_STEP_THETA            (0.0003f)
#define OPENLOOP_DEBUG_TOTAL_Te              (-0.08f)
#define MOTOR_DEBUG_SELF_MODE                (0)
#define MOTOR_DEBUG_ENCODERMODE              (1)
#define MOTOR_CLOSELOOP_ERMODE               (2)
#define MOTOR_WORK_MODE                      (MOTOR_DEBUG_ENCODERMODE)

#define STM32G4CUBX                       
#define CURRMENT_PERIOD                      (0.0001f)//电流环周期

/**************************************************************************************************
                                        PCBA板级相关
**************************************************************************************************/
#define CURRMENTLOOP_KP                     (0.08f)         
#define CURRMENTLOOP_KI                     (0.001f)
#define SPEEDLOOP_KP                        (0.02f)
#define SPEEDLOOP_KI                        (0.01f)
#define POSLOOP_KP                          (0.1f)
#define POSLOOP_KI                          (0.1f)

#define VOLATAGE_BUS                        (24.0f)
#define CIRCLE_MAX_VAL                      (24.0f)
#define D_MAX_VAL                           (12.0f)
#define D_MIN_VAL                           -D_MAX_VAL
#define Q_MAX_VAL                           D_MAX_VAL
#define Q_MIN_VAL                           -Q_MAX_VAL

#define CIRCLE_OUT_MAX                      (24.0f) //电流环输出最大值
#define CIRCLE_OUT_MIN                      (-24.0f)
#define SPEED_OUT_MAX                       (12.0f) //速度环输出最大值
#define SPEED_OUT_MIN                       (-12.0f)

void Board_init(void);

#endif
