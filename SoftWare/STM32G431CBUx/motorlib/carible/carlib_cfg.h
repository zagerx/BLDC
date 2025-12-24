#ifndef CARLIB_CFG_H
#define CARLIB_CFG_H
#include "openloop_voltage.h"
struct carlib_config {
	/* ---------- 设备句柄 ---------- */
	struct device *inverter;
	struct device *currents;
	struct device *feedback;
	openloop_voltage_t *op;
	/* ---------- 结果输出 ---------- */
	struct motor_parameters *params;
};
#endif
