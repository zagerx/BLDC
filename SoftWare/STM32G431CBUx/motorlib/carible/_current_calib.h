#ifndef _CURRENT_CALIB_H
#define _CURRENT_CALIB_H

#include "carlib_cfg.h"
#include "device.h"
#include <stdint.h>
#include <stdbool.h>
#include "openloop_voltage.h"
enum curr_calib_state {
	CURR_CALIB_STATE_IDLE = 1,
	CURR_CALIB_STATE_ALIGN,
	CURR_CALIB_STATE_SAMPLING,
	CURR_CALIB_STATE_PROCESSING,
	CURR_CALIB_STATE_ERROR,
	CURR_CALIB_STATE_DONE,
};

struct carlib_current {
	struct carlib_config *cfg;

	enum curr_calib_state state;

	uint32_t a_channle_sum;
	uint32_t b_channle_sum;
	uint32_t c_channle_sum;

	uint16_t sample_conut;
	uint16_t sample_target; // 例如 1024 / 2048

	float align_time; // 对齐等待时间
	float align_timer;
};
void carlib_current_init(struct carlib_current *carlib, struct carlib_config *cfg);
int32_t curr_calib_update(struct carlib_current *carlib, float dt);
#endif
