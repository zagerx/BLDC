#ifndef _ENCODER_CALIB_H
#define _ENCODER_CALIB_H

#include <stdint.h>
#include <stdbool.h>
#include "carlib_cfg.h"

enum encoder_calib_state {
	ENC_CALIB_INIT = 0,
	ENC_CALIB_ALIGN,
	ENC_CALIB_DIR_CHECK,
	ENC_CALIB_ROTATE,
	ENC_CALIB_PROCESS,
	ENC_CALIB_OFFSET_INIT,
	ENC_CALIB_OFFSET_RUNING,
	ENC_CALIB_DONE,
	ENC_CALIB_ERROR,
};

struct carlib_encoder {
	struct carlib_config *cfg;

	enum encoder_calib_state state;

	/* ---------- 运行变量 ---------- */
	float elapsed;
	float elec_angle_start;
	float elec_angle_end;
	float mech_angle_start;
	float mech_angle_end;

	int32_t error;

	// /* ---------- 错误追踪 ---------- */
	uint32_t error_code;
	float total_elec_rad; // 累计走过的总电角度

	// 编码器相关
	uint32_t raw_prev;     // 上一次的编码器原始值
	int32_t raw_delta_acc; // 累计机械角度变化量
};

void carlib_encoder_init(struct carlib_encoder *ec, struct carlib_config *cfg);
int32_t encoder_calib_update(struct carlib_encoder *ec, float dt);

/* 错误代码定义 */
#define ENC_ERR_NONE            0
#define ENC_ERR_PARAM           (1 << 0)
#define ENC_ERR_ANGLE_TOO_SMALL (1 << 1)
#define ENC_ERR_CALC_FAILED     (1 << 2)
#define ENC_ERR_OVERFLOW        (1 << 3)

#endif
