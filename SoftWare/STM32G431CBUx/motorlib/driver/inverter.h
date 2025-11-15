#ifndef INVERTER_H
#define INVERTER_H
#include "stdint.h"

struct inverter_api {};
struct inverter_config {
  void (*tim_init)(void);
  void (*tim_pwm_start)(void);
  void (*tim_pwm_stop)(void);
  void (*tim_pwm_set)(float, float, float);
  void (*tim_pwm_disable)(void);
  void (*tim_pwm_enable)(void);
};
void inverter_start(const struct device *dev);
void inverter_stop(const struct device *dev);
void inverter_set_3phase_voltages(const struct device *dev, float a, float b,
                                  float c);
void inverter_set_3phase_disable(const struct device *dev);
void inverter_set_3phase_enable(const struct device *dev);
#endif
