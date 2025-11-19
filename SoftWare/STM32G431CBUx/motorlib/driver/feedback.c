#include "feedback.h"

#include "device.h"
#include <stdint.h>

void feedback_update_angle_vec(struct device *dev,float dt)
{
	struct feedback_config *cfg = dev->config;

	uint16_t raw = cfg->get_raw();

	// calce

	struct feedback_data *data = dev->data;
	data->raw = raw;
	data->e_angle = raw / 1.0f;
	return;
}
