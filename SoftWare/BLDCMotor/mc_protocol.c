
/*
mc_tar_speed:200.0f  
mc_tar_id0.5f
mc_tar_iq:0.8f

*/
#include "motorctrl_common.h"
#include "mc_protocol.h"
// #include "protocol.h"
#include "board.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>  
#include <ctype.h> // for isspace()  

#include "debuglog.h"
extern void _bsp_protransmit(unsigned char* pdata,unsigned short len);
__attribute__((weak)) void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
}


static void* _set_motorstop(char *str,int32_t iq);
static void* _set_motorstart(char *str,int32_t iq);
static void* _set_motornormalmode(char *str,int32_t iq);

static void* _set_pid_param(char *str,int32_t kp);

static int _findF_from_str(const char *str, float *val) ;
static unsigned short _findcmd_from_map(const char *str, const commandmap_t *map, size_t mapSize);
static int parse_floats_from_string(const char *str, float vals[], int max_vals);

// 命令映射表  
commandmap_t sg_commandmap[] = {  
    {"motor_stop",      4,   _set_motorstop,      "motor state stop"},
    {"motor_start",     5,   _set_motorstart,     "motor state runing"},
    {"motor_normalmode",     5,   _set_motornormalmode,     "motor state runing"},    
    {"pid_param",      6,   _set_pid_param,            "pid param set"},
};
static char *Rx_Buf[62] = {0};
void motorprotocol_getdata(char *data,unsigned short len)
{
    memcpy(Rx_Buf,data,len);
}

void motorprotocol_pause(char *cmd)
{
    if (cmd[0] == 0 && cmd[1] == 0)
    {
        return;
    }
    const size_t mapSize = sizeof(sg_commandmap) / sizeof(sg_commandmap[0]); 
    _findcmd_from_map(cmd,sg_commandmap,mapSize);
}

void motorprotocol_process(void)
{   
    motorprotocol_pause((char *)Rx_Buf);
    memset((char *)Rx_Buf, 0, sizeof(Rx_Buf));
}
void motorprotocol_transmit(char *pstr,uint16_t len)
{
    _bsp_protransmit((unsigned char *)pstr,len);
}
static void* _set_pid_param(char *str,int32_t kp)
{
    float val = 0.0f;
    USER_DEBUG_NORMAL("%s\r\n",str);
    float vals[3];
    parse_floats_from_string(str,vals,3);
    motordebug.pid_kp = vals[0];
    motordebug.pid_ki = vals[1];
    motordebug.pid_kc = vals[2];
    USER_DEBUG_NORMAL("%f %f %f\r\n",vals[0],vals[1],vals[2]);
    return 0;    
}

static void* _set_motorstart(char *str,int32_t iq)
{
    motordebug.cur_cmd = sg_commandmap[CMD_SET_START].cmd;
    return 0;
}
static void* _set_motorstop(char *str,int32_t iq)
{
    motordebug.cur_cmd = sg_commandmap[CMD_SET_STOP].cmd;
    return 0;
}
static void* _set_motornormalmode(char *str,int32_t iq)
{
    motordebug.cur_cmd = sg_commandmap[CMD_SET_NORMALMODE].cmd;
    return 0;    
}

// 查找命令的函数  
static unsigned short _findcmd_from_map(const char *str, const commandmap_t *map, size_t mapSize) {  
    if (str == NULL) {  
        return 0; // 如果传入的字符串为空，则返回0表示不匹配  
    }  
      
    for (size_t i = 0; i < mapSize; ++i) {  
        // 使用strncmp比较直到遇到冒号或字符串结束  
        size_t cmdLen = strlen(map[i].cmd);  
        if (strncmp(str, map[i].cmd, cmdLen) == 0 && str[cmdLen] == ':') { 

            if (!map[i].pf_cmdexcue)
            {
                return 0;
            }

            map[i].pf_cmdexcue((char *)str,0);
            return map[i].cmd_index; // 找到匹配项，返回对应值  
        }
    }
    return 0; // 未找到匹配项，返回0  
}  
static int parse_floats_from_string(const char *str, float vals[], int max_vals)
{  
    // 查找冒号  
    const char *colon_pos = strchr(str, ':');  
    if (colon_pos == NULL) {  
        return 0; // 没有找到冒号  
    }  
      
    // 跳过冒号后的空格（如果有）  
    colon_pos++;  
    while (isspace((unsigned char)*colon_pos)) {  
        colon_pos++;  
    }  
      
    // 初始化解析位置  
    const char *ptr = colon_pos;  
    int num_floats = 0;  
  
    // 解析浮点数  
    while (*ptr != '\0' && num_floats < max_vals) {  
        // 跳过前面的空格（如果有）  
        while (isspace((unsigned char)*ptr)) {  
            ptr++;  
        }  
  
        // 检查是否到达字符串末尾  
        if (*ptr == '\0') {  
            break;  
        }  
  
        // 解析浮点数  
        char *endptr;  
        vals[num_floats] = strtof(ptr, &endptr);  
  
        // 更新解析位置  
        ptr = endptr;  
  
        // 检查逗号或字符串末尾  
        if (*ptr == ',') {  
            ptr++; // 跳过逗号  
            num_floats++; // 增加浮点数计数  
  
            // 跳过逗号后的空格（如果有）  
            while (isspace((unsigned char)*ptr)) {  
                ptr++;  
            }  
        } else if (*ptr == '\0') {  
            num_floats++; // 如果到达字符串末尾，也计数为一个浮点数  
            break;  
        } else {  
            // 非预期字符，退出循环  
            break;  
        }  
    }  
  
    // 如果成功解析了至少一个浮点数，则返回其数量  
    return num_floats > 0 ? num_floats : 0;  
} 
static int _findF_from_str(const char *str, float *val) {  
    if (str == NULL || val == NULL) {  
        return 0; // Invalid arguments  
    }  
  
    const char *colonPos = strchr(str, ':'); // Find the colon in the string  
    if (colonPos == NULL || *(colonPos + 1) == '\0') {  
        return 0; // No colon found or no characters after colon  
    }  
  
    // Skip the colon and any following whitespace  
    const char *numStart = colonPos + 1;  
    while (isspace((unsigned char)*numStart)) {  
        numStart++;  
    }  
  
    // Attempt to parse the number  
    char *endPtr;  
    float parsedValue = strtof(numStart, &endPtr);  
  
    // Check if a valid number was parsed  
    if (endPtr != numStart && (*endPtr == '\0' || isspace((unsigned char)*endPtr))) {  
        *val = parsedValue;  
        return 1; // Success  
    }
  
    return 0; // Failure: no valid number found or trailing non-whitespace characters  
}  


