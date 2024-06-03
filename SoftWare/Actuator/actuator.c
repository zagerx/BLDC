#include "Fct.h"

static int cmd01_actutorfunc(actuator_t *pthis);
static void Fct_FindCmd(FCT_PRO_CMD cmd);
static commandmap_t sgcommandmap[] = {
    {CMD_01,{ACT_IDLE,cmd01_actutorfunc}},
};

extern void Fct_protocolpause(uint8_t *data)
{
    /*根据协议处理 data*/
    FCT_PRO_CMD cmd;
    cmd = (FCT_PRO_CMD)(*data);
    
    Fct_FindCmd(cmd);
}
extern void Fct_actuatorprocess(void)
{
    for (uint16_t i = 0; i < sizeof(sgcommandmap)/sizeof(sgcommandmap[0]); i++)
    {
        if (sgcommandmap[i].actutor.pFunc != NULL)
        {
            sgcommandmap[i].actutor.pFunc((actuator_t*)&(sgcommandmap[i].actutor));
        }
    }
}

static void Fct_FindCmd(FCT_PRO_CMD cmd)
{
    for (uint16_t i = 0; i < sizeof(sgcommandmap)/sizeof(sgcommandmap[0]); i++)
    {
        if (sgcommandmap[i].cmd != cmd)
        {
            continue;
        }
        
        if (sgcommandmap[i].actutor.pFunc != NULL)
        {
            sgcommandmap[i].actutor.state = ACT_START;
        }
    }
}

static int cmd01_actutorfunc(actuator_t *pthis)
{
    switch (pthis->state)
    {
    case ACT_START:
        break;

    default:
        break;
    }
    return 0;
}
