#ifndef __PRINTF_LOG__H
#define __PRINTF_LOG__H

#ifdef	ENABLE_DEBUG
    #ifdef ENABLE_DEBUG_RTT
        #define USER_DEBUG(format,...)	SEGGER_RTT_printf(0,format,##__VA_ARGS__)
    #else
        #include "stdio.h"
        #define USER_DEBUG(format,...)	printf(format,##__VA_ARGS__)
    #endif
#else
	#define	USER_DEBUG(format,...)
#endif









#endif
