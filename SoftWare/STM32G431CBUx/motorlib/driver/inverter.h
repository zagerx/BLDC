#ifndef INVERTER_H
#define INVERTER_H
#include "stdint.h"
#include "device.h"

struct inverter_config {
	void (*tim_pwm_set)(float, float, float);
	void (*tim_pwm_disable)(void);
	void (*tim_pwm_enable)(void);
};
struct inverter_t {
	struct inverter_config *config;
};
void inverter_start(const struct inverter_t *inverter);
void inverter_stop(const struct inverter_t *inverter);
void inverter_set_3phase_voltages(const struct inverter_t *inverter, float a, float b, float c);
void inverter_set_3phase_disable(const struct inverter_t *inverter);
void inverter_set_3phase_enable(const struct inverter_t *inverter);
#endif
