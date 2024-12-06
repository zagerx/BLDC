#ifndef __TRAJECTORY_PLANNING__H
#define __TRAJECTORY_PLANNING__H

#include "stdint.h"
typedef struct _s_in s_in_t;
struct _s_in
{
    uint16_t cout; // 每个阶段的step
    uint16_t tau;
    uint16_t Tm;
    uint16_t Ts[8]; // Ts[1]代表第一阶段
    int16_t status; // 轨迹执行状态
    int16_t state;  // 轨迹规划状态

    float Ja;   // 加加速度
    float V[8]; // V[1]代表第一阶段结束速度
    float V0;   // 每次执行时的初始速度
    float cur_output;
    float last_targe;
    float max_acc;
};
float s_type_interpolation(void *object, float new_targe);

typedef struct _linear_in linear_in_t;
struct _linear_in
{
    float cur_output;
    float last_targe;
    float max_acc;
    float acc;
    int16_t status;
};
float linear_interpolation(void *linear, float new_targe);
void linear_interpolation_init(void *Object, float max_acc);
void linear_interpolation_deinit(void *Object);

#endif