#include "motor_mode_normal.h"
#include "motorctrl_common.h"
#include "debuglog.h"
fsm_rt_t motor_normalmode(fsm_cb_t *pthis)
{
    enum{
        IDLE = USER,
    };
    motor_t *motor = (motor_t*)pthis->pdata;
    switch (pthis->chState)
    {
    case ENTER:
        USER_DEBUG_NORMAL("enter Normalmode\r\n");
        pthis->chState = IDLE;
        break;
    case EXIT:
        USER_DEBUG_NORMAL("exit Normalmode\r\n");
        break;
    default:
        break;
    }
    return 0;
}
