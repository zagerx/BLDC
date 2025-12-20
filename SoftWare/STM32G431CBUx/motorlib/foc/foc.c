#include "foc.h"
#undef M_PI
#define M_PI 3.14159265358979323846f
#undef RAD_TO_DEG
#define RAD_TO_DEG (180.0f / M_PI)
void _open_loop_voltage_vector_drive(struct device *inverer, float driver_angle, float driver_vol)
{
	float sin_val, cos_val;
	sin_cos_f32(driver_angle, &sin_val, &cos_val);
	float ualpha, ubeta;
	inv_park_f32(driver_vol, 0.0f, &ualpha, &ubeta, sin_val, cos_val);
	float duty[3];
	svm_set(ualpha, ubeta, duty);
	inverter_set_3phase_voltages(inverer, duty[0], duty[1], duty[2]);
}

void foc_apply_voltage_dq(struct device *inverter, float ud, float uq, float elec_angle)
{
	float sin_val, cos_val;
	float ualpha, ubeta;
	float duty[3];

	/* dq -> αβ */
	sin_cos_f32(elec_angle * RAD_TO_DEG, &sin_val, &cos_val);
	inv_park_f32(ud, uq, &ualpha, &ubeta, sin_val, cos_val);

	/* αβ -> PWM */
	svm_set(ualpha, ubeta, duty);

	/* Apply to inverter */
	inverter_set_3phase_voltages(inverter, duty[0], duty[1], duty[2]);
}

void foc_update_current_idq(struct foc_data *f_data, const float i_abc[3], float elec_angle)
{
	float i_alpha, i_beta;
	float i_d, i_q;
	float sin_val, cos_val;

	/* Clarke: ABC -> αβ */
	clarke_f32(i_abc[0], i_abc[1], &i_alpha, &i_beta);

	/* Park: αβ -> dq */
	sin_cos_f32(elec_angle * RAD_TO_DEG, &sin_val, &cos_val);
	park_f32(i_alpha, i_beta, &i_d, &i_q, sin_val, cos_val);

	/* Update FOC runtime data */
	f_data->id = i_d;
	f_data->iq = i_q;
}