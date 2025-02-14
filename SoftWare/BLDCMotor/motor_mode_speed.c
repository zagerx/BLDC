#include "motor_mode_speed.h"
#include "motorctrl_common.h"
#include "string.h"
#include "mc_encoder.h"

#include "board.h"

#include "debuglog.h"
#include "mc_smo.h"
#include "math.h"
#include "mc_commend.h"

static void speed_ctrlparam_init(motor_t *motor);
static void speed_ctrlparam_deinit(motor_t *motor);

fsm_rt_t motor_speedmode(fsm_cb_t *pthis)
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
    static s_in_t test_s_valu;  
    switch (pthis->chState)
    {
    case ENTER:
        USER_DEBUG_NORMAL("entry speed mode\n");
        mc_encoder_init(&(motor->encoder_handle));//编码器初始化
        speed_ctrlparam_init(motor);
        motor->encoder_handle.runflag = 0;
        pthis->chState = READY;
    case READY:
        if (motor->curmode != STATUS_START)
        {
            break;
        }
        motor->enable();
        USER_DEBUG_NORMAL("motor enable\n");
        pthis->chState = CALIBRATE;
        break;    
    case CALIBRATE:
        break;  
    case RUN:
        if (motor->curmode == STATUS_STOP)
        {
            motor->disable();
            mc_encoder_deinit(&(motor->encoder_handle));
            speed_ctrlparam_deinit(motor);            
            pthis->chState = ENTER;
        }else{
            motor->currment_handle.id_tar = 0.0f;
            // motor->currment_handle.iq_tar = motor->debug.pid_debug_target;
        }
        break;
    case EXIT:
        USER_DEBUG_NORMAL("exit speed mode\n");
        motor->disable();
        mc_encoder_deinit(&(motor->encoder_handle));
        speed_ctrlparam_deinit(motor);
        break;
    default:
        break;
    }
    return 0;
}

static void speed_ctrlparam_init(motor_t *motor)
{
    pid_init(&(motor->currment_handle.d_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    pid_init(&(motor->currment_handle.q_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);  
    pid_init(&(motor->speed_handle.pid),SPEEDLOOP_KP,SPEEDLOOP_KI,1.0,SPEED_OUT_MAX,SPEED_OUT_MIN);    
    lowfilter_init(&(motor->encoder_handle.speedfilter),15.0f);        
    USER_DEBUG_NORMAL("Currment  :   Kp:%.4f Ki:%.4f\n",CURRMENTLOOP_KP,CURRMENTLOOP_KI);  
    USER_DEBUG_NORMAL("Spedd loop:   Kp:%.4f,Ki:%.4f\n",SPEEDLOOP_KP,CURRMENTLOOP_KI);
#ifdef ENABLE_LINEAR_IN
    linear_interpolation_init(&(motor->speed_handle.linear),2.0f,-2.0f);
#endif
}



static void speed_ctrlparam_deinit(motor_t *motor)
{
#ifdef ENABLE_LINEAR_IN
    linear_interpolation_deinit(&(motor->speed_handle.linear));
#endif
    pid_reset(&(motor->currment_handle.d_pid));
    pid_reset(&(motor->currment_handle.q_pid));  
    pid_reset(&(motor->speed_handle.pid)); 
}
