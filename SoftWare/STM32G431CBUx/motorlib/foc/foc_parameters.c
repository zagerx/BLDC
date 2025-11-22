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
	sin_cos_f32(eangle * RAD_TO_DEG, &sin_val, &cos_val);
	park_f32(i_alpha, i_beta, &i_d, &i_q, sin_val, cos_val);
	foc_param->id = i_d;
	foc_param->iq = i_q;
}
void read_focparam_idq(struct foc_parameters *foc_param, float *id, float *iq)
{
	*id = foc_param->id;
	*iq = foc_param->iq;
}

void debug_update_iq_ref(struct foc_parameters *foc_param, float iq_ref)
{
	foc_param->id_ref = 0.0f;
	foc_param->id_ref = iq_ref;
}
