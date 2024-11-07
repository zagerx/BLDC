#include "motor_speedmode.h"
#include "mc_protocol.h"
#include "motorctrl_common.h"
#include "string.h"
#include "mc_encoder.h"
#include "flash.h"
#include "mc_speedloop.h"
#include "board.h"
#include "motorctrl_cfg.h"
#include "debuglog.h"
#include "mc_smo.h"

static void motor_init(mc_param_t *motor);
static void mc_param_deinit(void);

fsm_rt_t motor_speedmode(fsm_cb_t *pthis)
{
    enum{
        READY = USER,
        CALIBRATE,
        RUN,
    };
    mc_param_t *motor;
    motor = (mc_param_t*)pthis->pdata;    
    switch (pthis->chState)
    {
    case ENTER:
        USER_DEBUG_NORMAL("entry speed mode\n");
        motor_init(motor);
        motordebug.id_targe = 0.0f;
        motordebug.iq_targe = 0.0f;
        motordebug.speed_targe = 0.0f;
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
            // motor->speed_handle.real = motor->encoder_handle.speed;
            // motor->currment_handle.iq_tar = speed_loop(&(motor->speed_handle));
            // motor->currment_handle.id_tar = 0.0f;
        }
        break;    
    case EXIT:
        USER_DEBUG_NORMAL("exit speed mode\n");
        motor->disable();
        mc_param_deinit();
        pthis->chState = ENTER;
        break;
    default:
        break;
    }
    return 0;
}

static void motor_init(mc_param_t *motor)
{
    motot_func_register(motor);
    mc_encoder_init(&(motor->encoder_handle));

    /*从FLASH指定位置读取PID参数数据*/
    flash_t temp;
    user_flash_read(PID_PARSE_ADDR,(uint8_t *)&temp,PID_PARSE_SIZE);
    /*初始化PID参数*/
    pid_init(&(motor->currment_handle.d_pid),temp.fbuf[0],temp.fbuf[1],1.0,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(motor->currment_handle.q_pid),temp.fbuf[0],temp.fbuf[1],1.0,D_MAX_VAL,D_MIN_VAL);  

    // pid_init(&(motor->speed_handle.pid),0.01f,0.08f,1.0,D_MAX_VAL,D_MIN_VAL);
    // lowfilter_init(&(motor->encoder_handle.speedfilter),15.0f);
    USER_DEBUG_NORMAL("D Kp%.4f Ki%.4f\n",temp.fbuf[0],temp.fbuf[1]);
    // USER_DEBUG_NORMAL("Q Kp%.04f Ki%.04f\n",motor->currment_handle.q_pid.kp,motor->currment_handle.q_pid.ki);
}



static void mc_param_deinit(void)
{
    memset(&motor1,0,sizeof(motor1));
    memset(&motordebug,0,sizeof(motordebug));
}

