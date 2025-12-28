#ifndef FOC_PARAMETERS_H
#define FOC_PARAMETERS_H
#include "foc_pid.h"
#include <stdbool.h>

/* =========================================================
 *  Measurement (FOC feedback quantities)
 *  - 来源：电流采样 / 编码器 / 观测器
 *  - 特点：只读（对控制环而言）
 * ========================================================= */
struct foc_meas {
	float id; /* d-axis current */
	float iq; /* q-axis current */
	float pre_mech_angle;
	float velocity; /* mechanical or electrical velocity */
	float position; /* mechanical or electrical position */
};

/* =========================================================
 *  Reference (Internal loop references)
 *  - 来源：控制环内部计算
 *  - 特点：环间中间量，不允许外部直接写
 * ========================================================= */
struct foc_ref {
	float id;       /* current loop reference */
	float iq;       /* current loop reference */
	float velocity; /* velocity loop reference */
};

/* =========================================================
 *  Controller (Regulators and parameters)
 *  - 来源：初始化 / 调试接口
 *  - 特点：低频修改
 * ========================================================= */
struct foc_ctrl {
	struct foc_pid id;
	struct foc_pid iq;
	struct foc_pid velocity;
	struct foc_pid postion;
};

/* =========================================================
 *  FOC Core Data
 * ========================================================= */
struct foc_data {
	struct foc_meas meas;       /* measured quantities */
	struct foc_ref ref;         /* internal references */
	struct foc_ctrl controller; /* controllers */
};

#endif
