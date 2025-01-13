/****************************************************************************
Filename: borado_motor.h
Functions list:
Notes:
Copyright (c) 2022, Syrius Robotics
All rights reserved.
****************************************************************************/


#ifndef __BORADO_MOTOR__H
#define __BORADO_MOTOR__H

#ifdef __cplusplus
extern "C"{
#endif

/**************************************************************************************************
                                        电机参数
**************************************************************************************************/
#define MOTOR_PAIRS                          (5.0f)//电机极对数

/**************************************************************************************************
                                        电机编码器参数
**************************************************************************************************/
#define ABZ_ENCODER_LINES_HALF               (2500)
#define ABZ_ENCODER_LINES                    (5000)
#define ABZ_ENCODER_RESOLUTION               (0.00628f)// 2*pi/5000

#define ENCODER_TYPE_SENSORLESS              (0)
#define ENCODER_TYPE_ABS                     (1)
#define ENCODER_TYPE_HALL                    (2)
#define ENCODER_TYPE_HALL_ABZ                (3)
#define ENCODER_TYPE                         ENCODER_TYPE_HALL_ABZ//编码器类型

#define MEC_ANGLE_OFFSET                     (0.0F)//编码器偏移


#if (ENCODER_TYPE_HALL==ENCODER_TYPE_HALL || ENCODER_TYPE == ENCODER_TYPE_HALL_ABZ)
    #define HALL_UPDATE_PERIOD              (0.0001f)
    #define HALL_POSITIVE_OFFSET            (+0.0f+1.0f)
    #define HALL_NEGATIVE_OFFSET            (3.18f-1.0f)
    #define PLL_KP                          (80.0f)
    #define PLL_KI                          (0.02f)
    #define OMEGTOTHETA                     (0.0001f)         
    /*HALL基准角度 D轴强拖 0.1f*/
    #define SCETION_6_BASEANGLE             (3.610f)
    #define SCETION_4_BASEANGLE             (4.703f)
    #define SCETION_5_BASEANGLE             (5.763f)
    #define SCETION_1_BASEANGLE             (0.473f)
    #define SCETION_3_BASEANGLE             (1.564f)
    #define SCETION_2_BASEANGLE             (2.617f)     
#endif

#ifdef __cplusplus
}
#endif

#endif	/* __BORADO_MOTOR__H */
