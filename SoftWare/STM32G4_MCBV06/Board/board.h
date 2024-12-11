#ifndef __BOARD__H
#define __BOARD__H

/*--------------------电控软件---------------------*/
//开环部分
#define OPENLOOP_DEBUG_TOTAL_Te             (-0.04f)
#define OPENLOOP_DEBUG_STEP_THETA           (-0.00008f)
//闭环部分
#define CURRMENT_PERIOD      (0.0001f)//电流环周期
#define SPEED_UPDATE_PERIOD  (0.002f)//速度更新周期
#define CURRMENTLOOP_KP             (0.08f)         
#define CURRMENTLOOP_KI             (0.001f)
#define SPEEDLOOP_KP                (0.18f)
#define SPEEDLOOP_KI                (0.008f)
#define POSLOOP_KP                  (0.1f)
#define POSLOOP_KI                  (0.1f)
/*-----------------编码器类型选择-------------------*/
#define ENCODER_TYPE_SENSORLESS              (0)
#define ENCODER_TYPE_ABS                     (1)
#define ENCODER_TYPE_HALL                    (2)
#define ENCODER_TYPE_HALL_ABZ                (3)
#define ENCODER_TYPE                         ENCODER_TYPE_HALL_ABZ

#if (ENCODER_TYPE_HALL==ENCODER_TYPE_HALL || ENCODER_TYPE == ENCODER_TYPE_HALL_ABZ)
    #define ABZ_ENCODER_LINES_HALF           (4096)
    #define ABZ_ENCODER_LINES           (8192)
    #define ABZ_ENCODER_RESOLUTION      (0.023f)// 2*pi/4096*motor_pairs  
    #define HALL_UPDATE_PERIOD   (0.0001f)
    #define HALL_POSITIVE_OFFSET (-0.3f)
    #define HALL_NEGATIVE_OFFSET (-0.2f)
    #define PLL_KP               (10.0f)
    #define PLL_KI               (0.02f)
    #define OMEGTOTHETA          (CURRMENT_PERIOD)         

    /*HALL基准角度 D轴强拖 0.1f*/
    // #define SCETION_6_BASEANGLE   (5.840f)
    // #define SCETION_4_BASEANGLE   (0.642f)
    // #define SCETION_5_BASEANGLE   (1.565f)
    // #define SCETION_1_BASEANGLE   (2.686f)
    // #define SCETION_3_BASEANGLE   (3.765f)
    // #define SCETION_2_BASEANGLE   (4.737f) 
    #define SCETION_6_BASEANGLE   (5.005f)
    #define SCETION_4_BASEANGLE   (6.057f)
    #define SCETION_5_BASEANGLE   (0.648f)
    #define SCETION_1_BASEANGLE   (1.854f)
    #define SCETION_3_BASEANGLE   (2.898f)
    #define SCETION_2_BASEANGLE   (3.815f)       
#endif

/*----------------------硬件相关----------------------*/
#define AMPLIFICATION_FACTOR (5.36f)  // 假设放大倍数为10  
#define SAMPLING_RESISTANCE  (0.005)  // 假设采样电阻为5mR欧姆
#define CIRCLE_MAX_VAL       (24.0f)
#define D_MAX_VAL            (12.0f)
#define D_MIN_VAL            -D_MAX_VAL
#define Q_MAX_VAL            D_MAX_VAL
#define Q_MIN_VAL            -Q_MAX_VAL
#define CIRCLE_OUT_MAX      (12.0f) //电流环输出最大值
#define CIRCLE_OUT_MIN      (-12.0f)
#define SPEED_OUT_MAX       (12.0f) //速度环输出最大值
#define SPEED_OUT_MIN       (-12.0f)

/*---------------------电机本体----------------------- */
#define MOTOR_PAIRS          (15.0f)//电机极对数

#endif
