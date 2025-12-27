#include "device.h"
#include "inverter.h"

void inverter_set_3phase_voltages(const struct inverter_t *inverter, float a, float b, float c)
{
	const struct inverter_config *cfg = inverter->config;
	if (cfg->tim_pwm_set) {
		cfg->tim_pwm_set(a, b, c);
	}
}

void inverter_set_3phase_disable(const struct inverter_t *inverter)
{
	const struct inverter_config *cfg = inverter->config;
	if (cfg->tim_pwm_disable) {
		cfg->tim_pwm_disable();
	}
}

void inverter_set_3phase_enable(const struct inverter_t *inverter)
{
	const struct inverter_config *cfg = inverter->config;
	if (cfg->tim_pwm_enable) {
		cfg->tim_pwm_enable();
	}
}
