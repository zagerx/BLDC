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
#define MOTOR_PAIRS                          (4.0f)//电机极对数

/**************************************************************************************************
                                        电机编码器参数
**************************************************************************************************/
#define ENCODER_TYPE_SENSORLESS              (0)
#define ENCODER_TYPE_ABS                     (1)
#define ENCODER_TYPE_HALL                    (2)
#define ENCODER_TYPE_HALL_ABZ                (3)
#define ENCODER_TYPE                         ENCODER_TYPE_HALL_ABZ//编码器类型

#if (ENCODER_TYPE_HALL==ENCODER_TYPE_HALL || ENCODER_TYPE == ENCODER_TYPE_HALL_ABZ)
    #define MEC_ANGLE_OFFSET                (0.0F)//编码器偏移(适配ABS编码器，防止编译报错)TODO

    #define OPENLOOP_DEBUG_TOTAL_Te             (0.01f)                   //电机自开环力矩
    #define OPENLOOP_DEBUG_STEP_THETA           (0.0002f)                 //电机自开环自给角度

    #define ABZ_ENCODER_LINES_HALF          (4096)
    #define ABZ_ENCODER_LINES               (8192)//编码器线数
    #define ABZ_ENCODER_RESOLUTION          (0.003067f)//编码器分辨率 2*pi/ABZ_ENCODER_LINES*MOTOR_PAIRS

    #define HALL_UPDATE_PERIOD              (0.0001f)//编码器更新周期
    #define HALL_POSITIVE_OFFSET            (0.0f)
    #define HALL_NEGATIVE_OFFSET            (0.0f)
    #define PLL_KP                          (80.0f)
    #define PLL_KI                          (0.02f)
    #define OMEGTOTHETA                     (0.0001f)//TODO
    /*HALL基准角度 D轴强拖 VBUS51.54f   0.01f 0.0002f*/
    /*基准角度待测试*/
    #define SCETION_6_BASEANGLE             (0.582f)
    #define SCETION_4_BASEANGLE             (1.610f)
    #define SCETION_5_BASEANGLE             (2.643f)
    #define SCETION_1_BASEANGLE             (3.695f)
    #define SCETION_3_BASEANGLE             (4.690f)
    #define SCETION_2_BASEANGLE             (5.799f)
#endif

#ifdef __cplusplus
}
#endif

#endif	/* __BORADO_MOTOR__H */
