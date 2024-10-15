#ifndef __BOARD__H
#define __BOARD__H

#define TOTAL_DISTANCE       (_2PI)//自开环距离
#define TOTAL_TIME           (2.0f)//自开环时间
#define TOTAL_OMEGA          (TOTAL_DISTANCE/TOTAL_TIME)//自开环角速度

#define CURRMENT_PERIOD      (0.0001f)//电流环周期
#define SPEED_UPDATE_PERIOD  (0.002f)//速度更新周期
#define SPEED_UPDATE_COUNT   (20)       //0.002/CURRMENT_PERIOD

#define MEC_ANGLE_OFFSET     (0.0056f)//编码器偏移值 AS5047:(0.0056f)
#define ENCODER_CPR          (0.00038349f)//2PI/16384 16384为AS5047旋转一圈的计数值
#define MOTOR_PAIRS          (14.0f)//电机极对数
#define AMPLIFICATION_FACTOR (5.7f)  // 假设放大倍数为10  
#define SAMPLING_RESISTANCE  (0.025)  // 假设采样电阻为100欧姆

#define CIRCLE_MAX_VAL       (24.0f)
#define D_MAX_VAL            (12.0f)
#define D_MIN_VAL            -D_MAX_VAL
#define Q_MAX_VAL            D_MAX_VAL
#define Q_MIN_VAL            -Q_MAX_VAL

#define CIRCLE_OUT_MAX      (12.0f) //电流环输出最大值
#define CIRCLE_OUT_MIN      (-12.0f)
#define SPEED_OUT_MAX       (12.0f) //速度环输出最大值
#define SPEED_OUT_MIN       (-12.0f)

#endif
