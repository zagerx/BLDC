#include "foc_data.h"
#include "coord_transform.h"
#undef M_PI
#define M_PI 3.14159265358979323846f
#undef RAD_TO_DEG
#define RAD_TO_DEG (180.0f / M_PI)

void update_focparam_idq(struct foc_data *f_data, float i_alpha, float i_beta, float eangle)
{
	float sin_val, cos_val;
	float i_d, i_q;
	sin_cos_f32(eangle, &sin_val, &cos_val);
	park_f32(i_alpha, i_beta, &i_d, &i_q, sin_val, cos_val);
	f_data->id = i_d;
	f_data->iq = i_q;
}
void read_focparam_idq(struct foc_data *f_data, float *id, float *iq)
{
	*id = f_data->id;
	*iq = f_data->iq;
}

void write_foc_param_(struct foc_data *f_data, enum foc_data_index flag, float *data)
{
	if (flag == INDEX_ID_REF) // 目标参数更新
	{
		f_data->id_ref = data[0];
	} else if (flag == INDEX_D_PI) {
		float kp, ki;
		kp = data[0];
		ki = data[1];
		f_data->id_pi_control.params->kp = kp;
		f_data->id_pi_control.params->ki = ki;
	} else if (flag == INDEX_Q_PI) {
		float kp, ki;
		kp = data[0];
		ki = data[1];
		f_data->iq_pi_control.params->kp = kp;
		f_data->iq_pi_control.params->ki = ki;
	} else if (flag == INDEX_VELOCITY_PI) { // PI参数更新
		float kp, ki;
		kp = data[0];
		ki = data[1];
		f_data->velocity_pi_control.params->kp = kp;
		f_data->velocity_pi_control.params->ki = ki;
	} else if (flag == INDEX_VELOCITY_REG) {
		f_data->velocity_tar = data[0];
	}
}
void read_foc_data(struct foc_data *f_data, enum foc_data_index flag, float *data)
{
	if (flag == INDEX_ID_REF) // 目标参数更新
	{
		data[0] = f_data->id_ref;
	} else if (flag == INDEX_Q_PI) {
		data[0] = f_data->id_ref;
	} else if (flag == INDEX_VELOCITY_REG) {
		data[0] = f_data->velocity_ref;
	} else if (flag == INDEX_D_PI) {
		float kp, ki;
		kp = f_data->id_pi_control.params->kp;
		ki = f_data->id_pi_control.params->ki;
		data[0] = kp;
		data[1] = ki;
	} else if (flag == INDEX_Q_PI) {
		float kp, ki;
		kp = f_data->iq_pi_control.params->kp;
		ki = f_data->iq_pi_control.params->ki;
		data[0] = kp;
		data[1] = ki;
	} else if (flag == INDEX_VELOCITY_PI) { // PI参数更新
		float kp, ki;
		kp = f_data->velocity_pi_control.params->kp;
		ki = f_data->velocity_pi_control.params->ki;
		data[0] = kp;
		data[1] = ki;
	}
}