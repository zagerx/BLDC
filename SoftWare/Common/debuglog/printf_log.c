#include "debuglog_cfg.h"
#include "debuglog.h"
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

/*==========================================================================================
 * @brief        常用的字符串处理
 * @FuncName     
 * @param        init_flag 
 * @param        runflag 
 * @param        ia 
 * @param        ib 
 * @param        ic 
 * @return       char* 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#define TEST_SUCCESS_STRING  "ok," 
#define TEST_FAIL_STRING     "err," 

char* err_state_printf(int8_t init_flag, int8_t runflag, int16_t ia, int16_t ib, int16_t ic,int16_t cur_state,int16_t cur_cmd) 
{
    static char u1Src[50];  // 使用static确保u1Src在函数调用之间保持生命期

    // 初始化u1Src为0，以确保字符串正确终止
    memset(u1Src, 0, sizeof(u1Src));

    // [0:4]字节存放'ok,' 或 'err,'
    int8_t cur_pos = 0;
    char str[20];
    uint8_t len;

    sprintf(str, "MCB: ");
    len = strlen(str);
    strncpy(&u1Src[cur_pos], str, len);
    cur_pos += len;

    if (init_flag) 
    {
        sprintf(str, TEST_SUCCESS_STRING);
        len = strlen(str);
        strncpy(&u1Src[cur_pos], str, len);
        cur_pos += len;
    } else {
        sprintf(str, TEST_FAIL_STRING);
        len = strlen(str);
        strncpy(&u1Src[cur_pos], str, len);
        cur_pos += len;
    }

    if (runflag)
    {
        sprintf(str, TEST_SUCCESS_STRING);
        len = strlen(str);
        strncpy(&u1Src[cur_pos], str, len);
        cur_pos += len;
    } else {
        sprintf(str, TEST_FAIL_STRING);
        len = strlen(str);
        strncpy(&u1Src[cur_pos], str, len);
        cur_pos += len;
    }

    sprintf(str, "ia:%d,", ia);
    len = strlen(str);
    strncpy(&u1Src[cur_pos], str, len);
    cur_pos += len;

    sprintf(str, "ib:%d,", ib);
    len = strlen(str);
    strncpy(&u1Src[cur_pos], str, len);
    cur_pos += len;

    sprintf(str, "ic:%d,", ic);
    len = strlen(str);
    strncpy(&u1Src[cur_pos], str, len);
    cur_pos += len;

    sprintf(str, "state:%d,", cur_state);
    len = strlen(str);
    strncpy(&u1Src[cur_pos], str, len);
    cur_pos += len;

    sprintf(str, "cmd:%d,", cur_cmd);
    len = strlen(str);
    strncpy(&u1Src[cur_pos], str, len);
    cur_pos += len;

    USER_DEBUG_NORMAL("err_flag  %s   %d\r\n",u1Src,cur_pos);
	return u1Src;
}





