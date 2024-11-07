#include "motor_normalmode.h"
#include "motor_speedmode.h"
#include "motorctrl_common.h"
#include "mc_protocol.h"
fsm_rt_t motor_normalmode(fsm_cb_t *pthis)
{
    mc_param_t *motor;
    motor = (mc_param_t*)pthis->pdata;
    switch (pthis->chState)
    {
    case ENTER:
        if (motor->curmode == M_SET_SpeedM)
        {
            motor->curmode = 0;//TODO
            TRAN_STATE(pthis,motor_speedmode);
        }
        break;
    case EXIT:
        break;
    default:
        break;
    }
    return 0;
}
