#ifndef __TRAJECTORY_PLANNING__H
#define __TRAJECTORY_PLANNING__H

#include "stdint.h"
typedef struct _s_in s_in_t;
struct _s_in
{
    float Ja;//加加速度
    uint16_t Ts[8];//Ts[1]代表第一阶段
    float V[8];//V[1]代表第一阶段结束速度
    float V0;//每次规划时的初始速度
    uint16_t cout;//每个阶段的step
    uint16_t tau;
    float cur_output;
    float last_targe;
    float max_acc;
    float max_dece;
    float acc;
    int16_t status;
};
float s_interpolation(void *object,float new_targe);



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