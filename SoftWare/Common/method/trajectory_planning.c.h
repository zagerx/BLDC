#ifndef __TRAJECTORY_PLANNING__H
#define __TRAJECTORY_PLANNING__H

#include "stdint.h"
float S_constant_accel_decel(float current_target);



typedef struct _linear_in linear_in_t;
 struct _linear_in
{
    float delt_y;
    float count;
    float cur_targe;
    float last_targe;
    int16_t status;

    float (*cacle)(void*,float);
    void (*init)(void*);
    void (*deinit)(void*);
};
float linear_interpolation(void *linear,float new_targe);
void linear_interpolation_init(void *Object);
void linear_interpolation_deinit(void *Object);

#endif