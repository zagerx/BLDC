#ifndef CARLIB_CFG_H
#define CARLIB_CFG_H
#include "openloop_voltage.h"
struct carlib_config {
	/* ---------- 设备句柄 ---------- */
	struct inverter_t *inverter;
	struct currsmp_t *currsmp;
	struct feedback_t *feedback;
	struct openloop_voltage *op;
	/* ---------- 结果输出 ---------- */
	struct motor_parameters *params;
};
#endif
