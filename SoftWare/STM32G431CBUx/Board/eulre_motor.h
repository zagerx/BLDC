/****************************************************************************
Filename: eulre_motor.h
Functions list:
Notes:
Copyright (c) 2022, Syrius Robotics
All rights reserved.
****************************************************************************/


#ifndef __EULRE_MOTOR__H
#define __EULRE_MOTOR__H

#ifdef __cplusplus
extern "C"{
#endif

/**************************************************************************************************
                                        电机电气参数
**************************************************************************************************/
#define MOTOR_PAIRS                          (2.0f)//电机极对数

/**************************************************************************************************
                                        电机编码器参数
**************************************************************************************************/
// #define HALL_ENABLE_CAIRLBE               (1)
#define HALL_UPDATE_PERIOD                   (0.0001f)
#define HALL_POSITIVE_OFFSET                 (-0.02)
#define HALL_NEGATIVE_OFFSET                 (-0.1f)
#define MEC_ANGLE_OFFSET                     (0.0F)//(-0.809f)
#define PLL_KP                               (80.50f)
#define PLL_KI                               (1.2f)
#define OMEGTOTHETA                          (0.0001f)     

#define ENCODER_TYPE_SENSORLESS              (0)
#define ENCODER_TYPE_ABS                     (1)
#define ENCODER_TYPE_HALL                    (2)
#define ENCODER_TYPE                         ENCODER_TYPE_HALL

#define SCETION_6_BASEANGLE                  (2.770f)
#define SCETION_4_BASEANGLE                  (3.847f)
#define SCETION_5_BASEANGLE                  (4.920f)
#define SCETION_1_BASEANGLE                  (5.960f)
#define SCETION_3_BASEANGLE                  (0.680f)
#define SCETION_2_BASEANGLE                  (1.730f)

#define SCETION_6_ANGLEDIFF                  (1.040f)
#define SCETION_4_ANGLEDIFF                  (1.077f)
#define SCETION_5_ANGLEDIFF                  (1.073f)
#define SCETION_1_ANGLEDIFF                  (1.040f)
#define SCETION_3_ANGLEDIFF                  (1.003f)
#define SCETION_2_ANGLEDIFF                  (1.050f)

#ifdef __cplusplus
}
#endif

#endif	/* __EULRE_MOTOR__H */
