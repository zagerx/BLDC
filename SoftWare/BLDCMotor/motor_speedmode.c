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
fsm_rt_t motor_speedmode(fsm_cb_t *pthis)
{
    flash_t temp;

    enum{
        READY = USER,
        RUN,
    };
    switch (pthis->chState)
    {
    case ENTER:
#if  !(defined(MOTOR_OPENLOOP) || defined(MOTOR_OPENLOOP_ENCODER))   
        /*从FLASH指定位置读取PID参数数据*/
        user_flash_read(PID_PARSE_ADDR,(uint8_t *)&temp,PID_PARSE_SIZE);
        /*初始化PID参数*/
        pid_init(&(mc_param.currment_handle.d_pid),temp.fbuf[0],temp.fbuf[1],1.0,D_MAX_VAL,D_MIN_VAL);
        pid_init(&(mc_param.currment_handle.q_pid),temp.fbuf[0],temp.fbuf[1],1.0,D_MAX_VAL,D_MIN_VAL);       
        pid_init(&(mc_param.speed_handle.pid),0.01f,0.001f,1.0,D_MAX_VAL,D_MIN_VAL);
        USER_DEBUG_NORMAL("D Kp%.04f Ki%.04f\n",temp.fbuf[0],temp.fbuf[1]);
        USER_DEBUG_NORMAL("Q Kp%.04f Ki%.04f\n",mc_param.currment_handle.q_pid.kp,mc_param.currment_handle.q_pid.ki);
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
            mc_param.speed_handle.tar = motordebug.speed_targe;
            mc_param.speed_handle.real = motordebug.speed_real;
            motordebug.iq_targe = speed_loop(&(mc_param.speed_handle));
            motordebug.id_targe = 0.0f;
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
void mc_param_deinit(void)
{
    memset(&mc_param,0,sizeof(mc_param));
    memset(&motordebug,0,sizeof(motordebug));
}

#ifdef MOTOR_OPENLOOP
    #include "sensor.h"
    // #define CURRMENT_PERIOD      (0.000125f)
    #define TOTAL_DISTANCE       (_2PI)
    #define TOTAL_TIME           (6.0f)
    #define TOTAL_OMEGA          (TOTAL_DISTANCE/TOTAL_TIME)
    static void mc_self_openlooptest(float *iabc);
    static void mc_encoderopenlooptest(float *iabc);
    void mc_test(float *iabc)
    {
        #ifndef MOTOR_OPENLOOP_ENCODER
            mc_self_openlooptest(iabc);
        #else
            mc_encoderopenlooptest(iabc);
        #endif
    }
    static void mc_self_openlooptest(float *iabc)
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

        int32_t raw = *((int32_t*)sensor_user_read(SENSOR_01));
        // float xtheta = 0.0f;
        // float speed = 0.0f;
        mc_param.encoder_handle.raw_data = raw;
        mc_encoder_read(&(mc_param.encoder_handle));
        // xtheta = mc_param.encoder_handle.ele_theta;
        // speed = mc_param.encoder_handle.speed;
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

            theta = TOTAL_OMEGA * CURRMENT_PERIOD * cnt;

            _3s_2s(i_abc,&i_ab);
            _2s_2r(i_ab,theta*7.0f,&i_dq);
            motordebug.ia_real = i_abc.a;
            motordebug.ib_real = i_abc.b;
            motordebug.ic_real = i_abc.c;
            motordebug.id_real = i_dq.d;
            motordebug.iq_real = i_dq.q;
            next_theta = TOTAL_OMEGA * CURRMENT_PERIOD * (cnt+1) * 7.0f;
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
    static void mc_encoderopenlooptest(float *iabc)
    {
        float theta = 0.0f;
        float next_theta = 0.0f;
        dq_t idq = {0.0f,-0.04f};
        alpbet_t temp_ab = {0};
        duty_t duty = {0};
        alpbet_t i_ab;
        dq_t i_dq;    
        int32_t raw = *((int32_t*)sensor_user_read(SENSOR_01));
        float speed = 0.0f;
        mc_param.encoder_handle.raw_data = raw;
        mc_encoder_read(&(mc_param.encoder_handle));
        speed = mc_param.encoder_handle.speed;
        theta = mc_param.encoder_handle.ele_theta;
        motordebug.ele_angle = theta;
        abc_t i_abc = {0};
        i_abc.a = iabc[0];
        i_abc.b = iabc[1];
        i_abc.c = iabc[2];
        _3s_2s(i_abc,&i_ab);
        _2s_2r(i_ab,theta,&i_dq);
        motordebug.ia_real = i_abc.a;
        motordebug.ib_real = i_abc.b;
        motordebug.ic_real = i_abc.c;
        motordebug.id_real = i_dq.d;
        motordebug.iq_real = i_dq.q;
        next_theta = theta + (1.5f * speed * CURRMENT_PERIOD);//TODO
        temp_ab = _2r_2s(idq,next_theta);    

        duty = SVM(temp_ab.alpha,temp_ab.beta);
        motor_set_pwm(duty._a,duty._b,duty._c);
    }

#endif