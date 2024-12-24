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

typedef struct _linear_in
{
	/* data */
	int32_t acc_max;
	int32_t acc;
	int16_t out_ref;          //执行输出值
	int16_t last_targe;
	uint16_t actor_count;     //执行器的计时器
	uint16_t actor_Ts;        //执行器所需时间
	uint16_t resp_max_cycle;  //响应最大周期
	int16_t actor_state;      //执行状态
	int16_t s_state;          //规划状态
    int16_t test_out;
}linear_in_t;

int16_t t_type_interpolation(linear_in_t *linear,int16_t target);
void t_type_interpolation_init(linear_in_t *linear,uint16_t max_acc,uint16_t respone);

#endif