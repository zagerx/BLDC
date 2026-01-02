#ifndef _FOC_H
#define _FOC_H

#include "inverter.h"
#include "svpwm.h"
#include "coord_transform.h"
#include "foc_data.h"
#include "feedback.h"

void _open_loop_voltage_vector_drive(struct inverter_t *inverter, float driver_angle,
				     float driver_vol);
void foc_limit_voltage_vector(float vbus, float *ud, float *uq);

void foc_modulate_dq(float eangle, float vbus, float ud, float uq, float *ualpha, float *ubeta);
void foc_currentloop_init(struct foc_data *f_data);
void foc_currentloop_deinit(struct foc_data *f_data);
void foc_currentloop(struct foc_data *f_data, float *ud, float *uq, float dt);

void foc_velocityloop_init(struct foc_data *f_data);
void foc_velocityloop_deinit(struct foc_data *f_data);
void foc_velocity_loop(struct foc_data *f_data, float tar, float dt);

void foc_posloop_init(struct foc_data *f_data);
void foc_posloop(struct foc_data *f_data, float tar, float dt);
void foc_posloop_deinit(struct foc_data *f_data);

#endif
