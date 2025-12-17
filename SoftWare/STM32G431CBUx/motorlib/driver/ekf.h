#ifndef _EKF_ESTIMATOR_H
#define _EKF_ESTIMATOR_H

#include "stdint.h"
#include "stdbool.h"
#include "math.h"

// 定义圆周率
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

typedef struct {
	// ====== 用户可调参数 ======
	float bandwidth_hz; // 期望带宽 (Hz)，用于推导噪声参数
	float dt;           // 固定采样周期 (秒)

	// 过程噪声协方差 (Q) 相关参数
	float q_pos; // 位置过程噪声强度
	float q_vel; // 速度过程噪声强度

	// 测量噪声协方差 (R) 相关参数
	float r_pos; // 位置测量噪声协方差 (rad^2)

	// ====== EKF 内部状态 ======
	float pos_est; // 连续位置状态估计 (rad)
	float vel_est; // 速度状态估计 (rad/s)

	float p00, p01, p10, p11; // 状态协方差矩阵 P 的四个元素
				  // P = [[p00, p01],
				  //      [p10, p11]]

	// ====== 与PLL结构一致的输出 ======
	float output_wrapped_angle;  // 包装角度 (0-2π)
	float output_velocity;       // 速度 (rad/s)
	float output_continuous_pos; // 连续位置 (rad)

	// ====== 内部工作变量 ======
	float last_raw_phase;   // 上一次的原始相位 (0-2π)
	int32_t last_raw_count; // 上一次的 raw count（用于检测过零）
	int32_t total_revs;     // 总圈数计数
	bool initialized;       // 初始化标志

} ekf_estimator_t;

// ====== 接口函数（严格匹配要求）=====
bool ekf_init(ekf_estimator_t *ekf, float bandwidth_hz, float dt);
bool ekf_update(ekf_estimator_t *ekf, int32_t adjusted_raw, float dt);

#endif