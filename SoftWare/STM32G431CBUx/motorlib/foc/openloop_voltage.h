#ifndef OPENLOOP_VOLTAGE_H
#define OPENLOOP_VOLTAGE_H
#include <stdint.h>

struct device;
struct op_align_config {
	float voltage;     // 电压
	float align_tim;   // 对齐时间
	float align_angle; // 对齐角度
};

struct op_rotate_config {
	float voltage;     // 旋转电压
	float speed;       // 角速度
	float duration;    // 旋转时间
	float start_angle; // 起始角度
};

struct openloop_voltage {
	struct device *inv;
	struct op_rotate_config rotate_cfg;
	struct op_align_config align_cfg;

	float elapsed; // 计时器

	float elec_angle;
	float total_elec_rad;
};

void openloop_voltage_init(struct openloop_voltage *op, struct device *inv);

void openloop_voltage_align_start(struct openloop_voltage *op, const struct op_align_config *cfg);
int openloop_voltage_align_update(struct openloop_voltage *op, float dt);

void openloop_voltage_roate_start(struct openloop_voltage *op, const struct op_rotate_config *cfg);
int openloop_voltage_rotate_update(struct openloop_voltage *op, float dt);
float openloop_voltage_get_total_elec_rad(struct openloop_voltage *op);
#endif
