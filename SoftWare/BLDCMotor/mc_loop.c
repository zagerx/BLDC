/*==========================================================================================
 * @file mc_loop.c
 * @author zager
 * @version      0.1
 * @date 2025-01-04
 * 
 * @copyright Copyright (c) 2025
 * 
 * @brief 
--------------------------------------------------------------------------------------------*/
#include "mc_loop.h"
#include "math.h"
#include "mc_smo.h"
#include "mc_focmath.h"


/*==========================================================================================
 * @brief        电流环
 * @FuncName     currment_loop
 * @param        obj 
 * @return       duty_t 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void currment_loop(void *obj,float *duty)
{
	motor_t* motor = (motor_t*)obj;
	mc_currment_t *curloop_handle = (mc_currment_t *)(&motor->currment_handle);
	mc_encoder_t *encoder = (mc_encoder_t*)(&motor->encoder_handle);

	float theta,next_theta;
	theta      = encoder->ele_theta;
	next_theta = theta ;//+ 1.57079632f;

	/*Clark&&Park transform*/
	_3s_2s(curloop_handle->i_abc,curloop_handle->i_alpbe);
	_2s_2r(curloop_handle->i_alpbe,theta,curloop_handle->i_dq);

	/*PID Control*/
	float Vd,Vq; 
	#if(MOTOR_WORK_MODE == MOTOR_DEBUG_ENCODERMODE)
		Vd = 0.0f;
		Vq = encoder->self_te;
	#else
		pid_cb_t *d_axis_pid = &(curloop_handle->d_pid);
		pid_cb_t *q_axis_pid = &(curloop_handle->q_pid);
		Vd = pid_contrl(d_axis_pid,curloop_handle->id_tar,curloop_handle->i_dq[0]);
		Vq = pid_contrl(q_axis_pid,curloop_handle->iq_tar,curloop_handle->i_dq[1]);
	#endif // DEBUG 

	/*限制矢量圆*/
	float v_dq[2];
	float v_limt_dq[2];
	v_dq[0] = Vd;
	v_dq[1] = Vq;
	_limit_vector_circle(v_dq,v_limt_dq);

	/*反Park变换*/
	float temp_ab[2] = {0};
	_2r_2s(v_limt_dq,next_theta,temp_ab);

#if 0
	/*Observer*/
	smo_t *smo;
	smo = &(curloop_handle->ti_smo);
	smo->Ialpha = i_alphabeta.alpha;
	smo->Ibeta= i_alphabeta.beta;
	smo->Ealpha = temp_ab.alpha;
	smo->Ebeta= temp_ab.beta;
	smo_observer(smo);
#endif	
	/*Apply SVM*/ 
	SVM(temp_ab[0],temp_ab[1],duty);
}


/*==========================================================================================
 * @brief        速度环
 * @FuncName     speed_loop
 * @param        pthis 
 * @return       float 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
float speed_loop(mc_speed_t *pthis)
{
    return pid_contrl(&(pthis->pid),pthis->tar,pthis->real);
}
