#ifndef __DEBUGLOG_CFG__H
#define __DEBUGLOG_CFG__H

#define DEBUG_INFO_NORMAL    (1)
#define DEBUG_INFO_WARING    (2)
#define DEBUG_INFO_ERR       (3)
#define DEBUG_LEVEL          (DEBUG_INFO_NORMAL)

#define DEBUG_DISABLE  (0)
#define DEBUG_RTT      (1)
#define DEBUG_UART     (2)

#define DEBUG_SW   DEBUG_UART

    #if (DEBUG_SW == DEBUG_UART)
        #include "usart.h"
        #include "stdio.h"
    #elif (DEBUG_SW == DEBUG_RTT)
        #include "SEGGER_RTT.h"
    #else
    #endif
#endif
