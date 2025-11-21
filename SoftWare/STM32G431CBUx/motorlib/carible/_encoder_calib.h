#ifndef _ENCODER_CALIB_H
#define _ENCODER_CALIB_H

#include <stdint.h>
#include <stdbool.h>
#include "device.h"

enum encoder_calib_state {
	ENC_CALIB_STATE_IDLE = 0,
	ENC_CALIB_STATE_ALIGN_START, // 初始定位到扫描起点
	ENC_CALIB_STATE_SCAN_FWD,    // 正向扫描 (过零点记录)
	ENC_CALIB_STATE_SCAN_BWD,    // 反向扫描 (过零点记录)
	ENC_CALIB_STATE_CALCULATE,   // 计算平均值
	ENC_CALIB_STATE_COMPLETE,
	ENC_CALIB_STATE_ERROR,
};

struct encoder_calib_config {
	struct device *feedback;
	struct device *inverter;

	float voltage; // 扫描电压 (ODrive: calib_current * phase_res)
	float speed;   // 扫描速度 (rad/s)

	uint32_t encoder_max; // CPR (Counts Per Revolution)
};

struct encoder_calib_data {
	enum encoder_calib_state state;

	float driver_elec_angle; // 当前输出的电角度  自给的
	float time_acc;
	// 扫描相关
	uint32_t raw_fwd;  // 正向扫描时的零点捕获值
	uint32_t raw_bwd;  // 反向扫描时的零点捕获值
	bool fwd_captured; // 标志位
	bool bwd_captured;
};

void encoder_calib_start(struct device *encoder_calib);
int32_t encoder_calib_update(struct device *encoder_calib, float dt);

#endif