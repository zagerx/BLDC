#include "./motorctrl.h"
#include "debuglog.h"
#include "motorctrl_common.h"
#include "mc_protocol.h"
#include "board.h"
#include "string.h"
#include "mc_torquemode.h"
#include "mc_currmentmode.h"
#include "mc_speedmode.h"
#include "mc_utils.h"
#include "mc_angle.h"
#include "fsm.h"
#include "motor_debugmode.h"
#include "initmodule.h"

#define CURRMENT_PERIOD      (0.000125f)
#define TOTAL_DISTANCE       (_2PI)
#define TOTAL_TIME           (6.0f)
#define TOTAL_OMEGA          (TOTAL_DISTANCE/TOTAL_TIME)


static fsm_rt_t motor_normalmode(fsm_cb_t *pthis);
static fsm_rt_t motor_idlemode(fsm_cb_t *pthis);
static void mc_param_init(void);
 void mc_param_deinit(void);
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

    motorprotocol_process();
    DISPATCH_FSM(&pmotor_fsm);
}

void mc_hightfreq_task(float *iabc)
{
#if 1
    mc_test(iabc,TOTAL_OMEGA);
#else

    duty_t duty = {0};
    /*获取角度 速度*/
    // int32_t raw = ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_RAW))[0];
    int32_t raw = *((int32_t*)sensor_user_read(SENSOR_01));

	float theta = 0.0f;
    float next_theta = 0.0f;
    float speed = 0.0f;
    mc_encoder_readspeedangle(&raw,&theta,&speed);
    next_theta = theta + 1.5f * CURRMENT_PERIOD * speed;
    motordebug.ele_angle = theta;
    motordebug.real_speed = speed;
    duty = currment_loop(iabc,theta,next_theta);
    motor_set_pwm(duty._a,duty._b,duty._c);
#endif
}

static void mc_param_init(void)
{
    // pid_init(&(mc_param.daxis_pi),0.0273f,166.2507f * CURRMENT_PERIOD,1.0f,D_MAX_VAL,D_MIN_VAL);
    // pid_init(&(mc_param.qaxis_pi),0.0273f,166.2507f * CURRMENT_PERIOD,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mc_param.daxis_pi),0.1,0.01f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mc_param.qaxis_pi),0.1,0.01f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mc_param.speedloop_pi),0.9f,0.1f,1.0f,D_MAX_VAL,D_MIN_VAL);
    // lowfilter_init(&(mc_param.elefitler[0]),80);
    // lowfilter_init(&(mc_param.elefitler[1]),80);
    // lowfilter_init(&(mc_param.elefitler[2]),80);
}

 void mc_param_deinit(void)
{
    memset(&mc_param,0,sizeof(mc_param));
    memset(&motordebug,0,sizeof(motordebug));
}

static fsm_rt_t motor_idlemode(fsm_cb_t *pthis)
{
    switch (pthis->chState)
    {
    case ENTER:
        if(!strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_DEBUGMODE].cmd)))
        {
            TRAN_STATE(pthis,motor_debugmode);
        }
        if(!strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_NORMALMODE].cmd)))
        {
            TRAN_STATE(pthis,motor_normalmode);
        }
        break;
    case EXIT:
        break;
    default:
        break;
    }
}



static fsm_rt_t motor_normalmode(fsm_cb_t *pthis)
{
    enum{
        MOTOR_INIT = USER,
        MOTOR_START, 
        MOTOR_RUNING,
        MOTOR_STOP,
    };
    switch (pthis->chState)
    {
    case ENTER:
        mc_param_deinit();
        pthis->chState = MOTOR_INIT;
    case MOTOR_INIT:
        if(strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_START].cmd)))
        {
            break;
        }
        mc_param_init();
        pthis->chState = MOTOR_START;
    case MOTOR_START:
        motor_enable();
        pthis->chState = MOTOR_RUNING;
        motorprotocol_transmit(sg_commandmap[CMD_SET_START].res_cmd,strlen(sg_commandmap[CMD_SET_START].res_cmd));
        break;

    case MOTOR_RUNING:
        {
            // dq_t idq = mc_update_torque(0.1f);//更新iq限制值
            // motordebug.id_targe = idq.d;
            // motordebug.iq_targe = idq.q;
            dq_t idq = {0.0f};
            idq.q = speed_loop(10.0f);
            motordebug.speed_targe = 10.0f;
            motordebug.id_targe = idq.d;
            motordebug.iq_targe = idq.q;
        }

        float vbus = *((float*)sensor_user_read(SENSOR_02));
        static short falut_cnt = 0;
        if (vbus > 22.0f)
        {
            falut_cnt = 0;
        }else{
            falut_cnt++;
            if (falut_cnt > 10)
            {
                falut_cnt = 0;
                motor_disable();
                pthis->chState = EXIT;
            }
        }

        motordebug.vbus = vbus;

        if(strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_START].cmd)))
        {
            motorprotocol_transmit(sg_commandmap[CMD_SET_STOP].res_cmd,strlen(sg_commandmap[CMD_SET_STOP].res_cmd));
            pthis->chState = EXIT;
        }
        break;

    case EXIT:
        motor_disable();
        mc_param_deinit();
        pthis->chState = ENTER;

        USER_DEBUG_NORMAL("exit normalmode\r\n");
        break;
    default:
        break;
    }
}

board_init(motorctrl_init)