#include "mc_protocol.h"
#include "mc_currmentloop.h"
#include "mc_utils.h"
#include "mc_angle.h"
#include "motor_speedmode.h"
#include "motorctrl_cfg.h"
#include "motorctrl_common.h"

#include "board.h"
#include "sensor.h"
#include "fsm.h"
#include "taskmodule.h"
#include "initmodule.h"
#include "debuglog.h"
#include "string.h"


static fsm_rt_t motor_idlemode(fsm_cb_t *pthis);

static fsm_cb_t pmotor_fsm;
mc_param_t mc_param = {0};
motordebug_t motordebug = {0};


void motorctrl_init(void)
{
    pmotor_fsm.fsm = (fsm_t *)motor_idlemode;
    pmotor_fsm.count = 0;
    pmotor_fsm.chState = ENTER;
}


void motortctrl_process(void)
{
    DISPATCH_FSM(&pmotor_fsm);
    static unsigned short cnt = 0;
    if (cnt++>200)
    {
        cnt = 0;
        mc_protocol_send(S_HeartP,NULL,0,0,0);
        mc_protocol_sendspeed();
    }
}

void mc_hightfreq_task(float *iabc)
{
#ifdef MOTOR_OPENLOOP
    mc_test(iabc);
#else
    duty_t duty = {0};
    /*获取角度 速度*/
    int32_t raw = *((int32_t*)sensor_user_read(SENSOR_01));
	float theta = 0.0f;
    float next_theta = 0.0f;
    float speed = 0.0f;

    mc_param.encoder_handle.raw_data = raw;
    mc_encoder_read(&(mc_param.encoder_handle));
    theta = mc_param.encoder_handle.ele_theta;
    speed = mc_param.encoder_handle.speed;

    next_theta = theta + 1.5f * CURRMENT_PERIOD * speed;
    motordebug.ele_angle = theta;
    motordebug.speed_real = speed;
    
    mc_param.currment_handle.i_abc[0] = iabc[0];
    mc_param.currment_handle.i_abc[1] = iabc[1];
    mc_param.currment_handle.i_abc[2] = iabc[2];
    mc_param.currment_handle.theta = theta;
    mc_param.currment_handle.next_theta = next_theta;
    duty = currment_loop(&(mc_param.currment_handle));
    motor_set_pwm(duty._a,duty._b,duty._c);
#endif
}

static fsm_rt_t motor_idlemode(fsm_cb_t *pthis)
{
    switch (pthis->chState)
    {
    case ENTER:
        if (motordebug.rec_cmd == M_SET_SpeedM)
        {
            TRAN_STATE(pthis,motor_speedmode);
        }
        break;
    case EXIT:
        break;
    default:
        break;
    }
    return 0;
}

board_init(motorctrl_init)
board_task(motortctrl_process)
