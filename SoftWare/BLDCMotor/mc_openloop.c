
#include "motorctrl_common.h"
#include "string.h"
#include "mc_encoder.h"
#include "board.h"
#include "motorctrl_cfg.h"


static void mc_self_openlooptest(float *iabc,motor_t* motor);
static void mc_encoderopenlooptest(float *iabc,motor_t* motor);

void mc_openloop(float *iabc,motor_t* motor)
{
#ifndef MOTOR_OPENLOOP_ENCODER
    mc_self_openlooptest(iabc,motor);
#else
    mc_encoderopenlooptest(iabc,motor);
#endif
}

unsigned char mc_self_openloop_VF(float *iabc,motor_t* motor)
{
    duty_t duty = {0};
    alpbet_t i_ab;
    dq_t i_dq;
    abc_t i_abc = {0};
    static float theta = 0.0f;

    /*读取编码器角度值*/
    motor->encoder_handle.self_theta = theta;
    mc_encoder_read(&(motor->encoder_handle)); 
    /*反PARK CLARK变换*/
    i_abc.a = iabc[0];
    i_abc.b = iabc[1];
    i_abc.c = iabc[2];
    _3s_2s(i_abc, &i_ab);
    _2s_2r(i_ab, theta, &i_dq);

    /*强制拖动电机转动*/
    i_dq.d = OPENLOOP_DEBUG_TOTAL_Te; // D轴强拖。注意:应和预定位阶段保持一致
    i_dq.q = 0.00f; 

    /*park变换*/
    i_ab = _2r_2s(i_dq, theta);
    /*SVPWM*/
    duty = SVM(i_ab.alpha, i_ab.beta);
    motor->setpwm(duty._a, duty._b, duty._c);

    theta += OPENLOOP_DEBUG_STEP_THETA;
    if (theta > _2PI)
    {
        theta = 0.0f;
        return 1;
    }
    if (theta < 0.0f)
    {
        theta = _2PI;
        return 1;
    }
    
    return 0;
}

static void mc_self_openlooptest(float *iabc,motor_t* motor)
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
        i_dq.d = OPENLOOP_DEBUG_TOTAL_Te; // D轴强拖。注意:应和预定位阶段保持一致
        i_dq.q = 0.00f;
        theta = 0.0f;
        iab = _2r_2s(i_dq, theta);
        duty = SVM(iab.alpha, iab.beta);
        motor->setpwm(duty._a, duty._b, duty._c);
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
        mc_self_openloop_VF(iabc,motor);
        break;

    case STOP:
        state = RUNING;
        break;
    default:
        break;
    }
}

static void mc_encoderopenlooptest(float *iabc,motor_t* motor)
{
    float theta = 0.0f;
    abc_t i_abc;
    dq_t i_dq;

    duty_t duty;
    alpbet_t i_ab;
    dq_t udq;
    alpbet_t temp_ab;

    enum{
        IDLE=0,
        RUN
    };
    static uint16_t state = RUN;
    switch (state)
    {
    case IDLE:
        duty = SVM(0.08f, 0);
        motor->setpwm(duty._a, duty._b, duty._c);
        mc_encoder_read(&(motor->encoder_handle));
        static uint32_t cnt = 0;
        if (cnt++>8000)
        {
            cnt = 0;
            state = RUN;
        }
        break;
    case RUN:
        mc_encoder_read(&(motor->encoder_handle));
        theta = motor->encoder_handle.ele_theta;
        i_abc.a = iabc[0];i_abc.b = iabc[1];i_abc.c = iabc[2];
        _3s_2s(i_abc, &i_ab);
        _2s_2r(i_ab, theta, &i_dq);
        motor->currment_handle.i_debugd = i_dq.d;
        motor->currment_handle.i_debugq = i_dq.q;
        udq.d = motor->currment_handle.id_tar;udq.q = motor->currment_handle.iq_tar;//OPENLOOP_DEBUG_TOTAL_Te;//
        temp_ab = _2r_2s(udq, theta);
        duty = SVM(temp_ab.alpha, temp_ab.beta);
        motor->setpwm(duty._a, duty._b, duty._c);
        break;
    default:
        break;
    }
}



