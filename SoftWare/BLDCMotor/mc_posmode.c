#include "mc_posmode.h"
#include "mc_posloop.h"
#include "mc_protocol.h"
#include "motorctrl_common.h"
#include "string.h"
#include "mc_encoder.h"
#include "flash.h"
#include "mc_speedloop.h"
#include "board.h"
#include "motorctrl_cfg.h"
#include "debuglog.h"



static void mc_param_init(void);
static void mc_param_deinit(void);
void mc_tarpos_update(float delte_dist)
{
    mc_pos_t *pos;
    pos = &(mc_param.pos_handle);
    pos->total_dist += delte_dist;
    pos->tar = pos->total_dist;
}

fsm_rt_t motor_posmode(fsm_cb_t *pthis)
{
    enum{
        READY = USER,
        RUN,
    };
    switch (pthis->chState)
    {
    case ENTER:
        USER_DEBUG_NORMAL("motor enter position model\n");
        mc_param_init();
        pthis->chState = READY;
    case READY:
        if (motordebug.rec_cmd != M_SET_START)
        {
            break;
        }
        motor_enable();
        /*更新起始位置*/
        // mc_tarpos_update(0.0f);
        pthis->chState = RUN;
        break;
    case RUN:
        if (motordebug.rec_cmd == M_SET_STOP)
        {
            pthis->chState = EXIT;
        }else{
            mc_posloop(&(mc_param.pos_handle));
        }
        break;    
    case EXIT:
        motor_disable();
        mc_param_deinit();
        pthis->chState = ENTER;    
        USER_DEBUG_NORMAL("motor exit position model\n");
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
    pid_init(&(mc_param.currment_handle.d_pid),temp.fbuf[0],temp.fbuf[1],1.0,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mc_param.currment_handle.q_pid),temp.fbuf[0],temp.fbuf[1],1.0,D_MAX_VAL,D_MIN_VAL);       
    pid_init(&(mc_param.speed_handle.pid),0.01f,0.08f,1.0,D_MAX_VAL,D_MIN_VAL);
    lowfilter_init(&(mc_param.speed_handle.speedfilter),15.0f);
    pid_init(&(mc_param.pos_handle.pi_contrl),0.6f,0.1f,1.0f,4000.0f,-4000.0f);
    USER_DEBUG_NORMAL("D Kp%.04f Ki%.04f\n",temp.fbuf[0],temp.fbuf[1]);
    USER_DEBUG_NORMAL("Q Kp%.04f Ki%.04f\n",mc_param.currment_handle.q_pid.kp,mc_param.currment_handle.q_pid.ki);
}
static void mc_param_deinit(void)
{
    memset(&mc_param,0,sizeof(mc_param));
    memset(&motordebug,0,sizeof(motordebug));
}



