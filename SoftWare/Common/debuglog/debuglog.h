#ifndef __DEBUGLOG__H
#define __DEBUGLOG__H
#include "./debuglog_cfg.h"

#ifndef DEBUG_MODE_UART
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
    __attribute__((weak)) char readline_fromPC(char* buffer, int bufferSize);
 
#else
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
#endif
__attribute__((weak)) void USER_DEBUG_INIT(void);
#endif

