
#include "motorctrl_common.h"
#include "string.h"
#include "mc_encoder.h"
#include "board.h"
#include "motorctrl_cfg.h"
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

unsigned char TEST_Fuc(void)
{
    duty_t duty = {0};
    dq_t idq = {0.0f,0.04f};
    idq.d = -TOTAL_Te;    
    idq.q = 0.00f;    
    alpbet_t temp_ab = {0};
    static float theta = 0.0f;
    motordebug.self_ele_theta = theta;
    temp_ab = _2r_2s(idq,theta);
    duty = SVM(temp_ab.alpha,temp_ab.beta);
    motor_set_pwm(duty._a,duty._b,duty._c);   
    theta += STEP_THETA;
    if (theta >= _2PI * MOTOR_PAIRS)
    {
        theta = 0.0f;
        return 1;
    }
    return 0;
}

static void mc_self_openlooptest(float *iabc)
{
    static unsigned short cnt = 0;
    float next_theta,theta;
    dq_t idq = {0.0f,0.04f};
    idq.q = TOTAL_Te;
    alpbet_t temp_ab = {0};
    duty_t duty = {0};
    alpbet_t i_ab;
    dq_t i_dq;
    abc_t i_abc = {0};
    i_abc.a = iabc[0];
    i_abc.b = iabc[1];
    i_abc.c = iabc[2];
    // mc_encoder_t encoder;
    mc_encoder_read(&(mc_param.encoder_handle));
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

        // theta = TOTAL_OMEGA * CURRMENT_PERIOD * cnt;

        // _3s_2s(i_abc,&i_ab);
        // _2s_2r(i_ab,theta*MOTOR_PAIRS,&i_dq);
        // motordebug.ia_real = i_abc.a;
        // motordebug.ib_real = i_abc.b;
        // motordebug.ic_real = i_abc.c;
        // motordebug.id_real = i_dq.d;
        // motordebug.iq_real = i_dq.q;
        // next_theta = TOTAL_OMEGA * CURRMENT_PERIOD * (cnt+1) * MOTOR_PAIRS;
        // temp_ab = _2r_2s(idq,next_theta);
        // duty = SVM(temp_ab.alpha,temp_ab.beta);
        // motor_set_pwm(duty._a,duty._b,duty._c);
        // cnt++;
        // if (theta > _2PI)
        // {
        //     state = STOP;
        //     break;
        // }
        if (TEST_Fuc())
        {
            state = STOP;
            break;
        }
        
        break;

    case STOP:
        cnt = 0;
        state = PREPOSITIONING;
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
    float speed = 0.0f;
    mc_encoder_t encoder;
    mc_encoder_read(&(encoder));
    speed = encoder.speed;
    theta = encoder.ele_theta;
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

