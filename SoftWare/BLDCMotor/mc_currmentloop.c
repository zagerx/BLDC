#include "mc_currmentloop.h"
#include "math.h"
#include "mc_smo.h"
#define SQRT_3__2    0.86602540378f
duty_t currment_loop(void *obj)
{
	motor_t* motor = (motor_t*)obj;
	mc_currment_t *curloop_handle = (mc_currment_t *)(&motor->currment_handle);
	mc_encoder_t *encoder=(mc_encoder_t*)(&motor->encoder_handle);
	float theta,next_theta,id_targe,iq_targe;
    abc_t i_abc = {0};
    alpbet_t i_alphabeta;
	dq_t i_dq = {0};
	pid_cb_t *d_axis_pid;
	pid_cb_t *q_axis_pid;

	i_abc.a    = curloop_handle->i_abc[0];
	i_abc.b    = curloop_handle->i_abc[1];
	i_abc.c    = curloop_handle->i_abc[2];
	theta      = encoder->ele_theta;
	next_theta = theta+1.57f;
	d_axis_pid = &(curloop_handle->d_pid);
	q_axis_pid = &(curloop_handle->q_pid);
	id_targe = curloop_handle->id_tar;
	iq_targe = curloop_handle->iq_tar;

	/*Clark&&Park transform*/
	_3s_2s(i_abc,&i_alphabeta);
	_2s_2r(i_alphabeta,theta,&i_dq);
	motor->debug.id_real = i_dq.d;
	motor->debug.iq_real = i_dq.q;
	/*PID Control*/
	float Vd,Vq; 
	#if(MOTOR_WORK_MODE == MOTOR_DEBUG_ENCODERMODE)
		Vd = 0.0f;
		Vq = encoder->self_te;
	#else
		// Vd = id_targe;
		// Vq = iq_targe;	
		Vd = pid_contrl(d_axis_pid,id_targe,i_dq.d);
		Vq = pid_contrl(q_axis_pid,iq_targe,i_dq.q);
	#endif // DEBUG 

	/*Limiting Vector Circle*/
	#if 0
		float mod_to_V = (2.0f / 3.0f) * 24.0f;
		float V_to_mod = 1.0f / mod_to_V;
		float mod_d = V_to_mod * Vd;
		float mod_q = V_to_mod * Vq;
		float mod_scalefactor = 0.80f * SQRT_3__2 * 1.0f / sqrtf(mod_d * mod_d + mod_q * mod_q);
		if (mod_scalefactor < 1.0f)
		{
			mod_d *= mod_scalefactor;
			mod_q *= mod_scalefactor;
		}
	#else
		float k;
		k = 0.8f*0.5773502f*sqrtf(24.0f/(Vd*Vd+Vq*Vq));
		if (k < 1.0f)
		{
			Vd *= k;
			Vq *= k;
		}
	#endif

	/*Inverse Park Transform*/
	alpbet_t temp_ab = {0};
	dq_t v_dq;
	v_dq.d = Vd;
	v_dq.q = Vq;
	temp_ab = _2r_2s(v_dq,next_theta);

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
	duty_t duty = {0};
	duty = SVM(temp_ab.alpha,temp_ab.beta);
	return duty;
}

void currment_loop_paraminit(pid_cb_t *pid)
{
	/*pid 参数*/
}

void currment_loop_paramdeinit(pid_cb_t *pid)
{
	/*pid 反向初始化参数*/
}
