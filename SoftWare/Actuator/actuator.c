#include "actuator.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "debuglog.h"
#undef NULL
#define NULL 0

typedef struct {  
    ACT_DRI_EVENT event;  
    actuator_t *actutor;
} eventmap_t;

#define MAP_SIZE 3

static eventmap_t sg_eventmap[MAP_SIZE] = {0};
extern void actuator_process(void)
{
    for (uint16_t i = 0; i < MAP_SIZE; i++)
    {
        if (sg_eventmap[i].actutor->pFunc != NULL && sg_eventmap[i].actutor != NULL)
        {
            sg_eventmap[i].actutor->pFunc((actuator_t*)(sg_eventmap[i].actutor));
        }
    }
}

extern void actuator_mapinit(void)
{
    memset(sg_eventmap, 0, sizeof(eventmap_t) * MAP_SIZE);
}
extern int actuator_resgiter(actuator_t *act,ACT_DRI_EVENT event_index,uint16_t map_pos)
{
    sg_eventmap[map_pos].event = event_index;
    sg_eventmap[map_pos].actutor = (actuator_t *)act;
    return 0;
}
static void _evevt_triggle_actutor(ACT_DRI_EVENT event);

extern void actutor_get_driverevent(void *data)
{
    _evevt_triggle_actutor(*(ACT_DRI_EVENT*)(data));
}
static void _evevt_triggle_actutor(ACT_DRI_EVENT event)
{
    for (uint16_t i = 0; i < sizeof(sg_eventmap)/sizeof(sg_eventmap[0]); i++)
    {
        if (sg_eventmap[i].event != event)
        {
            continue;
        }

        if (sg_eventmap[i].actutor->pFunc != NULL)
        {
            sg_eventmap[i].actutor->state = ACT_START;
        }
    }
}

