#ifndef __BOARD__H
#define __BOARD__H


/*-----------------传感器类型选择-------------------*/
#define ENCODER_TYPE_SENSORLESS              (0)
#define ENCODER_TYPE_ABS                     (1)
#define ENCODER_TYPE_HALL                    (2)
#define ENCODER_TYPE                         ENCODER_TYPE_HALL
#ifndef ENCODER_TYPE_HALL    
#else/*------------霍尔传感器部分-------------------*/
    #define HALL_UPDATE_PERIOD   (0.0001f)
    #define HALL_POSITIVE_OFFSET (-0.3f)
    #define HALL_NEGATIVE_OFFSET (-0.2f)
    #define PLL_KP 2.0f
    #define PLL_KI 0.01f
    #define OMEGTOTHETA 0.0002f         
    //D轴强拖 0.04f
    // #define SCETION_6_BASEANGLE   (4.924f)
    // #define SCETION_4_BASEANGLE   (5.936f)
    // #define SCETION_5_BASEANGLE   (0.573f)
    // #define SCETION_1_BASEANGLE   (1.767f)
    // #define SCETION_3_BASEANGLE   (2.764f)
    // #define SCETION_2_BASEANGLE   (3.713f)
    //D轴强拖 0.1f
    #define SCETION_6_BASEANGLE   (5.005f)
    #define SCETION_4_BASEANGLE   (6.057f)
    #define SCETION_5_BASEANGLE   (0.648f)
    #define SCETION_1_BASEANGLE   (1.854f)
    #define SCETION_3_BASEANGLE   (2.898f)
    #define SCETION_2_BASEANGLE   (3.815f)   
    //D轴强拖 0.2f
    // #define SCETION_6_BASEANGLE   (4.901f)
    // #define SCETION_4_BASEANGLE   (5.960f)
    // #define SCETION_5_BASEANGLE   (0.590f)
    // #define SCETION_1_BASEANGLE   (1.743f)
    // #define SCETION_3_BASEANGLE   (2.791f)
    // #define SCETION_2_BASEANGLE   (3.732f)   
    //D轴强拖 0.2f 0.001f
    // #define SCETION_6_BASEANGLE   (5.237f)
    // #define SCETION_4_BASEANGLE   (6.174f)
    // #define SCETION_5_BASEANGLE   (0.837f)
    // #define SCETION_1_BASEANGLE   (1.981f)
    // #define SCETION_3_BASEANGLE   (3.030f)
    // #define SCETION_2_BASEANGLE   (3.995f)   

#endif


//电控  电机相关
#define MOTOR_PAIRS          (14.0f)//电机极对数

/*------电控 软件相关------*/
//开环部分
#define OPENLOOP_DEBUG_TOTAL_Te             (0.04f)
#define OPENLOOP_DEBUG_STEP_THETA           (-0.0004f)
//闭环部分
#define CURRMENT_PERIOD      (0.0001f)//电流环周期
#define SPEED_UPDATE_PERIOD  (0.002f)//速度更新周期
#define SPEED_UPDATE_COUNT   (20)       //0.002/CURRMENT_PERIOD

//电控 硬件相关
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
