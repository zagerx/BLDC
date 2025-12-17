#ifndef _FOC_H
#define _FOC_H

#include "inverter.h"
#include "svpwm.h"
#include "coord_transform.h"

void _open_loop_voltage_vector_drive(struct device *inverer, float driver_angle, float driver_vol);

#endif
