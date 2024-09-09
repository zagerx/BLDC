#ifndef __BOARD__H
#define __BOARD__H

#define CURRMENT_PERIOD      (0.0001f)
#define SPEED_UPDATE_PERIOD  (0.002f)
#define SPEED_UPDATE_COUNT   (20)       //0.002/CURRMENT_PERIOD
#define MEC_ANGLE_OFFSET     (0.0056f)

#define MOTOR_PAIRS          (7.0f)
#define ENCODER_CPR          (0.00038349f)//2PI/16384 16384为AS5047旋转一圈的计数值

#define CIRCLE_MAX_VAL       (24.0f)
#define D_MAX_VAL            (12.0f)
#define D_MIN_VAL            -D_MAX_VAL
#define Q_MAX_VAL            D_MAX_VAL
#define Q_MIN_VAL            -Q_MAX_VAL

#endif
