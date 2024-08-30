#include "debuglog_cfg.h"

#ifndef DEBUG_MODE_UART
    #define DOWN_BUFFER_NAME "DownBuffer"  
    #define DOWN_BUFFER_SIZE 1024  
    unsigned char downBuffer[DOWN_BUFFER_SIZE]; 
    __attribute__((weak)) void USER_DEBUG_INIT(void)
    {
        SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
        SEGGER_RTT_ConfigDownBuffer(0, DOWN_BUFFER_NAME, (void*)downBuffer, DOWN_BUFFER_SIZE, SEGGER_RTT_MODE_NO_BLOCK_SKIP);  
    }

    /*----------------RTT解析命令--------------------------*/
    #include "string.h"
    __attribute__((weak)) char readline_fromPC(char* buffer, int bufferSize) {  
        int index = 0; // 缓冲区中的当前位置  
        int c; // 读取的字符  

        // 清空缓冲区  
        memset(buffer, 0, bufferSize);  
    
        // 循环读取字符，直到遇到换行符或缓冲区满  
        while (SEGGER_RTT_HasKey()) {  
            c = SEGGER_RTT_GetKey(); // 读取一个字符  
            if (c == '\n' || c == '\r') { // 检查换行符或回车符（取决于你的系统配置）  
                break; // 如果是换行符，则结束循环  
            }
            if (index < bufferSize - 1) { // 确保不会溢出缓冲区  
                buffer[index++] = (char)c; // 将字符添加到缓冲区并递增索引  
            } else {  
                // 缓冲区已满，可以选择处理溢出情况（例如通过发送错误消息）  
                // 在这里我们简单地退出循环，忽略额外的字符
                break;  
            }
        }
        return index;
    } 
#else

    __attribute__((weak)) void USER_DEBUG_INIT(void)
    {
        return;
    }
    __attribute__((weak)) char readline_fromPC(char* buffer, int bufferSize) 
    {
        return 0;
    }

#endif