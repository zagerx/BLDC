#include "openloop_voltage.h"
#include "device.h"
#include <string.h>
#include <math.h>
#include "inverter.h"
#ifndef M_TWOPI
#define M_TWOPI (2.0f * 3.14159265358979323846f)
#endif

extern void foc_apply_voltage_dq(struct device *inverter, float ud, float uq, float elec_angle);

void openloop_voltage_init(struct openloop_voltage *op, struct device *inv)
{
	if (!op || !inv) {
		return;
	}

	memset(op, 0, sizeof(*op));
	op->inv = inv;
}
void openloop_voltage_align_start(struct openloop_voltage *op, const struct op_align_config *cfg)
{
	if (!op || !cfg || !op->inv) {
		return;
	}
	op->align_cfg.voltage = cfg->voltage;
	op->align_cfg.align_tim = cfg->align_tim;
	op->align_cfg.align_angle = cfg->align_angle;
	op->elapsed = 0.0f;

	return;
}
int openloop_voltage_align_update(struct openloop_voltage *op, float dt)
{
	if (!op || !op->inv) {
		return -1;
	}
	struct op_align_config *align_cfg = &op->align_cfg;

	op->elapsed += dt;
	if (op->elapsed > align_cfg->align_tim) {
		inverter_set_3phase_voltages(op->inv, 0.0f, 0.0f, 0.0f);
		return 0;
	}
	foc_apply_voltage_dq(op->inv, 0.0f, align_cfg->voltage, align_cfg->align_angle);
	return 1;
}
void openloop_voltage_roate_start(struct openloop_voltage *op, const struct op_rotate_config *cfg)
{
	if (!op || !cfg || !op->inv) {
		return;
	}

	op->rotate_cfg.duration = cfg->duration;
	op->rotate_cfg.speed = cfg->speed;
	op->rotate_cfg.voltage = cfg->voltage;
	op->elapsed = 0.0f;

	/* 初始化角度 */
	op->elec_angle = cfg->start_angle;
	op->total_elec_rad = 0.0f;
}

int openloop_voltage_rotate_update(struct openloop_voltage *op, float dt)
{
	if (!op || !op->inv) {
		return -1;
	}

	struct op_rotate_config *cfg = &op->rotate_cfg;
	float voltage = cfg->voltage;
	float duration = cfg->duration;
	float speed = cfg->speed;

	op->elapsed += dt;
	if (op->elapsed > duration) {
		inverter_set_3phase_voltages(op->inv, 0.0f, 0.0f, 0.0f);
		return 0;
	}

	float delta = speed * dt;
	op->elec_angle += delta;
	op->total_elec_rad += delta;

	if (op->elec_angle >= M_TWOPI) {
		op->elec_angle -= M_TWOPI;
	} else if (op->elec_angle < 0.0f) {
		op->elec_angle += M_TWOPI;
	}
	foc_apply_voltage_dq(op->inv, 0.0f, voltage, op->elec_angle);
	return 1;
}

float openloop_voltage_get_total_elec_rad(struct openloop_voltage *op)
{
	if (!op) {
		return 0.0f;
	}

	return op->total_elec_rad;
}
