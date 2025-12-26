#ifndef OPENLOOP_VOLTAGE_H
#define OPENLOOP_VOLTAGE_H

#include <stdint.h>

struct device;

/* ---------------- 对齐相关 ---------------- */

struct op_align_position {
	float angle;    /* 电角度 */
	float duration; /* 持续时间 */
};

struct op_align_config {
	const struct op_align_position *positions;
	uint8_t num_positions;
	float voltage;
};

/* ---------------- 旋转相关 ---------------- */

struct op_rotate_config {
	float voltage;
	float speed; /* rad/s */
	float duration;
	float start_angle;
};

/* ---------------- openloop 对象 ---------------- */

struct openloop_voltage {
	struct device *inv;

	/* align 状态 */
	struct op_align_config align_cfg;
	uint8_t align_index;
	float elapsed;

	/* rotate 状态 */
	struct op_rotate_config rotate_cfg;
	float elec_angle;
	float total_elec_rad;
};

/* ---------------- API ---------------- */

void openloop_voltage_init(struct openloop_voltage *op, struct device *inv);

/* 对齐（单点 / 多点统一） */
void openloop_voltage_align_start(struct openloop_voltage *op, const struct op_align_config *cfg);
int openloop_voltage_align_update(struct openloop_voltage *op, float dt);

/* 旋转 */

void openloop_voltage_rotate_start(struct openloop_voltage *op, const struct op_rotate_config *cfg);
int openloop_voltage_rotate_update(struct openloop_voltage *op, float dt);

/* 查询 */
float openloop_voltage_get_total_elec_rad(struct openloop_voltage *op);
float openloop_voltage_get_elec_angle(struct openloop_voltage *op);

#endif
