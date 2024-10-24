#ifndef __BOARD__H
#define __BOARD__H

#define BOARD_STM32H723

/*公共模块*/
#define DEBUG_DISABLE                  (0)
#define DEBUG_RTT                      (1)
#define DEBUG_UART                     (2)
#define DEBUG_SW                       (DEBUG_RTT)

#define USE_IPC_ENABLE                 (0)
#define USE_FSM_ENABLE                 (0)
#define USE_GPIO_ENABLE                (0)
#define USE_ASSERT_ENABLE              (0)
#define USE_IQMATH_ENABLE              (1)
#define USE_METHOD_ENABLE              (1)
#define USE_DEBUGLOG_ENABLE            (1)
#define USE_ANALOGI2C_ENABLE           (0)
#define USE_DATASTRUCTURES_ENABLE      (0)

/*特定模块*/

/*传感器模块*/
#define SENSOR_MODE_EN (1)
    #ifdef SENSOR_MODE_EN
    /*定义传感器数量*/
    typedef enum{
        SENSOR_NONE = -1,
        SENSOR_01, //角度传感器
        SENSOR_02, //电流检测传感器
        SENSOR_NUMBER
    }sens_type_idx_t;
    #else
    typedef enum{
        SENSOR_NONE = -1,
        SENSOR_NUMBER
    }sens_type_idx_t;
#endif

/*Motor Ctrl Mode*/
#define BOARD_MOTORCTRL_MODE
    #ifdef BOARD_MOTORCTRL_MODE
        #define A_ADCCHANNL_OFFSET          (32768)
        #define B_ADCCHANNL_OFFSET          (32768)
        #define C_ADCCHANNL_OFFSET          (32900)
        #define APHASE_SHUNT                 0.05F
        #define BPHASE_SHUNT                 0.05F
        #define CPHASE_SHUNT                 0.06F
        #define MOTOR_PAIR                  4.0f

        /*Ia = (float)ADC1*(float)(3.3f/4096.0f/RA_S/BETA_);*/
        #define A_VOLITETOCURRENT_FACTOR    (0.002f)
        #define B_VOLITETOCURRENT_FACTOR    (0.002f)
        #define C_VOLITETOCURRENT_FACTOR    (0.002f)

        #define T_PWM                       (100.0f)
        #define U_DC                        (24.0f)
        #define F_PWM                       (float)(1.0f/T_PWM)
        #define T_UDC                       (float)(T_PWM/U_DC)

        #define ANGLE_COMPENSATA            (-1.3f - 3.1415926/2.0f)

        #define CIRCLE_MAX_VAL               16.0f
        #define D_MAX_VAL                    CIRCLE_MAX_VAL*0.95f
        #define D_MIN_VAL                    -D_MAX_VAL
        #define Q_MAX_VAL                    D_MAX_VAL
        #define Q_MIN_VAL                    -Q_MAX_VAL
    #endif
#endif
