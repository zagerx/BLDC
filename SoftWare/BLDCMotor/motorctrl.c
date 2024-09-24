#include "mc_protocol.h"
#include "mc_currmentloop.h"
#include "mc_utils.h"
#include "mc_encoder.h"
#include "motor_speedmode.h"
#include "motor_normalmode.h"
#include "mc_posmode.h"
#include "motorctrl_cfg.h"
#include "motorctrl_common.h"

#include "board.h"
#include "sensor.h"
#include "fsm.h"
#include "taskmodule.h"
#include "initmodule.h"
#include "debuglog.h"
#include "string.h"

static fsm_cb_t pmotor_fsm;
mc_param_t mc_param = {0};
motordebug_t motordebug = {0};
extern void mc_protocol_nowsend(unsigned short cmd,unsigned char* pdata,unsigned short datalen);

static void motorthread_init(void)
{
    pmotor_fsm.fsm = (fsm_t *)motor_normalmode;
    pmotor_fsm.count = 0;
    pmotor_fsm.chState = ENTER;
}

void motortctrl_process(void)
{
    enum{
        MOTOR_INIT = 0,
        MOTOR_RUNING,
        MOTOR_EXIT
    };
    static short state = MOTOR_INIT;
    switch (state)
    {
    case MOTOR_INIT:
        motorthread_init();
        state = MOTOR_RUNING;
        break;
    case MOTOR_RUNING:
        DISPATCH_FSM(&pmotor_fsm);
        if (motordebug.rec_cmd == M_SET_SpeedM)
        {
            motordebug.rec_cmd = 0;//TODO
            TRAN_STATE(&pmotor_fsm,motor_speedmode);
        }else if(motordebug.rec_cmd == M_SET_NormalM){
            TRAN_STATE(&pmotor_fsm,motor_normalmode);
            motordebug.rec_cmd = 0;
        }else if(motordebug.rec_cmd == M_SET_PosM){
            TRAN_STATE(&pmotor_fsm,motor_posmode);
            motordebug.rec_cmd = 0;            
        }
        break;    
    default:
        break;
    }

    static unsigned short cnt = 0;
    if (cnt++>=1000)
    {
        cnt = 0;
        mc_protocol_send(S_HeartP,NULL,0,0,0);
    }

    static uint8_t cnt1 = 0;
    if (cnt1++>=1)
    {
        cnt1 = 0;
        float fbuf[2];
        fbuf[0] = motordebug.speed_real;
        fbuf[1] = motordebug.ele_angle;
        mc_protocol_nowsend(S_MotorSpeed,(uint8_t *)(&fbuf),8);
    }
}

void mc_hightfreq_task(float *iabc)
{
#ifdef MOTOR_OPENLOOP
    mc_test(iabc);
#else
    duty_t duty = {0};
    /*获取角度 速度*/
	float theta = 0.0f;
    float next_theta = 0.0f;
    float speed = 0.0f;

    mc_encoder_read(&(mc_param.encoder_handle));
    theta = mc_param.encoder_handle.ele_theta;
    speed = mc_param.encoder_handle.speed;

    next_theta = theta + 1.5f * CURRMENT_PERIOD * speed;
    
    mc_param.currment_handle.i_abc[0] = iabc[0];
    mc_param.currment_handle.i_abc[1] = iabc[1];
    mc_param.currment_handle.i_abc[2] = iabc[2];
    mc_param.currment_handle.theta = theta;
    mc_param.currment_handle.next_theta = next_theta;
    duty = currment_loop(&(mc_param.currment_handle));
    motor_set_pwm(duty._a,duty._b,duty._c);
#endif
}

board_task(motortctrl_process)
