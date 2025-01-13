#ifndef __BOARD__H
#define __BOARD__H
/**************************************************************************************************
                                        电机选择
**************************************************************************************************/
#if 1
    #include "borado_motor.h"
#else
    #include "super_motor.h"
#endif

/**************************************************************************************************
                                        电机控制模块相关参数
**************************************************************************************************/
#define MOTOR_DEBUG_SELF_MODE               (0)
#define MOTOR_DEBUG_ENCODERMODE             (1)
#define MOTOR_CLOSELOOP_ERMODE              (2)
#define MOTOR_WORK_MODE                     (MOTOR_CLOSELOOP_ERMODE)  //电机工作模式

#define OPENLOOP_DEBUG_TOTAL_Te             (0.04f)                   //电机自开环力矩
#define OPENLOOP_DEBUG_STEP_THETA           (0.0004f)                 //电机自开环自给角度

#define CURRMENT_PERIOD                     (0.0001f)                 //电流环周期
#define SPEED_UPDATE_PERIOD                 (0.002f)                  //速度更新周期

#define CURRMENTLOOP_KP                     (0.01f)                   //电流环 PI参数
#define CURRMENTLOOP_KI                     (0.0004f)
#define SPEEDLOOP_KP                        (0.18f)
#define SPEEDLOOP_KI                        (0.008f)
#define POSLOOP_KP                          (0.1f)
#define POSLOOP_KI                          (0.1f)

/**************************************************************************************************
                                        PCBA板级相关
**************************************************************************************************/
#define AMPLIFICATION_FACTOR                (5.36f)  // 假设放大倍数为10  
#define SAMPLING_RESISTANCE                 (0.005)  // 假设采样电阻为5mR欧姆
#define CIRCLE_MAX_VAL                      (24.0f)
#define VOLATAGE_BUS                        (48.0f) 
#define D_MAX_VAL                           (12.0f)
#define D_MIN_VAL                           -D_MAX_VAL
#define Q_MAX_VAL                           D_MAX_VAL
#define Q_MIN_VAL                           -Q_MAX_VAL
#define CIRCLE_OUT_MAX                      (12.0f) //电流环输出最大值
#define CIRCLE_OUT_MIN                      (-12.0f)
#define SPEED_OUT_MAX                       (12.0f) //速度环输出最大值
#define SPEED_OUT_MIN                       (-12.0f)

void user_board_init(void);
#endif
