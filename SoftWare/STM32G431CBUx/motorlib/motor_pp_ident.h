#ifndef MOTOR_PP_IDENT_H
#define MOTOR_PP_IDENT_H

#include <stdbool.h>
#include <stdint.h>
#include "device.h"
enum calibration_state {
	// 对齐阶段 - 将电机定位到已知起始位置
	CALIB_STATE_ALIGN = 0,

	// 正向校准 - 顺时针方向旋转并采集数据
	CALIB_STATE_CW_CALIBRATION,

	// 反向校准 - 逆时针方向旋转并采集数据
	CALIB_STATE_CCW_CALIBRATION,

	// 数据计算 - 处理采集的数据并计算结果
	CALIB_STATE_DATA_PROCESSING,

	// 校准结束 - 完成校准并保存结果
	CALIB_STATE_COMPLETE,

	// 错误状态 - 校准过程中出现错误
	CALIB_STATE_ERROR
};
struct pp_ident_config {
	// 施加的开环电角速度(rad/s) 或者 固定步进值(你可任选)
	float openloop_speed;

	// 开环运行时间(秒)，需保证至少旋转机械角度一圈
	float duration;

	// 编码器最大值，例如 AS5047 = 16384
	uint32_t encoder_max;

	float openloop_voltage;
};

struct pp_ident_data {
	uint32_t raw_start; // 开始时 encoder raw
	uint32_t raw_end;   // 结束时 encoder raw

	int32_t raw_delta; // 机械角度变化（未归一化）
	float mech_rounds; // 机械角度圈数（可能>1或<1）

	float electrical_rounds; // 开环期间走过的电角度圈数（由指令决定）
	uint16_t pole_pairs;     // 识别出的极对数

	bool done; // 是否识别完成

	float time_acc;        // 累计时间
	float elec_angle;      // 我们在内部人工产生的电角度指令
	int32_t raw_prev;      // 上一周期 raw
	int32_t raw_delta_acc; // 累积机械角变化
	enum calibration_state calibra_state;
};
void pp_ident_start(struct device *motor);
void pp_ident_update(struct device *motor, float dt);

#endif
