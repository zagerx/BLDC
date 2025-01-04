#ifndef __BOARD__H
#define __BOARD__H

#define OPENLOOP_DEBUG_STEP_THETA            (+0.001f)
#define OPENLOOP_DEBUG_TOTAL_Te              (+0.08f)
#define MOTOR_PAIRS                          (2.0f)//电机极对数
#define CURRMENT_PERIOD                      (0.0001f)//电流环周期

#define MOTOR_DEBUG_SELF_MODE              (0)
#define MOTOR_DEBUG_ENCODERMODE            (1)
#define MOTOR_CLOSELOOP_ERMODE             (2)
#define MOTOR_WORK_MODE                    MOTOR_DEBUG_SELF_MODE

#define ENCODER_TYPE_SENSORLESS              (0)
#define ENCODER_TYPE_ABS                     (1)
#define ENCODER_TYPE_HALL                    (2)
#define ENCODER_TYPE                         ENCODER_TYPE_HALL

#define STM32G4CUBX
#if (ENCODER_TYPE == ENCODER_TYPE_ABS)

#elif (ENCODER_TYPE == ENCODER_TYPE_HALL)
    #define HALL_UPDATE_PERIOD              (0.0001f)
    #define HALL_POSITIVE_OFFSET            (-0.00f)
    #define HALL_NEGATIVE_OFFSET            (+0.27f)
    #define MEC_ANGLE_OFFSET                (-0.809f)
    #define PLL_KP                          (80.50f)
    #define PLL_KI                          (1.2f)
    #define OMEGTOTHETA                     (0.0001f)     
    //D轴强拖
    // #define SCETION_6_BASEANGLE            (3.4891f)
    // #define SCETION_4_BASEANGLE            (4.5670f)
    // #define SCETION_5_BASEANGLE            (5.581f)
    // #define SCETION_1_BASEANGLE            (0.312f)
    // #define SCETION_3_BASEANGLE            (1.428f)
    // #define SCETION_2_BASEANGLE            (2.424f)
    #define SCETION_6_BASEANGLE            (2.9086f)
    #define SCETION_4_BASEANGLE            (3.9781f)
    #define SCETION_5_BASEANGLE            (4.9991f)
    #define SCETION_1_BASEANGLE            (6.0460f)
    #define SCETION_3_BASEANGLE            (0.8203f)
    #define SCETION_2_BASEANGLE            (1.8650f)    
#endif


#define CURRMENTLOOP_KP             (0.08f)         
#define CURRMENTLOOP_KI             (0.001f)
#define SPEEDLOOP_KP                (0.02f)
#define SPEEDLOOP_KI                (0.01f)
#define POSLOOP_KP                  (0.1f)
#define POSLOOP_KI                  (0.1f)

#define VOLATAGE_BUS         (24.0f)
#define CIRCLE_MAX_VAL       (24.0f)
#define D_MAX_VAL            (12.0f)
#define D_MIN_VAL            -D_MAX_VAL
#define Q_MAX_VAL            D_MAX_VAL
#define Q_MIN_VAL            -Q_MAX_VAL

#define CIRCLE_OUT_MAX      (24.0f) //电流环输出最大值
#define CIRCLE_OUT_MIN      (-24.0f)
#define SPEED_OUT_MAX       (12.0f) //速度环输出最大值
#define SPEED_OUT_MIN       (-12.0f)

void Board_init(void);

#endif
