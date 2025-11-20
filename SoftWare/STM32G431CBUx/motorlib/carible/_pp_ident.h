#ifndef _PP_IDENT_H
#define _PP_IDENT_H

#include <stdbool.h>
#include <stdint.h>
#include "device.h"

/* ---------------------------------------------------------
 * 状态定义
 * --------------------------------------------------------- */
enum pp_calib_state {
	PP_CALIB_STATE_IDLE = 0,
	PP_CALIB_STATE_ALIGN,           // 对齐阶段
	PP_CALIB_STATE_ROTATING,        // 旋转阶段
	PP_CALIB_STATE_DATA_PROCESSING, // 计算阶段
	PP_CALIB_STATE_COMPLETE,        // 完成
	PP_CALIB_STATE_ERROR            // 错误
};

/* ---------------------------------------------------------
 * 配置参数
 * --------------------------------------------------------- */
struct pp_ident_config {
	struct device *feedback;
	struct device *inverter;

	float openloop_speed;   // 开环电角速度 (rad/s)
	float openloop_voltage; // 注入电压幅值 (V)

	float align_duration;  // 对齐阶段持续时间 (s)
	float rotate_duration; // 旋转阶段持续时间 (s)

	uint32_t encoder_max; // 编码器最大计数值 (CPR)
};

/* ---------------------------------------------------------
 * 运行时数据
 * --------------------------------------------------------- */
struct pp_ident_data {
	enum pp_calib_state state;

	uint16_t pole_pairs; // 结果：识别出的极对数

	// 运行时计数器
	float time_acc; // 通用时间累加器

	// 角度相关
	float drive_angle;    // 用于 SVM 输出的当前电角度 (0 ~ 2PI)
	float total_elec_rad; // 累计走过的总电角度

	// 编码器相关
	int32_t raw_prev;      // 上一次的编码器原始值
	int32_t raw_delta_acc; // 累计机械角度变化量
};

/* ---------------------------------------------------------
 * API 声明
 * --------------------------------------------------------- */
void pp_ident_start(struct device *pp);

/**
 * @brief 更新极对数辨识状态机
 * @param pp 设备指针
 * @param dt 当前控制周期的 delta time (单位: 秒)
 * @return 0: 进行中, 1: 完成, -1: 错误
 */
int32_t pp_ident_update(struct device *pp, float dt);

#endif