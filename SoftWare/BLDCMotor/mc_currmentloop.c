#include "mc_currmentloop.h"

#include "stdbool.h"
#include "mc_utils.h"
#include "tim.h"


duty_t currment_loop(mc_currment_t *curloop_handle)
{
	float theta, next_theta;
    abc_t i_abc = {0};
    alpbet_t i_alphabeta;
	dq_t i_dq = {0};

	pid_cb_t *d_axis_pid;
	pid_cb_t *q_axis_pid;

	i_abc.a    = curloop_handle->i_abc[0];
	i_abc.b    = curloop_handle->i_abc[1];
	i_abc.c    = curloop_handle->i_abc[2];
	theta      = curloop_handle->theta;
	next_theta = curloop_handle->next_theta;
	d_axis_pid = &(curloop_handle->d_pid);
	q_axis_pid = &(curloop_handle->q_pid);

	_3s_2s(i_abc,&i_alphabeta);
	_2s_2r(i_alphabeta,theta,&i_dq);

	motordebug.ia_real = i_abc.a;
	motordebug.ib_real = i_abc.b;
	motordebug.ic_real = i_abc.c;
	float Id_des = motordebug.id_targe;
	float Iq_des = motordebug.iq_targe;

	motordebug.id_real  = i_dq.d;
	motordebug.iq_real  = i_dq.q;
	float Vd = pid_contrl(d_axis_pid,Id_des,i_dq.d);
	float Vq = pid_contrl(q_axis_pid,Iq_des,i_dq.q);
	float mod_to_V = (2.0f / 3.0f) * 24.0f;
	float V_to_mod = 1.0f / mod_to_V;
	float mod_d = V_to_mod * Vd;
	float mod_q = V_to_mod * Vq;
	float mod_scalefactor = 0.80f * sqrt3_by_2 * 1.0f / sqrtf(mod_d * mod_d + mod_q * mod_q);
	if (mod_scalefactor < 1.0f)
	{
		mod_d *= mod_scalefactor;
		mod_q *= mod_scalefactor;
	}

	// Inverse park transform
	i_dq.d = mod_d;
	i_dq.q = mod_q;
	alpbet_t temp_ab = {0};
	temp_ab = _2r_2s(i_dq,next_theta);

	// Apply SVM
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
