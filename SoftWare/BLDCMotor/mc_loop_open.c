
#include "motorctrl_common.h"
#include "string.h"
#include "mc_encoder.h"
#include "board.h"
#include "mc_focmath.h"
#include "math.h"
/*==========================================================================================
 * @brief        VF开环强拖
 * @FuncName     mc_self_openloop_VF
 * @param        iabc 
 * @param        motor 
 * @return       unsigned char 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
unsigned char mc_self_openloop_VF(float *iabc,motor_t* motor)
{
    float duty[3] = {0};
    float i_ab[2];
    float i_dq[2];
    static float theta = 0.0f;

    /*读取编码器角度值*/
    mc_encoder_read(&(motor->encoder_handle)); 
    /*反PARK CLARK变换*/
    _3s_2s(iabc, i_ab);
    _2s_2r(i_ab, theta, i_dq);

    /*强制拖动电机转动*/
    i_dq[0] = OPENLOOP_DEBUG_TOTAL_Te; // D轴强拖。注意:应和预定位阶段保持一致
    i_dq[1] = 0.00f; 

    /*park变换*/
    float temp;
    temp = fmodf(theta*MOTOR_PAIRS,6.28f);
    motor->encoder_handle.self_theta = temp;
    _2r_2s(i_dq, temp,i_ab);
    /*SVPWM*/
    SVM(i_ab[0], i_ab[1],duty);
    motor->setpwm(duty[0], duty[1], duty[2]);

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
/*==========================================================================================
 * @brief        开环自测函数
 * @FuncName     mc_self_openlooptest
 * @param        iabc 
 * @param        motor 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void mc_self_openlooptest(float *iabc,motor_t* motor)
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
        motor->encoder_handle.runflag = 1;

        float theta;
        float iab[2];
        float duty[3];
        float i_dq[2];
        i_dq[0] = OPENLOOP_DEBUG_TOTAL_Te; // D轴强拖。注意:应和预定位阶段保持一致
        i_dq[1] = 0.00f;
        theta = 0.0f;
        _2r_2s(i_dq, theta,iab);
        SVM(iab[0], iab[1],duty);
        motor->setpwm(duty[0], duty[1], duty[2]);
        // 延时等待一段时间
        static unsigned short cnt = 0;
        if (cnt++ > 16000)
        {
            cnt = 0;
            state = RUNING;
            motor->encoder_handle.sensor->init((motor->encoder_handle.sensor));
            motor->encoder_handle.sensor->get_first_points((motor->encoder_handle.sensor));
            motor->encoder_handle.sensor->set_calib_points((motor->encoder_handle.sensor));
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
