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

	float align_voltage;
	float align_duration; // 【新增/替换】：使用时间代替计数

	float openloop_voltage;
	float openloop_speed; // 开环电角速度（rad/s）

	uint32_t encoder_max;    // 旋转判断的最大编码器计数
	float min_rotation_frac; // 最小机械圈数(如0.005f) 判定旋转有效
	float rotate_duration;   // 【新增/替换】：使用时间代替计数

	// float duration; // 【已移除/冗余，由 rotate_duration 代替】
};

struct encoder_calib_data {
	enum encoder_calib_state state;

	float time_acc;   // 通用时间累加器（取代 align_count/sample_count）
	float elec_angle; // 用于驱动的电角度 (0 ~ 2PI)

	int32_t raw_prev;      // 上一周期 raw，用于 unwrap
	int32_t raw_delta_acc; // 累计差值，用于计算机械圈数

	// int16_t polarity;     // 结果: +1 or -1
	// uint32_t zero_offset; // 结果: 编码器零偏（raw counts 对应电角度 0）
};

void encoder_calib_start(struct device *encoder_calib);

/**
 * @brief 更新编码器校准状态机
 * @param encoder_calib 设备指针
 * @param dt 当前控制周期的 delta time (单位: 秒)
 * @return 0: 进行中, 1: 完成, -1: 错误
 */
int32_t encoder_calib_update(struct device *encoder_calib, float dt);

#endif