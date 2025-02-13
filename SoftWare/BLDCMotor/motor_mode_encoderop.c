#include "motor_mode_encoderop.h"
#include "board.h"
#include "debuglog.h"
#include "string.h"
#include "motorctrl_common.h"
#include "mc_encoder.h"
#include "mc_commend.h"
static void motor_paraminit(motor_t *motor);
static void motor_paramdeinit(motor_t *motor);

fsm_rt_t motor_encoder_ol_mode(fsm_cb_t *pthis)
{
    motor_t *motor;
    motor = (motor_t*)pthis->pdata;    
    switch (pthis->chState) 
    {
    case ENTER:
        USER_DEBUG_NORMAL("entry encoer openloop mode\n");
        mc_encoder_init(&(motor->encoder_handle));//编码器初始化
        motor_paraminit(motor);
        pthis->chState = MOTOR_STATUS_READY;
        motor->curMotorstate = pthis->chState;
    case MOTOR_STATUS_READY:
        if (motor->curCmd != CMD_START)
        {
            break;
        }
        motor->enable();
        pthis->chState = MOTOR_STATUS_CALIBRATE;
        motor->curMotorstate = pthis->chState;      
        break;

    case MOTOR_STATUS_CALIBRATE:
        {
            if (motor->debug.pid_debug_target != 0.0f)
            {
                motor->encoder_handle.self_te = motor->debug.pid_debug_target;
                mc_encoder_calibrate(&motor->encoder_handle);
                pthis->chState = MOTOR_STATUS_RUN;
                motor->curMotorstate = pthis->chState;
            }
        }
        break;
       
    case MOTOR_STATUS_RUN:
        if (motor->curCmd == CMD_STOP)
        {
            pthis->chState = MOTOR_STATUS_STOP;
            motor->curMotorstate = pthis->chState;
        }else{
            motor->encoder_handle.self_te = motor->debug.pid_debug_target;
        }
        break;
    case MOTOR_STATUS_STOP:
        motor->disable();
        pthis->chState = MOTOR_STATUS_READY;
        motor->curMotorstate = pthis->chState;
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
