#ifndef __DEBUGLOG__H
#define __DEBUGLOG__H
#include "./debuglog_cfg.h"
#if (DEBUG_SW == DEBUG_UART)
    #define USER_DEBUG_NORMAL(format,...)	printf(format,##__VA_ARGS__)
    #if(DEBUG_INFO_NORMAL >= DEBUG_LEVEL)
        #define USER_DEBUG_NORMAL(format,...)	           printf(format,##__VA_ARGS__)
        #define USER_DEBUG_WARING(format,...)	           printf(format,##__VA_ARGS__)
        #define USER_DEBUG_ERR(format,...)	               printf(format,##__VA_ARGS__)
    #elif(DEBUG_INFO_WARING >= DEBUG_LEVEL)
        #define USER_DEBUG_NORMAL(format,...)	
        #define USER_DEBUG_WARING(format,...)	           printf(format,##__VA_ARGS__)
        #define USER_DEBUG_ERR(format,...)	               printf(format,##__VA_ARGS__)
    #elif(DEBUG_INFO_ERR >= DEBUG_LEVEL)
        #define USER_DEBUG_NORMAL(format,...)	
        #define USER_DEBUG_WARING(format,...)       
        #define USER_DEBUG_ERR(format,...)	               printf(format,##__VA_ARGS__)
    #else
        #define USER_DEBUG_NORMAL(format,...)
    #endif    
#elif (DEBUG_SW == DEBUG_RTT)
    #if(DEBUG_INFO_NORMAL >= DEBUG_LEVEL)
        #define USER_DEBUG_NORMAL(format,...)	           SEGGER_RTT_printf(0,format,##__VA_ARGS__)
        #define USER_DEBUG_WARING(format,...)	           SEGGER_RTT_printf(0,format,##__VA_ARGS__)
        #define USER_DEBUG_ERR(format,...)	               SEGGER_RTT_printf(0,format,##__VA_ARGS__)
    #elif(DEBUG_INFO_WARING >= DEBUG_LEVEL)
        #define USER_DEBUG_NORMAL(format,...)	
        #define USER_DEBUG_WARING(format,...)	           SEGGER_RTT_printf(0,format,##__VA_ARGS__)
        #define USER_DEBUG_ERR(format,...)	               SEGGER_RTT_printf(0,format,##__VA_ARGS__)
    #elif(DEBUG_INFO_ERR >= DEBUG_LEVEL)
        #define USER_DEBUG_NORMAL(format,...)	
        #define USER_DEBUG_WARING(format,...)       
        #define USER_DEBUG_ERR(format,...)	               SEGGER_RTT_printf(0,format,##__VA_ARGS__)
    #else
        #define USER_DEBUG_NORMAL(format,...)
    #endif
#else
    #define USER_DEBUG_NORMAL(format,...)
    #define USER_DEBUG_WARING(format,...)
    #define USER_DEBUG_ERR(format,...)
#endif

__attribute__((weak)) void User_Printf_Init(void);


// #ifdef	ENABLE_DEBUG
//     #ifdef ENABLE_DEBUG_RTT
//         #include "SEGGER_RTT.h"
//         #if(DEBUG_INFO_NORMAL >= DEBUG_LEVEL)
//             #define USER_DEBUG_NORMAL(format,...)	           SEGGER_RTT_printf(0,format,##__VA_ARGS__)
//             #define USER_DEBUG_WARING(format,...)	           SEGGER_RTT_printf(0,format,##__VA_ARGS__)
//             #define USER_DEBUG_ERR(format,...)	               SEGGER_RTT_printf(0,format,##__VA_ARGS__)
//         #elif(DEBUG_INFO_WARING >= DEBUG_LEVEL)
//             #define USER_DEBUG_NORMAL(format,...)	
//             #define USER_DEBUG_WARING(format,...)	           SEGGER_RTT_printf(0,format,##__VA_ARGS__)
//             #define USER_DEBUG_ERR(format,...)	               SEGGER_RTT_printf(0,format,##__VA_ARGS__)
//         #elif(DEBUG_INFO_ERR >= DEBUG_LEVEL)
//             #define USER_DEBUG_NORMAL(format,...)	
//             #define USER_DEBUG_WARING(format,...)       
//             #define USER_DEBUG_ERR(format,...)	               SEGGER_RTT_printf(0,format,##__VA_ARGS__)
//         #else
//             #define USER_DEBUG_NORMAL(format,...)
//         #endif
//         void User_Printf_Init(void);
//     #else
//         #include "stdio.h"
//         #define USER_DEBUG_NORMAL(format,...)	printf(format,##__VA_ARGS__)
//     #endif
// #else
// 	#define	USER_DEBUG_NORMAL(format,...)
// #endif
#endif

