#include "device.h"
#include "inverter.h"

void inverter_start(const struct device *dev) {
  const struct inverter_config *cfg = dev->config;
  if (cfg->tim_pwm_start) {
    cfg->tim_pwm_start();
  }
}

void inverter_stop(const struct device *dev) {
  const struct inverter_config *cfg = dev->config;
  if (cfg->tim_pwm_stop) {
    cfg->tim_pwm_stop();
  }
}

void inverter_set_3phase_voltages(const struct device *dev, float a, float b,
                                 float c) {
  const struct inverter_config *cfg = dev->config;
  if (cfg->tim_pwm_set) {
    cfg->tim_pwm_set(a, b, c);
  }
}

void inverter_set_3phase_disable(const struct device *dev) {
  const struct inverter_config *cfg = dev->config;
  if (cfg->tim_pwm_disable) {
    cfg->tim_pwm_disable();
  }
}

void inverter_set_3phase_enable(const struct device *dev) {
  const struct inverter_config *cfg = dev->config;
  if (cfg->tim_pwm_enable) {
    cfg->tim_pwm_enable();
  }
}