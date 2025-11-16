#ifndef MOTOR_PP_IDENT_H
#define MOTOR_PP_IDENT_H

#include <stdbool.h>
#include <stdint.h>
struct pp_ident_config {
	// 施加的开环电角速度(rad/s) 或者 固定步进值(你可任选)
	float openloop_speed;

	// 开环运行时间(秒)，需保证至少旋转机械角度一圈
	float duration;

	// 编码器最大值，例如 AS5047 = 16384
	uint16_t encoder_max;
};

struct pp_ident_data {
	uint16_t raw_start; // 开始时 encoder raw
	uint16_t raw_end;   // 结束时 encoder raw

	int32_t raw_delta; // 机械角度变化（未归一化）
	float mech_rounds; // 机械角度圈数（可能>1或<1）

	float electrical_rounds; // 开环期间走过的电角度圈数（由指令决定）
	uint16_t pole_pairs;     // 识别出的极对数

	bool done; // 是否识别完成

	float time_acc;   // 累计时间
	float elec_angle; // 我们在内部人工产生的电角度指令
	bool running;
};

#endif
