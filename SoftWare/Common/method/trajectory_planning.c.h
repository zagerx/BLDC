#ifndef __TRAJECTORY_PLANNING__H
#define __TRAJECTORY_PLANNING__H

#include "stdint.h"
float S_constant_accel_decel(float current_target);



typedef struct _linear_in linear_in_t;
 struct _linear_in
{
    float cur_output;
    float last_targe;
    float max_acc;
    float max_dece;
    float acc;
    int16_t status;
};
float linear_interpolation(void *linear,float new_targe);
void linear_interpolation_init(void *Object,float max_acc,float max_dece);
void linear_interpolation_deinit(void *Object);

#endif