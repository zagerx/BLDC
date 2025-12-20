#ifndef _CURRENT_CALIB_H
#define _CURRENT_CALIB_H

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
	float align_duration;  // 对齐时间 (s)

	struct device *inverter;
	struct device *currsmp;
};

struct curr_calib_data {
	enum curr_calib_state state;

	float tim_acc;         // 时间累加器 (保持 float 以匹配 dt)
	uint32_t sample_index; // 采样计数

	// 优化：使用 uint32_t 进行整数累加
	// 前提：Raw值 * sample_count 不超过 uint32 最大值 (约42亿)
	uint32_t offset_a_acc;
	uint32_t offset_b_acc;
	uint32_t offset_c_acc;
};

void curr_calib_start(struct device *curr_calib, uint16_t sample_count);
int32_t curr_calib_update(struct device *curr_calib, float dt);

#endif
