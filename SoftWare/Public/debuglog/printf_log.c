#include "debuglog_cfg.h"
#if (DEBUG_SW == DEBUG_UART)
    int _write(int file, char *data, int len)
    {
        HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, (uint8_t*)data, len, 1000);
        return (status == HAL_OK ? len : 0);
    }
    __attribute__((weak)) void User_Printf_Init(void)
    {
    }
#elif (DEBUG_SW == DEBUG_RTT)
    __attribute__((weak)) void User_Printf_Init(void)
    {
        SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);            
    }
#else
    __attribute__((weak)) void User_Printf_Init(void)
    {
    }
#endif



// #include "debuglog_.h"
// #ifdef DEBUG_SW
//     #ifndef DEBUG_SW_RTT
//         #include "usart.h"
//         int _write(int file, char *data, int len)
//                 {
//                 HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, (uint8_t*)data, len, 1000);
//                 return (status == HAL_OK ? len : 0);
//         }
//     #else
//         void User_Printf_Init(void)
//         {
//             SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);            
//         }
//     #endif
// #endif





