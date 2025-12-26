#include "openloop_voltage.h"
#include "device.h"
#include "inverter.h"
#include <string.h>
#include <math.h>

#ifndef M_TWOPI
#define M_TWOPI (2.0f * 3.14159265358979323846f)
#endif

extern void foc_apply_voltage_dq(struct device *inv, float ud, float uq, float elec_angle);

/* --------------------------------------------------------- */

void openloop_voltage_init(struct openloop_voltage *op, struct device *inv)
{
	if (!op || !inv) {
		return;
	}
	memset(op, 0, sizeof(*op));
	op->inv = inv;
}

/* ---------------------------------------------------------
 * 对齐（单点 / 多点）
 * --------------------------------------------------------- */

void openloop_voltage_align_start(struct openloop_voltage *op, const struct op_align_config *cfg)
{
	if (!op || !cfg || !cfg->positions || cfg->num_positions == 0) {
		return;
	}

	op->align_cfg = *cfg;
	op->align_index = 0;
	op->elapsed = 0.0f;
}

int openloop_voltage_align_update(struct openloop_voltage *op, float dt)
{
	const struct op_align_position *pos;

	if (!op || !op->inv) {
		return -1;
	}

	if (op->align_index >= op->align_cfg.num_positions) {
		return 0;
	}

	pos = &op->align_cfg.positions[op->align_index];

	op->elapsed += dt;
	if (op->elapsed >= pos->duration) {
		op->elapsed = 0.0f;
		op->align_index++;

		if (op->align_index >= op->align_cfg.num_positions) {
			inverter_set_3phase_voltages(op->inv, 0.0f, 0.0f, 0.0f);
			return 0;
		}

		pos = &op->align_cfg.positions[op->align_index];
	}

	foc_apply_voltage_dq(op->inv, 0.0f, op->align_cfg.voltage, pos->angle);
	return 1;
}

/* ---------------------------------------------------------
 * 旋转
 * --------------------------------------------------------- */

void openloop_voltage_rotate_start(struct openloop_voltage *op, const struct op_rotate_config *cfg)
{
	if (!op || !cfg || !op->inv) {
		return;
	}

	op->rotate_cfg = *cfg;
	op->elapsed = 0.0f;
	op->elec_angle = cfg->start_angle;
	op->total_elec_rad = cfg->start_angle;
}

int openloop_voltage_rotate_update(struct openloop_voltage *op, float dt)
{
	float delta;

	if (!op || !op->inv) {
		return -1;
	}

	op->elapsed += dt;
	if (op->elapsed >= op->rotate_cfg.duration) {
		inverter_set_3phase_voltages(op->inv, 0.0f, 0.0f, 0.0f);
		return 0;
	}

	delta = op->rotate_cfg.speed * dt;
	op->elec_angle += delta;
	op->total_elec_rad += delta;

	if (op->elec_angle >= M_TWOPI) {
		op->elec_angle -= M_TWOPI;
	} else if (op->elec_angle < 0.0f) {
		op->elec_angle += M_TWOPI;
	}

	foc_apply_voltage_dq(op->inv, 0.0f, op->rotate_cfg.voltage, op->elec_angle);
	return 1;
}

/* --------------------------------------------------------- */

float openloop_voltage_get_total_elec_rad(struct openloop_voltage *op)
{
	if (!op) {
		return 0.0f;
	}
	return op->total_elec_rad;
}
float openloop_voltage_get_elec_angle(struct openloop_voltage *op)
{
	if (!op) {
		return 0.0f;
	}
	return op->elec_angle;
}
