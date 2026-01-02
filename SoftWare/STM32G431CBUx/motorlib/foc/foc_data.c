#include "foc_data.h"
#include "coord_transform.h"
#undef M_PI
#define M_PI 3.14159265358979323846f
#undef RAD_TO_DEG
#define RAD_TO_DEG (180.0f / M_PI)
#include <stdbool.h>
void foc_data_write_meas(struct foc_data *f_data, float *iabc, float vbus, float eangle,
			 float velocity, float odom)
{
	f_data->meas.ia = iabc[0];
	f_data->meas.ib = iabc[1];
	f_data->meas.ic = iabc[2];
	f_data->meas.vbus = vbus;
	f_data->meas.eangle = eangle;
	f_data->meas.velocity = velocity;
	f_data->meas.position = odom;
}
void foc_data_cacle_meas_idq(struct foc_data *f_data)
{
	float i_alpha, i_beta;
	float i_d, i_q;
	float sin_val, cos_val;
	float ia, ib;
	ia = f_data->meas.ia;
	ib = f_data->meas.ib;
	float eangle = f_data->meas.eangle;
	// float ic = f_data->meas.ic;
	/* Clarke: ABC -> αβ */
	clarke_f32(ia, ib, &i_alpha, &i_beta);

	/* Park: αβ -> dq */
	sin_cos_f32(eangle * RAD_TO_DEG, &sin_val, &cos_val);
	park_f32(i_alpha, i_beta, &i_d, &i_q, sin_val, cos_val);

	/* Update FOC runtime data */
	f_data->meas.id = i_d;
	f_data->meas.iq = i_q;
}
