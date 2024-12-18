#include "mc_encoderopmode.h"
#include "board.h"
#include "motorctrl_cfg.h"
#include "debuglog.h"
#include "string.h"
#include "motorctrl_common.h"
#include "mc_protocol.h"
#include "mc_encoder.h"
static void motor_paraminit(motor_t *motor);
static void motor_paramdeinit(motor_t *motor);

fsm_rt_t motor_encoder_ol_mode(fsm_cb_t *pthis)
{
    enum{
        READY = USER,
        CALIBRATE,
        IDLE,
        RUN,
        INIT,
    };
    motor_t *motor;
    motor = (motor_t*)pthis->pdata;    
    switch (pthis->chState) 
    {
    case ENTER:
        USER_DEBUG_NORMAL("entry encoer openloop mode\n");
        motor_paraminit(motor);//编码器初始化
        motor->encoder_handle.runflag = 0;
        pthis->chState = READY;
    case READY:
        if (motor->curmode != M_SET_START)
        {
            break;
        }
        motor->enable();
        USER_DEBUG_NORMAL("motor enable\n");
        #if(MOTOR_OPENLOOP && MOTOR_OPENLOOP_ENCODER)
            pthis->chState = CALIBRATE;
            USER_DEBUG_NORMAL("encoder loop test\n");
        #else
            USER_DEBUG_NORMAL("self loop test\n");
            motor->encoder_handle.runflag = 1;
            pthis->chState = RUN;
        #endif
        
        break;

    case CALIBRATE:
        {
            motor->encoder_handle.get_firstpos((motor->encoder_handle.sensor));
            if (  motor->debug.pid_debug_target != 0.0f)
            {
                motor->encoder_handle.self_te = motor->debug.pid_debug_target;
                motor->encoder_handle.runflag = 1;
                pthis->chState = RUN;
            }
        }
        break;
    
    case IDLE:
        break;
    case INIT:
        break;    
    case RUN:
        if (motor->curmode == M_SET_STOP)
        {
            pthis->chState = EXIT;
        }else{
            motor->encoder_handle.self_te = motor->debug.pid_debug_target;
        }
        break;
        break;    
    case EXIT:
        USER_DEBUG_NORMAL("exit encoder openloop mode\n");
        motor->disable();
        motor_paramdeinit(motor);
        pthis->chState = ENTER;
        break;
    default:
        break;
    }
    return 0;
}

static void motor_paraminit(motor_t *motor)
{
    mc_encoder_init(&(motor->encoder_handle));
}



static void motor_paramdeinit(motor_t *motor)
{
    motor->encoder_handle.self_te = 0.0f;
    motor->encoder_handle.deinit((motor->encoder_handle.sensor));

    motor->debug.pid_debug_target = 0.0f;
    motor->debug.pid_debug_target = 0.0f;
}
