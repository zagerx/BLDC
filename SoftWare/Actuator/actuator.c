#include "actuator.h"
#include "actuator_cfg.h"
#include "stdint.h"

#ifdef ACTUATOR_MODE
typedef enum{
    CMD_NONE = -1,
    EVENT_01 = 0,
    EVENT_02 = 1,   
}ACT_DRI_EVENT;
#endif


typedef struct {  
    ACT_DRI_EVENT cmd;  
    actuator_t actutor;
} eventmap_t;
static eventmap_t sg_eventmap[] = {
    {EVENT_01,{ACT_IDLE,event01_actutorfunc}},
};

static void _evevt_triggle_actutor(ACT_DRI_EVENT event);


extern void actutor_get_driverevent(void *data)
{
    _evevt_triggle_actutor((ACT_DRI_EVENT)(*data));
}
extern void actuator_process(void)
{
    for (uint16_t i = 0; i < sizeof(sg_eventmap)/sizeof(sg_eventmap[0]); i++)
    {
        if (sg_eventmap[i].actutor.pFunc != NULL)
        {
            sg_eventmap[i].actutor.pFunc((actuator_t*)&(sg_eventmap[i].actutor));
        }
    }
}

static void _evevt_triggle_actutor(ACT_DRI_EVENT event)
{
    for (uint16_t i = 0; i < sizeof(sg_eventmap)/sizeof(sg_eventmap[0]); i++)
    {
        if (sg_eventmap[i].event != event)
        {
            continue;
        }
        
        if (sg_eventmap[i].actutor.pFunc != NULL)
        {
            sg_eventmap[i].actutor.state = ACT_START;
        }
    }
}

