#include "motor_posmode.h"
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
    pos = &(motor1.pos_handle);
    pos->total_dist += delte_dist;
    pos->tar = pos->total_dist;
}

fsm_rt_t motor_posmode(fsm_cb_t *pthis)
{
    mc_param_t *motor;
    motor = (mc_param_t*)pthis->pdata;    
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
        if (motor->curmode != M_SET_START)
        {
            break;
        }
        motor1.enable();
        pthis->chState = RUN;
        break;
    case RUN:
        if (motor->curmode == M_SET_STOP)
        {
            pthis->chState = EXIT;
            break;
        }
        motor1.pos_handle.p_speed->real = motor1.encoder_handle.speed;
        motor1.pos_handle.real = motor1.encoder_handle.total_realmectheta;
        mc_posloop(&(motor1.pos_handle));
        break;
    case EXIT:
        motor1.disable();
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
    pid_init(&(motor1.currment_handle.d_pid),0.1f,0.01f,1.0,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(motor1.currment_handle.q_pid),0.1f,0.01f,1.0,D_MAX_VAL,D_MIN_VAL);       
    pid_init(&(motor1.speed_handle.pid),0.01f,0.08f,1.0,D_MAX_VAL,D_MIN_VAL);
    lowfilter_init(&(motor1.encoder_handle.speedfilter),15.0f);


    // pid_init(&(motor1.pos_handle.pi_contrl),temp.fbuf[0],temp.fbuf[1],1.0f,4000.0f,-4000.0f);
    pid_init(&(motor1.pos_handle.pi_contrl),1200.0f,0.02,1.0f,4000.0f,-4000.0f);
    motor1.pos_handle.p_speed = &(motor1.speed_handle);//更新速度环指针
    USER_DEBUG_NORMAL("pos_handle.pi_contrl:Kp=%04f Ki=%04f\n",motor1.pos_handle.pi_contrl.kp,motor1.pos_handle.pi_contrl.ki);
}
static void mc_param_deinit(void)
{
    memset(&motor1,0,sizeof(motor1));
    memset(&motordebug,0,sizeof(motordebug));
}



