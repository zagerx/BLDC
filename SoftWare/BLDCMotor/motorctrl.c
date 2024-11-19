#include "mc_protocol.h"
#include "mc_currmentloop.h"
#include "mc_utils.h"
#include "mc_encoder.h"
#include "motor_speedmode.h"
#include "motor_normalmode.h"
#include "motor_posmode.h"
#include "motorctrl_cfg.h"
#include "motorctrl_common.h"
#include "mc_openloop.h"
#include "board.h"
#include "fsm.h"
#include "taskmodule.h"
#include "initmodule.h"
#include "debuglog.h"
#include "string.h"

/*
#ifndef VOTF_SOFTWARE
    static unsigned flag = 0;
    if (flag)
    {
        flag  = 0;
        // cnt = 0;
        // mc_protocol_send(S_HeartP,NULL,0,0,0);
        // static uint32_t cout;
        // USER_DEBUG_NORMAL("hear %d\n",cout++);
        mc_protocol_nowsend(S_HeartP,NULL,0);
    }else{
        flag = 1;
        float fbuf[2];
        fbuf[0] = motordebug.speed_real;
        fbuf[1] = motordebug.pos_real;
        mc_protocol_nowsend(S_MotorSpeed,(uint8_t *)(&fbuf),8);        
    }
#endif
*/

static fsm_cb_t MotorFsm;
motor_t motor1 = {0};
extern void mc_protocol_nowsend(unsigned short cmd,unsigned char* pdata,unsigned short datalen);

static void MotorFsm_Init(void)
{
    MotorFsm.fsm = (fsm_t *)motor_normalmode;
    MotorFsm.count = 0;
    MotorFsm.chState = ENTER;
    MotorFsm.pdata = &motor1;
}

void motortctrl_process(void)
{
    static short state = 0;
    switch (state)
    {
    case 0:
        MotorFsm_Init();
        motor_func_register((motor_t*)&motor1);

        USER_DEBUG_NORMAL("MotorThread is Ready\n");
        state = 1;
        break;
    case 1:
        DISPATCH_FSM(&MotorFsm);//模式切换
        break;    
    default:
        break;
    }
}

void mc_hightfreq_task(float *iabc,motor_t *motor)
{
    motor->currment_handle.i_abc[0] = iabc[0];
    motor->currment_handle.i_abc[1] = iabc[1];
    motor->currment_handle.i_abc[2] = iabc[2];
#ifdef MOTOR_OPENLOOP
    mc_openloop(iabc,motor);
#else
    duty_t duty = {0};
    /*获取角度 速度*/
	float theta = 0.0f;
    float next_theta = 0.0f;
    float speed = 0.0f;

    mc_encoder_read(&(motor->encoder_handle));
    theta = motor->encoder_handle.ele_theta;
    speed = motor->encoder_handle.speed;
    next_theta = theta;// + 1.5f * CURRMENT_PERIOD * speed;

    motor->currment_handle.i_abc[0] = iabc[0];
    motor->currment_handle.i_abc[1] = iabc[1];
    motor->currment_handle.i_abc[2] = iabc[2];
    motor->currment_handle.theta = theta;
    motor->currment_handle.next_theta = next_theta;
    duty = currment_loop(&(motor->currment_handle));
    motor->setpwm(duty._a,duty._b,duty._c);
#endif
}






board_task(motortctrl_process)
