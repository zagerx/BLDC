#include "foc.h"
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
