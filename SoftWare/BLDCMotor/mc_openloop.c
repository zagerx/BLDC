
#include "motorctrl_common.h"
#include "string.h"
#include "mc_encoder.h"
#include "board.h"
#include "motorctrl_cfg.h"

#define VF_MODE_D 

static void mc_self_openlooptest(float *iabc);
static void mc_encoderopenlooptest(float *iabc);

void mc_openloop(float *iabc)
{
#ifndef MOTOR_OPENLOOP_ENCODER
    mc_self_openlooptest(iabc);
#else
    mc_encoderopenlooptest(iabc);
#endif
}

unsigned char mc_self_openloop_VF(float *iabc)
{
    duty_t duty = {0};
    alpbet_t i_ab;
    dq_t i_dq;
    abc_t i_abc = {0};
    static float theta = 0.0f;

    /*读取编码器角度值*/
    mc_encoder_read(&(mc_param.encoder_handle)); 
    /*反PARK CLARK变换*/
    i_abc.a = iabc[0];
    i_abc.b = iabc[1];
    i_abc.c = iabc[2];
    _3s_2s(i_abc, &i_ab);
    _2s_2r(i_ab, theta, &i_dq);
    /*更新电流值，方便观察*/
    motordebug.ia_real = i_abc.a;
    motordebug.ib_real = i_abc.b;
    motordebug.ic_real = i_abc.c;
    motordebug.id_real = i_dq.d;
    motordebug.iq_real = i_dq.q;
    motordebug.self_ele_theta = theta;
    /*强制拖动电机转动*/
#ifdef VF_MODE_D
    i_dq.d = OPENLOOP_DEBUG_TOTAL_Te; // D轴强拖。注意:应和预定位阶段保持一致
    i_dq.q = 0.00f;
#else
    i_dq.d = 0.0f; // D轴强拖。注意:应和预定位阶段保持一致
    i_dq.q = OPENLOOP_DEBUG_TOTAL_Te;    
#endif // DEBUG    

    /*park变换*/
    i_ab = _2r_2s(i_dq, theta);
    /*SVPWM*/
    duty = SVM(i_ab.alpha, i_ab.beta);
    motor_set_pwm(duty._a, duty._b, duty._c);

    theta += OPENLOOP_DEBUG_STEP_THETA;
    if (theta > _2PI || theta < -_2PI)
    {
        theta = 0.0f;
        return 1;
    }
    return 0;
}

static void mc_self_openlooptest(float *iabc)
{
    enum
    {
        PREPOSITIONING,
        RUNING,
        STOP
    };
    static unsigned short state = PREPOSITIONING;
    switch (state)
    {
    case PREPOSITIONING:
    {
        float theta;
        alpbet_t iab;
        duty_t duty;
        dq_t i_dq;
#ifdef VF_MODE_D
    i_dq.d = OPENLOOP_DEBUG_TOTAL_Te; // D轴强拖。注意:应和预定位阶段保持一致
    i_dq.q = 0.00f;
#else
    i_dq.d = 0.0f; 
    i_dq.q = OPENLOOP_DEBUG_TOTAL_Te; // Q轴强拖。注意:应和预定位阶段保持一致
#endif
        theta = 0.0f;
        iab = _2r_2s(i_dq, theta);
        duty = SVM(iab.alpha, iab.beta);
        motor_set_pwm(duty._a, duty._b, duty._c);
        // 延时等待一段时间
        static unsigned short cnt = 0;
        if (cnt++ > 16000)
        {
            cnt = 0;
            state = RUNING;
        }
    }
    break;

    case RUNING:
        mc_self_openloop_VF(iabc);
        break;

    case STOP:
        state = RUNING;
        break;
    default:
        break;
    }
}

static void mc_encoderopenlooptest(float *iabc)
{
    float theta = 0.0f;
    abc_t i_abc;
    dq_t i_dq;

    duty_t duty;
    alpbet_t i_ab;
    dq_t idq;
    alpbet_t temp_ab;
    mc_encoder_read(&(mc_param.encoder_handle));
    theta = mc_param.encoder_handle.ele_theta;
    i_abc.a = iabc[0];
    i_abc.b = iabc[1];
    i_abc.c = iabc[2];
    _3s_2s(i_abc, &i_ab);
    _2s_2r(i_ab, theta, &i_dq);
    motordebug.ia_real = i_abc.a;
    motordebug.ib_real = i_abc.b;
    motordebug.ic_real = i_abc.c;
    motordebug.id_real = i_dq.d;
    motordebug.iq_real = i_dq.q;

    idq.d = 0.0f;idq.q = OPENLOOP_DEBUG_TOTAL_Te;
    temp_ab = _2r_2s(idq, theta);
    duty = SVM(temp_ab.alpha, temp_ab.beta);
    motor_set_pwm(duty._a, duty._b, duty._c);
}



