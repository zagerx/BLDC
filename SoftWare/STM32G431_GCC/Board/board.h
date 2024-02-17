#ifndef __BOARD__H
#define __BOARD__H

#define STM32G431GCC_BOARD

/*公共模块*/
#define DEBUG_DISABLE                  (0)
#define DEBUG_RTT                      (1)
#define DEBUG_UART                     (2)
#define DEBUG_SW                       (DEBUG_RTT)

#define USE_IPC_ENABLE                 (1)
#define USE_FSM_ENABLE                 (0)
#define USE_GPIO_ENABLE                (0)
#define USE_ASSERT_ENABLE              (0)
#define USE_IQMATH_ENABLE              (1)
#define USE_METHOD_ENABLE              (1)
#define USE_DEBUGLOG_ENABLE            (1)
#define USE_ANALOGI2C_ENABLE           (0)
#define USE_DATASTRUCTURES_ENABLE      (1)

/*特定模块*/

/*传感器模块*/
#define SENSOR_MODE_EN (1)
#ifdef SENSOR_MODE_EN
/*定义传感器数量*/
typedef enum{
    SENSOR_NONE = -1,
    SENSOR_01, //角度传感器
    SENSOR_02, 
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
    #define AD_OFFSET                   2116.0f
    #define RA_S                        0.05F
    #define RB_S                        0.05F
    #define RC_S                        0.06F
    #define MOTOR_PAIR                  2.0f
    #define BETA_                       5.7F
    #define FACTOR                      (1.0f)
    #define T_PWM                       (100.0f)*FACTOR
    #define U_DC                        (24.0f)*FACTOR
    #define F_PWM                       (float)(1.0f/T_PWM)*FACTOR
    #define T_UDC                       (float)(T_PWM/U_DC)*FACTOR

    #define CIRCLE_MAX_VAL               16.0f
    #define D_MAX_VAL                    CIRCLE_MAX_VAL*0.95f
    #define D_MIN_VAL                    -D_MAX_VAL
    #define Q_MAX_VAL                    D_MAX_VAL
    #define Q_MIN_VAL                    -Q_MAX_VAL
#endif

#endif
