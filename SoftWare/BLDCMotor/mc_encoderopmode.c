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
        mc_encoder_init(&(motor->encoder_handle));//编码器初始化
        motor_paraminit(motor);
        pthis->chState = READY;
    case READY:
        if (motor->curmode != M_SET_START)
        {
            break;
        }
        motor->enable();
        USER_DEBUG_NORMAL("motor enable\n");
        #if(MOTOR_WORK_MODE == MOTOR_DEBUG_ENCODERMODE)
            pthis->chState = CALIBRATE;
            USER_DEBUG_NORMAL("encoder loop test\n");
        #elif(MOTOR_WORK_MODE == MOTOR_DEBUG_SELF_MODE)
            USER_DEBUG_NORMAL("self loop test\n");
            motor->encoder_handle.runflag = 1;
            pthis->chState = RUN;
        #else            
        #endif
        
        break;

    case CALIBRATE:
        {
            if (  motor->debug.pid_debug_target != 0.0f)
            {
                motor->encoder_handle.self_te = motor->debug.pid_debug_target;
                mc_encoder_calibrate(&motor->encoder_handle);
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
    case EXIT:
        USER_DEBUG_NORMAL("exit encoder openloop mode\n");
        motor->disable();
        mc_encoder_deinit(&(motor->encoder_handle));
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
    motor->debug.pid_debug_target = 0.0f;
}



static void motor_paramdeinit(motor_t *motor)
{
    motor->debug.pid_debug_target = 0.0f;
}
