#ifndef _FOC_H
#define _FOC_H

#include "inverter.h"
#include "svpwm.h"
#include "coord_transform.h"
#include "foc_data.h"

void _open_loop_voltage_vector_drive(struct inverter_t *inverter, float driver_angle,
				     float driver_vol);
void foc_update_current_idq(struct foc_data *f_data, const float i_abc[3], float elec_angle);
void foc_apply_voltage_dq(struct inverter_t *inverter, float ud, float uq, float elec_angle);

#endif
