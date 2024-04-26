
/*
mc_tar_speed:200.0f  
mc_tar_id0.5f
mc_tar_iq:0.8f

*/
#include "motorctrl_common.h"
#include "board.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>  
#include <ctype.h> // for isspace()  

typedef void* (*pfunc)(char *,int32_t);
typedef struct {  
    const char *cmd;  
    unsigned short cmd_index;
    pfunc pf_cmdexcue;
} commandmap_t; 



static void* _set_tarid(char *str,int32_t id);
static void* _set_tariq(char *str,int32_t iq);
static void* _set_motorstop(char *str,int32_t iq);
static void* _set_motorstart(char *str,int32_t iq);
static void* _set_d_kp(char *str,int32_t kp);
static void* _set_d_ki(char *str,int32_t ki);
static void* _set_pidpara_cmd(char *str,int32_t param);
static void* _set_tarspeed(char *str,int32_t iq);

static int _findF_from_str(const char *str, float *val) ;
static unsigned short _findcmd_from_map(const char *str, const commandmap_t *map, size_t mapSize);
 

// 命令映射表  
commandmap_t sg_commandmap[] = {  
    {"mc_tar_speed", 1,NULL},  
    {"mc_tar_id", 2,_set_tarid},  
    {"mc_tar_iq", 3,_set_tariq},
    {"motor_stop",4,_set_motorstop},
    {"motor_start",5,_set_motorstart},
    {"mc_setd_kp",6,_set_d_kp},
    {"mc_setd_ki",7,_set_d_ki},
    {"mc_pid_paraset",8,_set_pidpara_cmd},
    {"mc_tar_speed",9,_set_tarspeed},

};
static char *Rx_Buf[62] = {0};
void motorprotocol_getdata(char *data)
{
    strcpy(Rx_Buf,data);
}

void motorprotocol_pause(char *cmd)
{
    float value =0.0f;
    unsigned short cmd_ID;
    if (cmd[0] == 0 && cmd[1] == 0)
    {
        /* code */
        return;
    }
    const size_t mapSize = sizeof(sg_commandmap) / sizeof(sg_commandmap[0]); 
    cmd_ID = _findcmd_from_map(cmd,sg_commandmap,mapSize);    
}

void motorprotocol_process(void)
{   
    motorprotocol_pause(Rx_Buf);
    memset(Rx_Buf, 0, sizeof(Rx_Buf));
}
static void* _set_d_kp(char *str,int32_t kp)
{
    float val = 0.0f;
    if (!_findF_from_str(str,&val))
    {
        return 0;
    }
    sg_motordebug.pid_d_kp = val;
    return 0;    
}
static void* _set_d_ki(char *str,int32_t ki)
{
    float val = 0.0f;            
    if (!_findF_from_str(str,&val))
    {
        return 0;
    }  
    sg_motordebug.pid_d_ki = val;
    return 0; 
}
static void* _set_motorstart(char *str,int32_t iq)
{
    sg_motordebug.motor_stat = 3;
    sg_motordebug.pid_Q_out = 0xFF;
    return 0;
}
static void* _set_motorstop(char *str,int32_t iq)
{
    sg_motordebug.motor_statue_To = "motor stop";
    return 0;
}
static void* _set_tarid(char *str,int32_t id)
{
    USER_DEBUG_NORMAL("set id\r\n");
    float val = 0.0f;            
    if (!_findF_from_str(str,&val))
    {
        return 0;
    }
    sg_motordebug.id_targe = val;
    return 0;
} 

static void* _set_tariq(char *str,int32_t iq)
{
    float val = 0.0f;            
    if (!_findF_from_str(str,&val))
    {
        return 0;
    }
    sg_motordebug.iq_targe = val;
    return 0;    
}

static void* _set_pidpara_cmd(char *str,int32_t param)
{
    sg_motordebug.motor_stat = 4;
    // sg_motordebug.motor_statue = ""
    return 0;
}

static void* _set_tarspeed(char *str,int32_t iq)
{
    float val = 0.0f;            
    if (!_findF_from_str(str,&val))
    {
        return 0;
    }
    sg_motordebug.speed_targe = val;
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
                USER_DEBUG_NORMAL("pf_cmdexcue NULL\r\n");
                return 0;
            }

            map[i].pf_cmdexcue(str,0);
            return map[i].cmd_index; // 找到匹配项，返回对应值  
        }
    }
    USER_DEBUG_NORMAL("NO Cmd PATH\r\n");
    return 0; // 未找到匹配项，返回0  
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


