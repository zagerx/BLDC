#include "foc_parameters.h"
#include "coord_transform.h"
#undef M_PI
#define M_PI 3.14159265358979323846f
#undef RAD_TO_DEG
#define RAD_TO_DEG (180.0f / M_PI)

void update_focparam_idq(struct foc_parameters *foc_param, float i_alpha, float i_beta,
			 float eangle)
{
	float sin_val, cos_val;
	float i_d, i_q;
	sin_cos_f32(eangle, &sin_val, &cos_val);
	park_f32(i_alpha, i_beta, &i_d, &i_q, sin_val, cos_val);
	foc_param->id = i_d;
	foc_param->iq = i_q;
}
void read_focparam_idq(struct foc_parameters *foc_param, float *id, float *iq)
{
	*id = foc_param->id;
	*iq = foc_param->iq;
}

void write_foc_param_(struct foc_parameters *foc_param, enum foc_parameters_index flag, float *data)
{
	if (flag == INDEX_ID_REF) // 目标参数更新
	{
		foc_param->id_ref = data[0];
	} else if (flag == INDEX_D_PI) {
		float kp, ki;
		kp = data[0];
		ki = data[1];
		foc_param->id_pi_control.kp = kp;
		foc_param->id_pi_control.ki = ki;
	} else if (flag == INDEX_Q_PI) {
		float kp, ki;
		kp = data[0];
		ki = data[1];
		foc_param->iq_pi_control.kp = kp;
		foc_param->iq_pi_control.ki = ki;
	} else if (flag == INDEX_VELOCITY_PI) { // PI参数更新
		float kp, ki;
		kp = data[0];
		ki = data[1];
		foc_param->velocity_pi_control.kp = kp;
		foc_param->velocity_pi_control.ki = ki;
	} else if (flag == INDEX_VELOCITY_REG) {
		foc_param->velocity_tar = data[0];
	}
}
void read_foc_param_(struct foc_parameters *foc_param, enum foc_parameters_index flag, float *data)
{
	if (flag == INDEX_ID_REF) // 目标参数更新
	{
		data[0] = foc_param->id_ref;
	} else if (flag == INDEX_Q_PI) {
		data[0] = foc_param->id_ref;
	} else if (flag == INDEX_VELOCITY_REG) {
		data[0] = foc_param->velocity_ref;
	} else if (flag == INDEX_D_PI) {
		float kp, ki;
		kp = foc_param->id_pi_control.kp;
		ki = foc_param->id_pi_control.ki;
		data[0] = kp;
		data[1] = ki;
	} else if (flag == INDEX_Q_PI) {
		float kp, ki;
		kp = foc_param->iq_pi_control.kp;
		ki = foc_param->iq_pi_control.ki;
		data[0] = kp;
		data[1] = ki;
	} else if (flag == INDEX_VELOCITY_PI) { // PI参数更新
		float kp, ki;
		kp = foc_param->velocity_pi_control.kp;
		ki = foc_param->velocity_pi_control.ki;
		data[0] = kp;
		data[1] = ki;
	}
}