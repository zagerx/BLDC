#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "device.h"
#include "stdint.h"
#include <math.h>
#include "stdbool.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define AS5047_CPR 16384 // AS5047是14位编码器，16384 CPR

struct feedback_config {
	uint16_t (*get_raw)(void);
	uint16_t cpr;              // 编码器每转计数
	uint8_t pole_pairs;        // 电机极对数
	int8_t direction;          // 编码器方向: 1 或 -1
	float pll_kp;              // PLL比例增益
	float pll_ki;              // PLL积分增益
	float pos_estimate_weight; // 位置估计权重
	bool index_search_enabled; // 是否启用索引搜索
	uint16_t offset;           // 编码器偏移量
};

struct feedback_data {
	// 原始数据
	uint16_t raw;
	uint16_t raw_prev;
	uint32_t zero_offset;

	// 角度信息
	float raw_angle;       // 原始机械角度 (rad)
	float raw_angle_prev;  // 上一次原始机械角度
	float mech_angle;      // 机械角度 (考虑方向) (rad)
	float mech_angle_prev; // 上一次机械角度
	float elec_angle;      // 电角度 (rad)
	float elec_angle_prev; // 上一次电角度
	float phase;           // 相位 (0-1)

	// 速度信息
	float vel_estimate; // 机械速度估计 (rad/s)
	float vel_elec;     // 电速度 (rad/s)

	// PLL状态
	float pll_pos;      // PLL位置
	float pll_vel;      // PLL速度
	float pll_kp_scale; // PLL比例缩放

	// 其他状态
	int32_t turns;           // 圈数计数
	bool index_found;        // 是否找到索引位
	float pos_estimate;      // 位置估计
	uint32_t timestamp_prev; // 上一次时间戳
};

struct feedback_api {
	float (*calc_elec_angle)(struct device *);
	float (*calc_velocity)(struct device *);
	void (*reset)(struct device *);
	void (*set_offset)(struct device *, uint16_t);
};
void feedback_init(struct device *dev);
void feedback_update_angle_vel(struct device *dev, float dt);
void feedback_set_offset(struct device *dev, uint16_t offset);
void feedback_set_pole_pairs(struct device *dev, uint8_t pole_pairs);
void feedback_set_direction(struct device *dev, int8_t direction);
#endif
