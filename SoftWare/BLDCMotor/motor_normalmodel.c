#include    "fsm.h"
#include "motorctrl_common.h"
#include "mc_protocol.h"
#include "board.h"
#include "string.h"
#include "mc_speedmode.h"
#include "mc_torquemode.h"

static void mc_param_init(void);
void mc_param_deinit(void);
#define CURRMENTLOOP_PERIOD      (0.0001f)

fsm_rt_t motor_normalmode(fsm_cb_t *pthis)
{
    enum{
        MOTOR_INIT = USER,
        MOTOR_START, 
        MOTOR_RUNING,
        MOTOR_STOP,
    };
    switch (pthis->chState)
    {
    case ENTER:
        mc_param_deinit();
        pthis->chState = MOTOR_INIT;
    case MOTOR_INIT:

        if (motordebug.rec_cmd != M_SET_START)
        {
            break;
        }
        
        mc_param_init();
        pthis->chState = MOTOR_START;
    case MOTOR_START:
        motor_enable();
        pthis->chState = MOTOR_RUNING;
        break;

    case MOTOR_RUNING:
        {
            dq_t idq = {0.0f};
            idq.q = speed_loop(10.0f);
            motordebug.speed_targe = 10.0f;
            motordebug.id_targe = idq.d;
            motordebug.iq_targe = idq.q;
        }


        if (motordebug.rec_cmd == M_SET_STOP)
        {
            pthis->chState = EXIT;
        }
        
        break;

    case EXIT:
        motor_disable();
        mc_param_deinit();
        pthis->chState = ENTER;
        break;
    default:
        break;
    }
    return 0;
}

static void mc_param_init(void)
{
    // pid_init(&(mc_param.daxis_pi),0.0273f,166.2507f * CURRMENTLOOP_PERIOD,1.0f,D_MAX_VAL,D_MIN_VAL);
    // pid_init(&(mc_param.qaxis_pi),0.0273f,166.2507f * CURRMENTLOOP_PERIOD,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mc_param.daxis_pi),0.1,0.01f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mc_param.qaxis_pi),0.1,0.01f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mc_param.speedloop_pi),0.9f,0.1f,1.0f,D_MAX_VAL,D_MIN_VAL);
    // lowfilter_init(&(mc_param.elefitler[0]),80);
    // lowfilter_init(&(mc_param.elefitler[1]),80);
    // lowfilter_init(&(mc_param.elefitler[2]),80);
}

 void mc_param_deinit(void)
{
    memset(&mc_param,0,sizeof(mc_param));
    memset(&motordebug,0,sizeof(motordebug));
}
