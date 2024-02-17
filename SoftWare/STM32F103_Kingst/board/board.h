#ifndef __BOARD__H
#define __BOARD__H

#define KINGST_BOARD
// #define UART_PRINTF_MODE   (1)

/*公共模块*/
#define DEBUG_DISABLE                  (0)
#define DEBUG_RTT                      (1)
#define DEBUG_UART                     (2)
#define DEBUG_SW                       (DEBUG_UART)

#define USE_IPC_ENABLE                 (0)
#define USE_FSM_ENABLE                 (0)
#define USE_GPIO_ENABLE                (0)
#define USE_ASSERT_ENABLE              (0)
#define USE_IQMATH_ENABLE              (0)
#define USE_METHOD_ENABLE              (0)
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
}ENUM_SENSOR;
#else
typedef enum{
    SENSOR_NONE = -1,
    SENSOR_NUMBER
}ENUM_SENSOR;
#endif



#endif
