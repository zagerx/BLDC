#ifndef _MOTOR_PROTOCOL__H
#define _MOTOR_PROTOCOL__H
#include "stdint.h"

enum{
    CMD_SET_TARSPEED_ = 0,
    CMD_SET_ID,
    CMD_SET_IQ,
    CMD_SET_STOP,
    CMD_SET_START,
    CMD_SET_D_KP,
    CMD_SET_D_KI,
    CMD_SET_PIDPARASET,
    CMD_SET_TARSPEED,
};
typedef void* (*pfunc)(char *,int32_t);
typedef struct {  
    const char *cmd;  
    unsigned short cmd_index;
    pfunc pf_cmdexcue;
    const char *res_cmd;
} commandmap_t; 


extern commandmap_t sg_commandmap[];
void motorprotocol_tramit(char *pstr,uint16_t len);

#endif
