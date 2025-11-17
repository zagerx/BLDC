#ifndef MOTOR_CURRENT_CALIB_H
#define MOTOR_CURRENT_CALIB_H
#include "device.h"
#include <stdint.h>
#include <stdbool.h>

enum curr_calib_state {
	CURR_CALIB_STATE_IDLE = 0,
	CURR_CALIB_STATE_ALIGN,
	CURR_CALIB_STATE_SAMPLING,
	CURR_CALIB_STATE_PROCESSING,
	CURR_CALIB_STATE_DONE,
	CURR_CALIB_STATE_ERROR,
};

struct curr_calib_config {
	uint16_t sample_count; // 采样次数
};

struct curr_calib_data {
	enum curr_calib_state state;

	uint32_t sample_index;

	float offset_a_acc;
	float offset_b_acc;
	float offset_c_acc;

	float offset_a;
	float offset_b;
	float offset_c;

	bool done;
};

void curr_calib_start(struct device *motor, uint16_t ample_count);
void curr_calib_update(struct device *motor, uint32_t *adc_raw);

#endif
