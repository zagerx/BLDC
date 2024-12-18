#include "motor_speedmode.h"
#include "mc_protocol.h"
#include "motorctrl_common.h"
#include "string.h"
#include "mc_encoder.h"
#include "mc_speedloop.h"
#include "board.h"
#include "motorctrl_cfg.h"
#include "debuglog.h"
#include "mc_smo.h"
#include "math.h"
static void motor_paraminit(motor_t *motor);
static void motor_paramdeinit(motor_t *motor);

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
        motor_paraminit(motor);//编码器初始化，获取初始位置
        motor->encoder_handle.runflag = 0;
        pthis->chState = READY;
    case READY:
        if (motor->curmode != M_SET_START)
        {
            break;
        }
        motor->enable();
        pthis->chState = RUN;
        break;    
    case RUN:
        if (motor->curmode == M_SET_STOP)
        {
            pthis->chState = EXIT;
        }else{
            if (motor->debug.pid_debug_target!=0.0f)
            {                
                motor->encoder_handle.runflag = 1;
                #ifdef ENABLE_LINEAR_IN
                    motor->speed_handle.tar = linear_interpolation(&(motor->speed_handle.linear),\
                                                                    motor->debug.pid_debug_target);
                    s_type_interpolation(&test_s_valu,motor->debug.pid_debug_target);                
                #endif // DEBUG
                motor->speed_handle.real = motor->encoder_handle.speed;
                motor->currment_handle.iq_tar = speed_loop(&(motor->speed_handle));
                motor->currment_handle.id_tar = 0.0f;
            }
        }
        break;
    case EXIT:
        USER_DEBUG_NORMAL("exit speed mode\n");
        motor->disable();
        motor->debug.pid_debug_target = 0.0f;
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
#ifdef MOTOR_CURMENLOOP_DEBUG
    /*从FLASH指定位置读取PID参数数据*/
    motor_Romparam_t temp;
    motor->read(&temp,sizeof(motor_Romparam_t));
    /*初始化PID参数*/
    pid_init(&(motor->currment_handle.d_pid),temp.fbuf[0],temp.fbuf[1],1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    pid_init(&(motor->currment_handle.q_pid),temp.fbuf[0],temp.fbuf[1],1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    USER_DEBUG_NORMAL("D_Axis Kp:%.04f Ki:%.04f\n",motor->currment_handle.d_pid.kp,motor->currment_handle.d_pid.ki);  
#elif MOTOR_CURMENLOOP_TEST
    pid_init(&(motor->currment_handle.d_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    pid_init(&(motor->currment_handle.q_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);   
#elif MOTOR_SPEEDLOOP_DEBUG
    pid_init(&(motor->currment_handle.d_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    pid_init(&(motor->currment_handle.q_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);  

    motor_Romparam_t temp;
    motor->read((uint8_t *)&temp,sizeof(motor_Romparam_t));   
    pid_init(&(motor->speed_handle.pid),temp.fbuf[0],temp.fbuf[1],1.0,SPEED_OUT_MAX,SPEED_OUT_MIN);    
    lowfilter_init(&(motor->encoder_handle.speedfilter),15.0f);        
    USER_DEBUG_NORMAL("Currment  :   Kp:%.4f Ki:%.4f\n",motor->currment_handle.q_pid.kp,motor->currment_handle.q_pid.ki);  
    USER_DEBUG_NORMAL("Spedd loop:   Kp:%.4f,Ki:%.4f\n",motor->speed_handle.pid.kp,motor->speed_handle.pid.ki);
#endif

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



static void motor_paramdeinit(motor_t *motor)
{
#ifdef ENABLE_LINEAR_IN
    linear_interpolation_deinit(&(motor->speed_handle.linear));
#endif
}
