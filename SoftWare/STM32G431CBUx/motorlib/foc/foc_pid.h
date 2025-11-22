#ifndef __FOC_PID_H
#define __FOC_PID_H

#include <stdint.h>

struct foc_pid {
	float kp; // 比例增益
	float ki; // 积分增益 (注意：这里的单位通常是 1/s * kp 或者是纯积分增益，建议用纯积分增益)
	float err_prev; // 上一次误差 (如果需要D项，电流环通常不需要D)
	float integral; // 积分累加器
	float limit;    // 输出限幅 (电压限制)
};

// 初始化
void foc_pid_init(struct foc_pid *pid, float kp, float ki, float limit);

// 复位
void foc_pid_reset(struct foc_pid *pid);

// 核心计算
float foc_pid_run(struct foc_pid *pid, float target, float meas, float dt);

#endif
