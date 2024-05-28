
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


extern void _bsp_protransmit(unsigned char* pdata,unsigned short len);



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
    {"mc_tar_speed",    1,   NULL,                NULL},  
    {"mc_tar_id",       2,   _set_tarid,          NULL},  
    {"mc_tar_iq",       3,   _set_tariq,          NULL},
    {"motor_stop",      4,   _set_motorstop,      "motor state stop"},
    {"motor_start",     5,   _set_motorstart,     "motor state runing"},
    {"mc_setd_kp",      6,   _set_d_kp,           NULL},
    {"mc_setd_ki",      7,   _set_d_ki,           NULL},
    {"mc_pid_paraset",  8,   _set_pidpara_cmd,    NULL},
    {"mc_tar_speed",    9,   _set_tarspeed,       NULL},

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
void motorprotocol_transmit(char *pstr,uint16_t len)
{
    _bsp_protransmit((unsigned char *)pstr,len);
}
static void* _set_d_kp(char *str,int32_t kp)
{
    float val = 0.0f;
    if (!_findF_from_str(str,&val))
    {
        return 0;
    }
    motordebug.pid_d_kp = val;
    return 0;    
}
static void* _set_d_ki(char *str,int32_t ki)
{
    float val = 0.0f;            
    if (!_findF_from_str(str,&val))
    {
        return 0;
    }  
    motordebug.pid_d_ki = val;
    return 0; 
}
static void* _set_motorstart(char *str,int32_t iq)
{
    // motordebug.cur_cmd = "motor start";// sg_commandmap[CMD_SET_START].cmd;
    motordebug.cur_cmd = sg_commandmap[CMD_SET_START].cmd;
    return 0;
}
static void* _set_motorstop(char *str,int32_t iq)
{
    motordebug.cur_cmd = sg_commandmap[CMD_SET_STOP].cmd;
    return 0;
}
static void* _set_tarid(char *str,int32_t id)
{
    float val = 0.0f;            
    if (!_findF_from_str(str,&val))
    {
        return 0;
    }
    motordebug.id_targe = val;
    return 0;
} 

static void* _set_tariq(char *str,int32_t iq)
{
    float val = 0.0f;            
    if (!_findF_from_str(str,&val))
    {
        return 0;
    }
    motordebug.iq_targe = val;
    return 0;    
}

static void* _set_pidpara_cmd(char *str,int32_t param)
{
    motordebug.motor_stat = 4;
    return 0;
}

static void* _set_tarspeed(char *str,int32_t iq)
{
    float val = 0.0f;            
    if (!_findF_from_str(str,&val))
    {
        return 0;
    }
    motordebug.speed_targe = val;
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

            map[i].pf_cmdexcue(str,0);
            return map[i].cmd_index; // 找到匹配项，返回对应值  
        }
    }
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


