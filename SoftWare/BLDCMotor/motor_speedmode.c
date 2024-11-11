#include "motor_speedmode.h"
#include "mc_protocol.h"
#include "motorctrl_common.h"
#include "string.h"
#include "mc_encoder.h"
// #include "flash.h"
#include "mc_speedloop.h"
#include "board.h"
#include "motorctrl_cfg.h"
#include "debuglog.h"
#include "mc_smo.h"

static void motor_init(motor_t *motor);
static void motor_deinit(motor_t *motor);

fsm_rt_t motor_speedmode(fsm_cb_t *pthis)
{
    enum{
        READY = USER,
        CALIBRATE,
        RUN,
    };
    motor_t *motor;
    motor = (motor_t*)pthis->pdata;    
    switch (pthis->chState)
    {
    case ENTER:
        USER_DEBUG_NORMAL("entry speed mode\n");
        motor_init(motor);
        pthis->chState = READY;
    case READY:
        if (motor->curmode != M_SET_START)
        {
            break;
        }
        motor->enable();
        #ifdef MOTOR_OPENLOOP
            pthis->chState = RUN;
        #else
            pthis->chState = CALIBRATE;
        #endif
    case CALIBRATE:
        {
            static unsigned short cnt;
            dq_t dq;alpbet_t ab;duty_t duty;
            if (cnt++>200)
            {
                cnt = 0;
                dq.d = 0.0f;
                dq.q = 0.0f;    
                ab = _2r_2s(dq, 0.0f);
                pthis->chState = RUN;
            }else{
                dq.d = OPENLOOP_DEBUG_TOTAL_Te;
                dq.q = 0.0f;    
                ab = _2r_2s(dq, 0.0f);
            }
            duty = SVM(ab.alpha, ab.beta);
            motor->setpwm(duty._a, duty._b, duty._c);
        }
        break;
    case RUN:
        if (motor->curmode == M_SET_STOP)
        {
            pthis->chState = EXIT;
        }else{
            #ifdef MOTOR_CURMENLOOP_DEBUG
                motor->currment_handle.id_tar = motor->currment_handle.pid_debug_target;
                motor->currment_handle.iq_tar = 0.0f;
            #elif MOTOR_CURMENLOOP_TEST 
                motor->currment_handle.id_tar = 0.0f;
                motor->currment_handle.iq_tar = motor->currment_handle.pid_debug_target;
            #else
                motor->speed_handle.tar = motor->currment_handle.pid_debug_target;
                motor->speed_handle.real = motor->encoder_handle.speed;
                motor->currment_handle.iq_tar = speed_loop(&(motor->speed_handle));
                motor->currment_handle.id_tar = 0.0f;
            #endif
        }
        break;    
    case EXIT:
        USER_DEBUG_NORMAL("exit speed mode\n");
        motor->disable();
        motor_deinit(motor);
        pthis->chState = ENTER;
        break;
    default:
        break;
    }
    return 0;
}

static void motor_init(motor_t *motor)
{
    motor_func_register(motor);
    mc_encoder_init(&(motor->encoder_handle));
#ifdef MOTOR_CURMENLOOP_DEBUG
    /*从FLASH指定位置读取PID参数数据*/
    motor_Romparam_t temp;
    motor->read(&temp,sizeof(motor_Romparam_t));
    /*初始化PID参数*/
    pid_init(&(motor->currment_handle.d_pid),temp.fbuf[0],temp.fbuf[1],1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    pid_init(&(motor->currment_handle.q_pid),temp.fbuf[0],temp.fbuf[1],1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    USER_DEBUG_NORMAL("Q Kp%.04f Ki%.04f\n",motor->currment_handle.q_pid.kp,motor->currment_handle.q_pid.ki);  
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
}



static void motor_deinit(motor_t *motor)
{
    memset(motor,0,sizeof(motor_t));
}

