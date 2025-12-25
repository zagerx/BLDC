#pragma once

#include <stdint.h>

struct device;
typedef struct {
	float voltage;     // 电压
	float align_tim;   // 对齐时间
	float align_angle; // 对齐角度
} op_align_config_t;

typedef struct {
	float voltage;     // 旋转电压
	float speed;       // 角速度
	float duration;    // 旋转时间
	float start_angle; // 起始角度

} op_rotate_config_t;

typedef struct {
	struct device *inv;
	op_rotate_config_t rotate_cfg;
	op_align_config_t align_cfg;

	float elapsed; // 计时器

	float elec_angle;
	float total_elec_rad;
} openloop_voltage_t;

void openloop_voltage_init(openloop_voltage_t *op, struct device *inv);

void openloop_voltage_align_start(openloop_voltage_t *op, const op_align_config_t *cfg);
int openloop_voltage_align_update(openloop_voltage_t *op, float dt);

void openloop_voltage_roate_start(openloop_voltage_t *op, const op_rotate_config_t *cfg);
int openloop_voltage_rotate_update(openloop_voltage_t *op, float dt);
float openloop_voltage_get_total_elec_rad(openloop_voltage_t *op);
