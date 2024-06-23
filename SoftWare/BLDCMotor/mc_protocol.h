#ifndef _MOTOR_PROTOCOL__H
#define _MOTOR_PROTOCOL__H
#include "stdint.h"

enum{

    CMD_SET_STOP,
    CMD_SET_START,
    CMD_SET_NORMALMODE,
    CMD_SET_DEBUGMODE,
    CMD_SET_D_KP,
    CMD_SET_PIDTAR,
};
typedef void* (*pfunc)(char *,int32_t);
typedef struct {  
    char *cmd;  
    unsigned short cmd_index;
    pfunc pf_cmdexcue;
    char *res_cmd;
} commandmap_t; 


extern commandmap_t sg_commandmap[];
void motorprotocol_transmit(char *pstr,uint16_t len);

#endif
