#ifndef __BOARD__H
#define __BOARD__H

#define BOARD_STM32G431

/*公共模块*/
#define DEBUG_DISABLE                  (0)
#define DEBUG_RTT                      (1)
#define DEBUG_UART                     (2)
#define DEBUG_SW                       (DEBUG_RTT)

#define USE_FSM_ENABLE                 (0)
#define USE_GPIO_ENABLE                (0)
#define USE_ASSERT_ENABLE              (0)
#define USE_IQMATH_ENABLE              (1)
#define USE_METHOD_ENABLE              (1)
#define USE_DEBUGLOG_ENABLE            (1)
#define USE_ANALOGI2C_ENABLE           (0)
#define USE_DATASTRUCTURES_ENABLE      (1)

/*
IPC模块
31                        3 2 1                0
                        当前电机状态         按键被按下
                        0:init
                        1:运行
                        2:停止
                        3:空闲                   
*/
#define USE_IPC_ENABLE                 (1)
typedef enum{
    EN_EVENT_01 = 0,
}EN_EVENT;
typedef enum{
    EN_EVENT_ONEBIT = 1,
    EN_EVENT_TWOBIT,
    EN_EVENT_THREEBIT
}EN_EVENT_SIZE;

typedef enum{
    PUBLIC_DATA_START_INDEX,
    PUBLIC_DATA_IQ_TARGET,
    PUBLIC_DATA_IA,
    PUBLIC_DATA_IB,    
    PUBLIC_DATA_IC,
    PUBLIC_DATA_IALPHA,
    PUBLIC_DATA_IBETA,
    PUBLIC_DATA_ID,
    PUBLIC_DATA_IQ,
    PUBLIC_DATA_TCMP1,
    PUBLIC_DATA_TCMP2,
    PUBLIC_DATA_TCMP3,
    PUBLIC_DATA_TEMP0,
    PUBLIC_DATA_TEMP1,    
    PUBLIC_DATA_STOP_INDEX,
}E_PUBLIC_DATA;



/*传感器模块*/
#define SENSOR_MODE_EN (1)
#ifdef SENSOR_MODE_EN
/*定义传感器数量*/
typedef enum{
    SENSOR_NONE = -1,
    SENSOR_01, //角度传感器
    SENSOR_NUMBER
}ENUM_SENSOR;
#else
typedef enum{
    SENSOR_NONE = -1,
    SENSOR_NUMBER
}ENUM_SENSOR;

#endif


#define BOARD_MOTORCTRL_MODE

#ifdef BOARD_MOTORCTRL_MODE
        #define A_ADCCHANNL_OFFSET          (2121)
        #define B_ADCCHANNL_OFFSET          (2138)
        #define C_ADCCHANNL_OFFSET          (2148)
        #define APHASE_SHUNT                 0.05F
        #define BPHASE_SHUNT                 0.05F
        #define CPHASE_SHUNT                 0.06F
        #define MOTOR_PAIR                  2.0f

        /*Ia = (float)ADC1*(float)(3.3f/4096.0f/RA_S/BETA_);*/
        #define A_VOLITETOCURRENT_FACTOR    (0.31f)
        #define B_VOLITETOCURRENT_FACTOR    (0.35f)
        #define C_VOLITETOCURRENT_FACTOR    (0.348999f)

        #define T_PWM                       (100.0f)
        #define U_DC                        (24.0f)
        #define F_PWM                       (float)(1.0f/T_PWM)
        #define T_UDC                       (float)(T_PWM/U_DC)
        #define ANGLE_COMPENSATA           (0.1f)

    #define CIRCLE_MAX_VAL               (24.0f)
    #define D_MAX_VAL                    12.0f//CIRCLE_MAX_VAL*sqrt(3.0f)/3.0f
    #define D_MIN_VAL                    -D_MAX_VAL
    #define Q_MAX_VAL                    D_MAX_VAL
    #define Q_MIN_VAL                    -Q_MAX_VAL
#endif

#endif
