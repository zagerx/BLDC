#ifndef __FCT__H
#define __FCT__H
#include "stdint.h"
typedef enum{
    CMD_NONE = -1,
    CMD_01 = 0,
    CMD_02 = 1,   
}FCT_PRO_CMD;

typedef enum{
    ACT_IDLE = 0,
    ACT_START = 1,
    ACT_EXIT = 2,   
}FCT_ACT_STATE;

typedef struct actuator actuator_t;
typedef int (*pfunc_act)(actuator_t*);
typedef struct actuator
{
    FCT_ACT_STATE state;
    pfunc_act pFunc;
}actuator_t;

typedef struct {  
    FCT_PRO_CMD cmd;  
    actuator_t actutor;
} commandmap_t; 


extern void Fct_protocolpause(uint8_t *data);
extern void Fct_actuatorprocess(void);


#endif
