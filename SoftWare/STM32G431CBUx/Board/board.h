#ifndef __BOARD__H
#define __BOARD__H

#define OPENLOOP_DEBUG_TOTAL_Te              (0.008f)
#define OPENLOOP_DEBUG_STEP_THETA            (0.0002f)
#define MOTOR_PAIRS                          (14.0f)//电机极对数
#define CURRMENT_PERIOD                      (0.0001f)//电流环周期



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
