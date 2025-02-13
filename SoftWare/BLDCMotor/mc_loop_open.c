
#include "motorctrl_common.h"
#include "string.h"
#include "mc_encoder.h"
#include "board.h"
#include "mc_focmath.h"
#include "math.h"
#include "mc_commend.h"
#include "debuglog.h"

/*==========================================================================================
 * @brief       自开环预定位函数
 * @FuncName     
 * @return       0:预定位成功 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
static int8_t mc_sol_preposition(motor_t* obj)
{
    motor_t* motor = (motor_t*)obj;
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

    if (motor->encoder_handle.self_theta_conut++ < 2000)
    {
        return 1;
    }
    motor->encoder_handle.self_theta_conut = 0;
    motor->encoder_handle.self_mec_theta = 0.0f;
    motor->encoder_handle.sensor->init((motor->encoder_handle.sensor));
    motor->encoder_handle.sensor->get_first_points((motor->encoder_handle.sensor));
    motor->encoder_handle.sensor->set_calib_points((motor->encoder_handle.sensor));
    return 0;
}
/*==========================================================================================
 * @brief        VF开环强拖
 * @FuncName     mc_self_openloop_VF
 * @param        iabc 
 * @param        motor 
 * @return       unsigned char 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void mc_self_openloop_VF(motor_t* motor,float *iabc)
{
    float duty[3] = {0};
    float i_ab[2];
    float i_dq[2];

    /*读取编码器角度值*/
    mc_encoder_read(&(motor->encoder_handle)); 
    /*反PARK CLARK变换*/
    _3s_2s(iabc, i_ab);
    _2s_2r(i_ab, motor->encoder_handle.self_mec_theta, i_dq);
    motor->currment_handle.i_abc[0] = iabc[0];
    motor->currment_handle.i_abc[1] = iabc[1];
    motor->currment_handle.i_abc[2] = iabc[2];
    /*强制拖动电机转动*/
    i_dq[0] = OPENLOOP_DEBUG_TOTAL_Te; // D轴强拖。注意:应和预定位阶段保持一致
    i_dq[1] = 0.00f; 

    /*park变换*/
    float temp;
    temp = fmodf(motor->encoder_handle.self_mec_theta*MOTOR_PAIRS,6.28f);
    motor->encoder_handle.self_theta = temp;
    _2r_2s(i_dq, temp,i_ab);
    /*SVPWM*/
    SVM(i_ab[0], i_ab[1],duty);
    motor->setpwm(duty[0], duty[1], duty[2]);

    motor->encoder_handle.self_mec_theta += OPENLOOP_DEBUG_STEP_THETA;
    if (motor->encoder_handle.self_mec_theta > _2PI)
    {
        motor->encoder_handle.self_mec_theta = 0.0f;
    }
    if (motor->encoder_handle.self_mec_theta < 0.0f)
    {
        motor->encoder_handle.self_mec_theta = _2PI;
    }
}
/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        pthis 
 * @return       fsm_rt_t 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
fsm_rt_t selfopenloop_mode(fsm_cb_t *pthis)
{
    motor_t *motor;
    motor = (motor_t*)pthis->pdata;    
    switch (pthis->chState) 
    {
    case ENTER:
        USER_DEBUG_NORMAL("entry selfopenloop_mode\n");
        mc_encoder_init(&(motor->encoder_handle));//编码器初始化
        pthis->chState = MOTOR_STATUS_READY;
        motor->curMotorstate = pthis->chState;
    case MOTOR_STATUS_READY:
        if (motor->curCmd != CMD_START)
        {
            break;
        }
        motor->enable();
        motor->encoder_handle.runflag = 1;
        pthis->chState = MOTOR_STATUS_CALIBRATE;
        motor->curMotorstate = pthis->chState;
        break;

    case MOTOR_STATUS_CALIBRATE://自开环 预定位到0度
        if(!mc_sol_preposition(motor))//预定位成功
        {
            USER_DEBUG_NORMAL("prepositin successful\r\n");
            pthis->chState = MOTOR_STATUS_RUN;
            motor->curMotorstate = pthis->chState;            
        }
        break;

    case MOTOR_STATUS_RUN:
        if (motor->curCmd == CMD_STOP)
        {
            pthis->chState = MOTOR_STATUS_STOP;
            motor->curMotorstate = pthis->chState;
        }
        break;
    case MOTOR_STATUS_STOP:
        motor->disable();
        pthis->chState = MOTOR_STATUS_READY;
        motor->curMotorstate = pthis->chState;
        break;
    case EXIT:
        USER_DEBUG_NORMAL("exit selfopenloop_mode\n");
        motor->disable();
        mc_encoder_deinit(&(motor->encoder_handle));
        pthis->chState = ENTER;
        break;
    default:
        break;
    }
    return 0;
}
