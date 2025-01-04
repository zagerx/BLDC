#ifndef __BOARD__H
#define __BOARD__H


#define ENCODER_TYPE_SENSORLESS              (0)
#define ENCODER_TYPE_ABS                     (1)
#define ENCODER_TYPE_HALL                    (2)
#define ENCODER_TYPE                         ENCODER_TYPE_ABS

#define MOTOR_DEBUG_SELF_MODE              (0)
#define MOTOR_DEBUG_ENCODERMODE            (1)
#define MOTOR_CLOSELOOP_ERMODE             (2)
#define MOTOR_WORK_MODE                    MOTOR_DEBUG_ENCODERMODE

#define OPENLOOP_DEBUG_TOTAL_Te              (0.008f)
#define OPENLOOP_DEBUG_STEP_THETA           (0.0002f)

#define CURRMENT_PERIOD      (0.0001f)//电流环周期
#define SPEED_UPDATE_PERIOD  (0.002f)//速度更新周期
#define SPEED_UPDATE_COUNT   (20)       //0.002/CURRMENT_PERIOD

#define MEC_ANGLE_OFFSET     (-0.9f)//编码器偏移值 AS5047:(0.0056f)
#define ENCODER_CPR          (0.00038349f)//2PI/16384 16384为AS5047旋转一圈的计数值
#define MOTOR_PAIRS          (7.0f)//电机极对数

#define AMPLIFICATION_FACTOR (5.7f)  // 假设放大倍数为10  
#define SAMPLING_RESISTANCE  (0.025)  // 假设采样电阻为100欧姆
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

#define CIRCLE_OUT_MAX      (12.0f) //电流环输出最大值
#define CIRCLE_OUT_MIN      (-12.0f)
#define SPEED_OUT_MAX       (12.0f) //速度环输出最大值
#define SPEED_OUT_MIN       (-12.0f)


void user_board_init(void);

#endif
