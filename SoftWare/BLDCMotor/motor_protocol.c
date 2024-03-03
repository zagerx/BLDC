
/*
mc_tar_speed:200.0f  
mc_tar_id0.5f
mc_tar_iq:0.8f

*/
#include "motorctrl_common.h"
#include "_common.h"

#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>  
#include <ctype.h> // for isspace()  

typedef void* (*pfunc)(int32_t);
typedef struct {  
    const char *cmd;  
    unsigned short cmd_index;
    pfunc pf_cmdexcue;
} commandmap_t; 



static void* _set_tarid(int32_t id);
static void* _set_tariq(int32_t iq);

static int _findF_from_str(const char *str, float *val) ;
static unsigned short _findcmd_from_map(const char *str, const commandmap_t *map, size_t mapSize);
 

// 命令映射表  
commandmap_t sg_commandmap[] = {  
    {"mc_tar_speed", 1,NULL},  
    {"mc_tar_id", 2,_set_tarid},  
    {"mc_tar_iq", 3,_set_tariq}  
    // 可以在这里添加更多的命令映射  
};

void motorprotocol_pause(char *cmd)
{
    float value =0.0f;
    unsigned short cmd_ID;
    USER_DEBUG_NORMAL("recive: %s\r\n",cmd);

    const size_t mapSize = sizeof(sg_commandmap) / sizeof(sg_commandmap[0]); 
    cmd_ID = _findcmd_from_map(cmd,sg_commandmap,mapSize);    
}

static void* _set_tarid(int32_t id)
{
    USER_DEBUG_NORMAL("set id\r\n");
    sg_motordebug.Q_id_targe = id;
    return 0;
} 

static void* _set_tariq(int32_t iq)
{
    USER_DEBUG_NORMAL("set iq\r\n");
    sg_motordebug.Q_iq_targe = iq;
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
            float val = 0.0f;            
            if (!_findF_from_str(str,&val))
            {
                return 0;
            }
            if (!map[i].pf_cmdexcue)
            {
                USER_DEBUG_NORMAL("pf_cmdexcue NULL\r\n");
                return 0;
            }
            USER_DEBUG_NORMAL("%f\r\n",val);
            int32_t temp = 0;
            temp = val * (1<<15);
            map[i].pf_cmdexcue(temp);
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


