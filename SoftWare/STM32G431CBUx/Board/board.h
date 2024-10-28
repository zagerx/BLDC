#ifndef __BOARD__H
#define __BOARD__H

#define OPENLOOP_DEBUG_TOTAL_Te              (0.5f)
#define OPENLOOP_DEBUG_STEP_THETA            (0.001f)
#define MOTOR_PAIRS                          (2.0f)//电机极对数
#define CURRMENT_PERIOD                      (0.0001f)//电流环周期

#ifndef ENCODER_TYPE_HALL    


#else
//D轴强拖
#define SCETION_6_BASEANGLE   (3.0891f)
#define SCETION_4_BASEANGLE   (4.1670f)
#define SCETION_5_BASEANGLE   (5.191f)
#define SCETION_1_BASEANGLE   (6.227f)
#define SCETION_3_BASEANGLE   (1.024f)
#define SCETION_2_BASEANGLE   (2.050f)

//Q轴强拖
// #define SCETION_6_BASEANGLE   (1.390f)
// #define SCETION_4_BASEANGLE   (2.334f)
// #define SCETION_5_BASEANGLE   (3.446f)
// #define SCETION_1_BASEANGLE   (4.527f)
// #define SCETION_3_BASEANGLE   (5.418f)
// #define SCETION_2_BASEANGLE   (0.331f)
#endif


#define CIRCLE_MAX_VAL       (24.0f)
#define D_MAX_VAL            (12.0f)
#define D_MIN_VAL            -D_MAX_VAL
#define Q_MAX_VAL            D_MAX_VAL
#define Q_MIN_VAL            -Q_MAX_VAL

#define CIRCLE_OUT_MAX      (12.0f) //电流环输出最大值
#define CIRCLE_OUT_MIN      (-12.0f)
#define SPEED_OUT_MAX       (12.0f) //速度环输出最大值
#define SPEED_OUT_MIN       (-12.0f)



void Board_init(void);

#endif
