#ifndef _ENCODER_CALIB_H
#define _ENCODER_CALIB_H

#include <stdint.h>
#include <stdbool.h>
#include "carlib_cfg.h"

enum encoder_calib_state {
	ENC_CALIB_IDLE = 0,
	ENC_CALIB_ALIGN,
	ENC_CALIB_DIR_CHECK,
	ENC_CALIB_ROTATE,
	ENC_CALIB_PROCESS,
	ENC_CALIB_OFFSET,
	ENC_CALIB_DONE,
	ENC_CALIB_ERROR,
};

struct carlib_encoder {
	struct carlib_config *cfg;

	enum encoder_calib_state state;

	/* ---------- 配置参数 ---------- */
	float align_voltage; /* D轴对齐电压 */
	float align_time;    /* 对齐保持时间 */
	float rotate_speed;  /* 电角速度 rad/s */
	float rotate_time;   /* 转动时间 */

	/* ---------- 运行变量 ---------- */
	float elapsed;
	float elec_angle_start;
	float elec_angle_end;
	float mech_angle_start;
	float mech_angle_end;

	int encoder_dir;
	int pole_pairs;
	float encoder_offset;

	int32_t error;
};

void carlib_encoder_init(struct carlib_encoder *ec);
int32_t encoder_calib_update(struct carlib_encoder *ec, float dt);

#endif
