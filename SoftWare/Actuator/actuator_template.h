#ifndef __ACTUATOR_CFG__H
#define __ACTUATOR_CFG__H



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
    FCT_ACT_STATE state;
    pfunc_act pFunc;
}actuator_t;

int event01_actutorfunc(actuator_t *pthis);



#endif
