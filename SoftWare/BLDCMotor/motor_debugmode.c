#include "motor_debugmode.h"
#include "mc_protocol.h"
#include "motorctrl_common.h"
#include "string.h"

#define CURRMENT_PERIOD      (0.000125f)
#define TOTAL_DISTANCE       (_2PI)
#define TOTAL_TIME           (6.0f)
#define TOTAL_OMEGA          (TOTAL_DISTANCE/TOTAL_TIME)


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

        if (motordebug.rec_cmd == M_SET_START)
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

fsm_rt_t motor_debugmode(fsm_cb_t *pthis)
{
    enum{
        READY = USER,
        RUN,
    };
    switch (pthis->chState)
    {
    case ENTER:
        {
            break;
        }
        USER_DEBUG_NORMAL("motor enable\n");
        pid_init(&(mc_param.daxis_pi),motordebug.pid_kp,motordebug.pid_ki,1.0f,D_MAX_VAL,D_MIN_VAL);
        pthis->chState = READY;
    case READY:
        {
            break;
        }
        motordebug.id_targe = motordebug.pid_tar;
        motor_enable();
        pthis->chState = RUN;
        break;
    case RUN:
        
        {
            int32_t cov;// = ((int32_t*)sensor_user_read(SENSOR_02,EN_SENSORDATA_COV))[0];
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
                    pthis->chState = EXIT;
                }
            }           
            motordebug.vbus = vbus;
        }

        // if(!strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_STOP].cmd)))
        {
            pthis->chState = EXIT;
        }
        break;    
    case EXIT:
        USER_DEBUG_NORMAL("motor disable\n");
        motor_disable();
        mc_param_deinit();
        pthis->chState = ENTER;
        break;
    default:
        break;
    }
    return 0;
}
