#ifndef _ENCODER_CALIB_H
#define _ENCODER_CALIB_H

#include <stdint.h>
#include <stdbool.h>
#include "device.h"

enum encoder_calib_state {
	ENC_CALIB_STATE_IDLE = 0,
	ENC_CALIB_STATE_ALIGN,
	ENC_CALIB_STATE_ROTATE_TEST,
	ENC_CALIB_STATE_PROCESSING,
	ENC_CALIB_STATE_COMPLETE,
	ENC_CALIB_STATE_ERROR,
};

struct encoder_calib_config {

	struct device *feedback;
	struct device *inverter;
	/* ALIGN 时用的电压幅值（0~0.577） */
	float align_voltage;

	/* 开环旋转时的电压幅值（0~0.577） */
	float openloop_voltage;

	/* 开环旋转时的电角速度（rad/s），正为一个方向，负为相反 */
	float openloop_speed;

	/* 旋转判断的最大编码器计数（如 AS5047=16384） */
	uint32_t encoder_max;

	/* 旋转持续时间（s），本实现使用采样计数方式，保留字段以便扩展 */
	float duration;
};

struct encoder_calib_data {
	uint32_t raw_start;
	uint32_t raw_end;

	int32_t raw_delta;     // 本次最后一次差值
	int32_t raw_prev;      // 上一周期 raw，用于 unwrap
	int32_t raw_delta_acc; // 累积差值，用于计算机械圈数

	float mech_rounds;
	float electrical_rounds;

	uint32_t sample_count;
	uint32_t align_count;

	float elec_angle;
	float time_acc;

	int16_t polarity;     // +1 or -1
	uint32_t zero_offset; // 保存的编码器零偏（raw counts 对应电角度 0）

	bool done;
	enum encoder_calib_state state;
};

void encoder_calib_start(struct device *encoder_calib);
void encoder_calib_update(struct device *encoder_calib, float dt);
enum encoder_calib_state encoder_calib_get_state(struct device *encoder_calib);

#endif
