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
        motor_paraminit(motor);//编码器初始化，获取初始位置
        pthis->chState = READY;
    case READY:
        if (motor->curmode != M_SET_START)
        {
            break;
        }
        motor->enable();
        USER_DEBUG_NORMAL("motor enable\n");
        pthis->chState = CALIBRATE;
        break;

    case CALIBRATE:
        {
        }
        break;
    
    case IDLE:
        break;
    case INIT:
        break;    
    case RUN:
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
    memset(motor,0,sizeof(motor_t));
    motor_func_register(motor);
}
