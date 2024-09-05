#include "./motorctrl.h"
#include "taskmodule.h"
#include "debuglog.h"
#include "motorctrl_common.h"
#include "mc_protocol.h"
#include "board.h"
#include "string.h"
#include "mc_currmentmode.h"
#include "mc_utils.h"
#include "mc_angle.h"
#include "fsm.h"
#include "motor_debugmode.h"
#include "initmodule.h"
// #include "perf_counter.h"
#define CURRMENT_PERIOD      (0.000125f)
#define TOTAL_DISTANCE       (_2PI)
#define TOTAL_TIME           (6.0f)
#define TOTAL_OMEGA          (TOTAL_DISTANCE/TOTAL_TIME)


extern fsm_rt_t motor_normalmode(fsm_cb_t *pthis);
// extern fsm_rt_t motor_debugmode(fsm_cb_t *pthis);

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
#if 0
    mc_test(iabc,TOTAL_OMEGA);
#else

    duty_t duty = {0};
    /*获取角度 速度*/
    int32_t raw = *((int32_t*)sensor_user_read(SENSOR_01));

	float theta = 0.0f;
    float next_theta = 0.0f;
    float speed = 0.0f;
    mc_encoder_readspeedangle(&raw,&theta,&speed);
    motordebug.ele_angle = theta;
    next_theta = theta + 1.5f * CURRMENT_PERIOD * speed;
    motordebug.ele_angle = theta;
    motordebug.real_speed = speed;

    duty = currment_loop(iabc,theta,next_theta);
    motor_set_pwm(duty._a,duty._b,duty._c);
#endif
}

static fsm_rt_t motor_idlemode(fsm_cb_t *pthis)
{
    switch (pthis->chState)
    {
    case ENTER:
        if (motordebug.rec_cmd == M_SET_NormalM)
        {
            TRAN_STATE(pthis,motor_normalmode);
        }
        if (motordebug.rec_cmd == M_SET_DebugM)
        {
            TRAN_STATE(pthis,motor_debugmode);
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
