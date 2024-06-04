#include "actuator_cfg.h"

int event01_actutorfunc(actuator_t *pthis)
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


