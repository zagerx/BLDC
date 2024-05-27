#include "mc_currmentmode.h"

#include "stdbool.h"
#include "mc_utils.h"
#include "tim.h"



#define  TIM_PERIOD                       3500

// extern mc_param_t mc_param ;
extern mc_param_t mc_param;
float  vbus_voltage=24.0f;

duty_t currment_loop(float *abc,float theta,float next_theta)
{
    abc_t i_abc = {0};
	i_abc.a = abc[0];
	i_abc.b = abc[1];
	i_abc.c = abc[2];
	motordebug.ia = i_abc.a;
	motordebug.ib = i_abc.b;
	motordebug.ic = i_abc.c;

    alpbet_t motor_Ialphabeta;
	_3s_2s(i_abc,&motor_Ialphabeta);
	dq_t idq = {0};
	_2s_2r(motor_Ialphabeta,theta,&idq);

	float Iq_des,Id_des;
	Id_des = motordebug.id_targe;
	Iq_des = motordebug.iq_targe;
	float Ierr_d = Id_des - idq.d;
	float Ierr_q = Iq_des - idq.q;

	motordebug.id_real  = idq.d;
	motordebug.iq_real  = idq.q;
#if 1
	float Vd = pid_contrl(&(mc_param.daxis_pi),Id_des,idq.d);
	float Vq = pid_contrl(&(mc_param.qaxis_pi),Iq_des,idq.q);
#else
	float Vd = 0.0f;
	float Vq = 0.8f;
#endif
	float mod_to_V = (2.0f / 3.0f) * vbus_voltage;
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
	idq.d = mod_d;
	idq.q = mod_q;
	alpbet_t temp_ab = {0};
	temp_ab = _2r_2s(idq,next_theta);

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
