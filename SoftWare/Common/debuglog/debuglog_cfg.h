#ifndef __DEBUGLOG_CFG__H
#define __DEBUGLOG_CFG__H
/*
**  默认使用RTT打印
*/
#define DEBUG_INFO_NORMAL    (1)
#define DEBUG_INFO_WARING    (2)
#define DEBUG_INFO_ERR       (3)
#define DEBUG_LEVEL          (DEBUG_INFO_NORMAL)


#ifndef DEBUG_MODE_UART
    #include "SEGGER_RTT.h"
#else
    #include "usart.h"
    #include "stdio.h"
#endif


#endif
