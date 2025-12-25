/* openloop_voltage.c */

#include "openloop_voltage.h"
#include <math.h>
#include "device.h"
#include "inverter.h"

#undef M_PI
#define M_PI 3.14159265358979323846f
/* 外部已有接口 */
extern void foc_apply_voltage_dq(struct device *inverter, float ud, float uq, float elec_angle);

/* ------------------------------------------------------------ */
/* 内部工具函数 */
/* ------------------------------------------------------------ */
static float wrap_angle(float angle)
{
	const float two_pi = 2.0f * (float)M_PI;

	while (angle >= two_pi) {
		angle -= two_pi;
	}
	while (angle < 0.0f) {
		angle += two_pi;
	}
	return angle;
}

/* ------------------------------------------------------------ */
/* 接口实现 */
/* ------------------------------------------------------------ */
void openloop_voltage_init(openloop_voltage_t *ol, struct device *inverter)
{
	if (!ol) {
		return;
	}
	ol->inverter = inverter;

	ol->voltage = 0.0f;
	ol->elec_speed = 0.0f;
	ol->elec_angle = 0.0f;
	ol->mode = OPENLOOP_MODE_HOLD_ANGLE;
	ol->enabled = false;
}

void openloop_voltage_enable(openloop_voltage_t *ol)
{
	if (!ol) {
		return;
	}
	ol->enabled = true;
}

void openloop_voltage_disable(openloop_voltage_t *ol)
{
	if (!ol) {
		return;
	}
	ol->enabled = false;

	/* 安全起见：关断输出 */
	if (ol->inverter) {
		inverter_set_3phase_voltages(ol->inverter, 0.0f, 0.0f, 0.0f);
	}
}

void openloop_voltage_set_mode(openloop_voltage_t *ol, openloop_mode_t mode)
{
	if (!ol) {
		return;
	}
	ol->mode = mode;
}

void openloop_voltage_set_voltage(openloop_voltage_t *ol, float voltage)
{
	if (!ol) {
		return;
	}
	ol->voltage = voltage;
}

void openloop_voltage_set_speed(openloop_voltage_t *ol, float elec_speed)
{
	if (!ol) {
		return;
	}
	ol->elec_speed = elec_speed;
}

void openloop_voltage_set_angle(openloop_voltage_t *ol, float elec_angle)
{
	if (!ol) {
		return;
	}
	ol->elec_angle = wrap_angle(elec_angle);
}

float openloop_voltage_get_angle(const openloop_voltage_t *ol)
{
	if (!ol) {
		return 0.0f;
	}
	return ol->elec_angle;
}

void openloop_voltage_update(openloop_voltage_t *ol, float dt)
{
	if (!ol || !ol->enabled || !ol->inverter) {
		return;
	}

	/* -------- 角度生成 -------- */
	switch (ol->mode) {
	case OPENLOOP_MODE_HOLD_ANGLE:
		/* 角度保持不变 */
		break;

	case OPENLOOP_MODE_CONST_SPEED:
		ol->elec_angle += ol->elec_speed * dt;
		ol->elec_angle = wrap_angle(ol->elec_angle);
		break;

	default:
		break;
	}

	/* -------- 电压输出 -------- */
	/* 开环：ud = 0，仅施加 uq */
	foc_apply_voltage_dq(ol->inverter, 0.0f, ol->voltage, ol->elec_angle);
}
