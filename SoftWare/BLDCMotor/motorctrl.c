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
#define CURRMENT_PERIOD      (0.000125f)
#define TOTAL_DISTANCE       (_2PI)
#define TOTAL_TIME           (6.0f)
#define TOTAL_OMEGA          (TOTAL_DISTANCE/TOTAL_TIME)

enum{
    MOTOR_INIT,
    MOTOR_START, 
    MOTOR_RUNING,
    MOTOR_STOP,
};

mc_param_t mc_param = {0};
motordebug_t motordebug = {0};

static void mc_param_init(void);
static void mc_param_deinit(void);
static void mc_test(float *iabc,float omega);
fsm_rt_t motor_debugmode(fsm_cb_t *pthis);
fsm_rt_t motor_normalmode(fsm_cb_t *pthis);

static fsm_cb_t debugmode_cb = {
    .chState = START,
    .fsm = motor_debugmode,
};
static fsm_cb_t pmotor_fsm;
void motortctrl_process(void)
{
    static uint16_t _state = MOTOR_INIT;
    static unsigned char flag = 0;
    if (!flag)
    {
        flag = 1;
        pmotor_fsm.fsm = debugmode_cb.fsm;
        return;
    }
    DISPATCH_FSM(&pmotor_fsm);
    motorprotocol_process();
    switch (_state)
    {
    case MOTOR_INIT:
        if(strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_START].cmd)))
        {
            break;
        }
        mc_param_init();
        _state = MOTOR_START;
    case MOTOR_START:
        motor_enable();
        _state = MOTOR_RUNING;
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

        int32_t cov = ((int32_t*)sensor_user_read(SENSOR_02,EN_SENSORDATA_COV))[0];
        float vbus = (float)((float)cov / (1<<15));
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
                _state = MOTOR_STOP;
            }
        }

        motordebug.vbus = vbus;

        if(strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_START].cmd)))
        {
            motorprotocol_transmit(sg_commandmap[CMD_SET_STOP].res_cmd,strlen(sg_commandmap[CMD_SET_STOP].res_cmd));
            _state = MOTOR_STOP;
        }
        break;

    case MOTOR_STOP:
        motor_disable();
        mc_param_deinit();
        _state = MOTOR_INIT;
        break;
    default:
        break;
    }
}

void mc_hightfreq_task(float *iabc)
{
#if 0    
    mc_test(iabc,TOTAL_OMEGA);
#else

    duty_t duty = {0};
    /*获取角度 速度*/
    int32_t raw = ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_RAW))[0];
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
void mc_test(float *iabc,float omega)
{
    static unsigned short cnt = 0;

	float theta = 0.0f;
    float next_theta = 0.0f;
    dq_t idq = {0.0f,0.04f};
    alpbet_t temp_ab = {0};
    duty_t duty = {0};
    alpbet_t i_ab;
    dq_t i_dq;

    abc_t i_abc = {0};
	i_abc.a = iabc[0];
	i_abc.b = iabc[1];
	i_abc.c = iabc[2];


    enum{
        PREPOSITIONING,
        RUNING,
        STOP
    };
    static unsigned short state = PREPOSITIONING;
    switch (state)
    {

    case PREPOSITIONING:
    {
        next_theta = 0.0f;
        temp_ab = _2r_2s(idq,next_theta);
        duty = SVM(temp_ab.alpha,temp_ab.beta);
        motor_set_pwm(duty._a,duty._b,duty._c);
        if (cnt++ > 8000)
        {
            cnt = 0;
            state = RUNING;
        }
    }
    break;

    case RUNING:

        theta = omega * CURRMENT_PERIOD * cnt;

        _3s_2s(i_abc,&i_ab);
        _2s_2r(i_ab,theta*7.0f,&i_dq);
        motordebug.ia_real = i_abc.a;
        motordebug.ib_real = i_abc.b;
        motordebug.ic_real = i_abc.c;
        motordebug.id_real = i_dq.d;
        motordebug.iq_real = i_dq.q;
        next_theta = omega * CURRMENT_PERIOD * (cnt+1) * 7.0f;
        motordebug.ele_angle = next_theta;
        temp_ab = _2r_2s(idq,next_theta);
        duty = SVM(temp_ab.alpha,temp_ab.beta);
        motor_set_pwm(duty._a,duty._b,duty._c);
        cnt++;
        if (theta > _2PI)
        {
            state = STOP;
            break;
        }        
        break;

    case STOP:

        if(strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_START].cmd)))
        {
            cnt = 0;
            state = PREPOSITIONING;
            break;
        }

        break;
    default:
        break;
    }

}

static void mc_param_init(void)
{
    pid_init(&(mc_param.daxis_pi),0.0273f,166.2507f * CURRMENT_PERIOD,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mc_param.qaxis_pi),0.0273f,166.2507f * CURRMENT_PERIOD,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mc_param.speedloop_pi),0.9f,0.1f,1.0f,D_MAX_VAL,D_MIN_VAL);
    // lowfilter_init(&(mc_param.elefitler[0]),80);
    // lowfilter_init(&(mc_param.elefitler[1]),80);
    // lowfilter_init(&(mc_param.elefitler[2]),80);
}

static void mc_param_deinit(void)
{
    memset(&mc_param,0,sizeof(mc_param));
    memset(&motordebug,0,sizeof(motordebug));
}



motor_t* motor_table[2] = {0};
void motor_register(motor_t *motor)
{
    motor_table[0] = motor;
}


fsm_rt_t motor_debugmode(fsm_cb_t *pthis)
{
    enum{
        IDLE = USER,
    };
    switch (pthis->chState)
    {
    case START:
        USER_DEBUG_NORMAL("enter debugmode\r\n");
        pthis->chState = IDLE;
        break;
    case IDLE:
        if (pthis->count++ > 500)
        {
            pthis->count = 0;
            TRAN_STATE(pthis,motor_normalmode);
        }
        
        break;
    case EXIT:
        USER_DEBUG_NORMAL("exit debugmode\r\n");
        break;
    default:
        break;
    }
}
fsm_rt_t motor_normalmode(fsm_cb_t *pthis)
{
    enum{
        IDLE = USER,
    };    
    switch (pthis->chState)
    {
    case START:
        USER_DEBUG_NORMAL("enter normalmode\r\n");
        pthis->chState = IDLE;
        break;
    case IDLE:
        if (pthis->count++ > 500)
        {
            pthis->count = 0;
            TRAN_STATE(pthis,motor_debugmode);
        }
        
        break;

    case EXIT:
        USER_DEBUG_NORMAL("exit normalmode\r\n");
        break;
    default:
        break;
    }
}

