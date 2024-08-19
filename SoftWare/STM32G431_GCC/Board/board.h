#ifndef __BOARD__H
#define __BOARD__H

#define BOARD_STM32G431

/*公共模块*/
#define DEBUG_DISABLE                  (0)
#define DEBUG_RTT                      (1)
#define DEBUG_UART                     (2)
#define DEBUG_SW                       (DEBUG_RTT)

/*传感器模块*/
#define SENSOR_MODE_EN (1)
#ifdef SENSOR_MODE_EN
/*定义传感器数量*/

#else
typedef enum{
    SENSOR_NONE = -1,
    SENSOR_NUMBER
}sens_type_idx_t;

#endif


#define BOARD_MOTORCTRL_MODE

#ifdef BOARD_MOTORCTRL_MODE
    #define A_ADCCHANNL_OFFSET          (2121)
    #define B_ADCCHANNL_OFFSET          (2138)
    #define C_ADCCHANNL_OFFSET          (2148)
    #define APHASE_SHUNT                 0.05F
    #define BPHASE_SHUNT                 0.05F
    #define CPHASE_SHUNT                 0.06F
    #define MOTOR_PAIR                   7.0f

    /*Ia = (float)ADC1*(float)(3.3f/4096.0f/RA_S/BETA_);*/
    #define A_VOLITETOCURRENT_FACTOR    (0.31f)
    #define B_VOLITETOCURRENT_FACTOR    (0.35f)
    #define C_VOLITETOCURRENT_FACTOR    (0.348999f)

    #define T_PWM                       (100.0f)
    #define U_DC                        (24.0f)
    #define F_PWM                       (float)(1.0f/T_PWM)
    #define T_UDC                       (float)(T_PWM/U_DC)
    #define ANGLE_COMPENSATA            (0.1f)

    #define CIRCLE_MAX_VAL               (24.0f)
    #define D_MAX_VAL                    12.0f//4.0f//CIRCLE_MAX_VAL*sqrt(3.0f)/3.0f
    #define D_MIN_VAL                    -D_MAX_VAL
    #define Q_MAX_VAL                    D_MAX_VAL
    #define Q_MIN_VAL                    -Q_MAX_VAL
#endif




// void board_init(void);


#endif
