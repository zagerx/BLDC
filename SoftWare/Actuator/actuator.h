#ifndef __ACTUATOR__H
#define __ACTUATOR__H
#include "stdint.h"

typedef enum{
    CMD_NONE = -1,
    EVENT_01 = 0,
    EVENT_02 = 1,
}ACT_DRI_EVENT;

typedef enum{
    ACT_IDLE = 0,
    ACT_START = 1,
    ACT_EXIT = 2,   
}FCT_ACT_STATE;
typedef struct actuator actuator_t;
typedef int (*pfunc_act)(actuator_t*);
typedef struct actuator
{
    uint32_t cnt;
    FCT_ACT_STATE state;
    pfunc_act pFunc;
}actuator_t;


extern void actutor_get_driverevent(void *data);
extern void actuator_process(void);
extern int actuator_resgiter(actuator_t *act,ACT_DRI_EVENT event_index,uint16_t map_pos);
extern void actuator_mapinit(void);

#endif
