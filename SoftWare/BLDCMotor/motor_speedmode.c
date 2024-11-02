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
static void mc_param_init(void);
static void mc_param_deinit(void);

fsm_rt_t motor_speedmode(fsm_cb_t *pthis)
{
    enum{
        READY = USER,
        RUN,
    };
    switch (pthis->chState)
    {
    case ENTER:
        USER_DEBUG_NORMAL("entry speed mode\n");
    #if  !(defined(MOTOR_OPENLOOP) || defined(MOTOR_OPENLOOP_ENCODER))
        mc_param_init();
    #endif

#if (ENCODER_TYPE == ENCODER_TYPE_HALL)
     mc_hallencoder_init();
#endif

        motordebug.id_targe = 0.0f;
        motordebug.iq_targe = 0.0f;
        motordebug.speed_targe = 0.0f;
        pthis->chState = READY;
    case READY:
        if (motordebug.rec_cmd != M_SET_START)
        {
            break;
        }
        motor_enable();
        pthis->chState = RUN;
        break;
    case RUN:
        if (motordebug.rec_cmd == M_SET_STOP)
        {
            pthis->chState = EXIT;
        }else{
            mc_param.speed_handle.real = mc_param.encoder_handle.speed;
            mc_param.currment_handle.iq_tar = speed_loop(&(mc_param.speed_handle));
            mc_param.currment_handle.id_tar = 0.0f;
        }
        break;    
    case EXIT:
        USER_DEBUG_NORMAL("exit speed mode\n");
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

    flash_t temp;
    /*从FLASH指定位置读取PID参数数据*/
    user_flash_read(PID_PARSE_ADDR,(uint8_t *)&temp,PID_PARSE_SIZE);
    /*初始化PID参数*/
    pid_init(&(mc_param.currment_handle.d_pid),0.1f,0.01f,1.0,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mc_param.currment_handle.q_pid),0.1f,0.01f,1.0,D_MAX_VAL,D_MIN_VAL);       
    pid_init(&(mc_param.speed_handle.pid),0.01f,0.08f,1.0,D_MAX_VAL,D_MIN_VAL);
    lowfilter_init(&(mc_param.encoder_handle.speedfilter),15.0f);
    USER_DEBUG_NORMAL("D Kp%.04f Ki%.04f\n",temp.fbuf[0],temp.fbuf[1]);
    USER_DEBUG_NORMAL("Q Kp%.04f Ki%.04f\n",mc_param.currment_handle.q_pid.kp,mc_param.currment_handle.q_pid.ki);
#if 1
    /**/
    smo_init(&(mc_param.currment_handle.ti_smo));
#endif




}
static void mc_param_deinit(void)
{
    memset(&mc_param,0,sizeof(mc_param));
    memset(&motordebug,0,sizeof(motordebug));
}

