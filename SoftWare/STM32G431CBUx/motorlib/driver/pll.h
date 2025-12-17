#ifndef _PLL_H
#define _PLL_H
#include "stdint.h"
#include "stdbool.h"
#include <string.h>
#include <math.h>
typedef struct {
	// PI控制器参数
	float kp;        // 比例增益
	float ki;        // 积分增益
	float bandwidth; // 带宽（Hz）

	// 状态变量
	float pos_est;    // 连续位置估计（rad，可多圈）
	float vel_est;    // 速度估计（rad/s）
	float integrator; // 积分器状态

	// 上一次的包装角度（用于计算增量）
	float last_phase; // 上一次的测量相位（0-2π）

	// 输出
	float output_wrapped_angle;  // 包装角度（0-2π）
	float output_velocity;       // 速度（rad/s）
	float output_continuous_pos; // 连续位置（rad）

	// 初始化标志
	bool initialized;
} pll_estimator_t;
bool pll_init(pll_estimator_t *pll, float bandwidth_hz, float dt);
bool pll_update(pll_estimator_t *pll, int32_t adjusted_raw, float dt);
#endif
