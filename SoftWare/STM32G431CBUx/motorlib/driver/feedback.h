#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "device.h"
#include "stdint.h"
#include "stdbool.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

/*
 * 若要启用 PLL 估计并把 PLL 字段包含进 data 结构，请在编译时定义：
 * -DFEEDBACK_USE_PLL
 *
 * 否则默认使用差分法估计速度，且 PLL 字段与代码将被剔除。
 */

#ifdef FEEDBACK_USE_VEL_FILTER
#include "lfp.h"
#endif
struct feedback_config {
	uint32_t (*get_raw)(void);

	uint32_t cpr;       // 编码器每转计数
	uint8_t pole_pairs; // 电机极对数
	int8_t direction;   // 编码器方向: 1 或 -1
	uint32_t zero_offset;
	float pos_estimate_weight; // 位置估计权重

#ifdef FEEDBACK_USE_PLL
	float pll_kp; // PLL比例增益
	float pll_ki; // PLL积分增益
#endif
};

struct feedback_data {
	/* 原始数据 */
	uint32_t raw;
	float accumulated_dt;
	/* 角度信息 */
	float mech_angle;
	float mech_angle_prev; // 用于差分法计算速度
	float elec_angle;

	/* 速度信息 */
	float vel_estimate; // 机械速度估计 (rad/s)
	float vel_elec;     // 电速度 (rad/s)

#ifdef FEEDBACK_USE_PLL
	/* PLL 状态：仅在编译期启用时存在 */
	float pll_pos; // PLL位置
	float pll_vel; // PLL速度
		       // float pll_kp_scale; // 已删除 (冗余)
#endif

	/* 其他状态 */
	int32_t turns;
	float pos_estimate;

#ifdef FEEDBACK_USE_VEL_FILTER
	struct first_order_lpf vel_filter; // 速度滤波器实例
#endif
};

struct feedback_api {
	float (*calc_elec_angle)(struct device *);
	float (*calc_velocity)(struct device *);
	void (*reset)(struct device *);
	void (*set_offset)(struct device *, uint16_t);
};

int feedback_init(struct device *dev);
void update_feedback(struct device *dev, float dt);
void update_feedback_offset(struct device *dev, uint16_t offset);
void update_feedback_pole_pairs(struct device *dev, uint8_t pole_pairs);
void update_feedback_direction(struct device *dev, int8_t direction);
void update_feedback_cpr(struct device *feedback, uint32_t cpr);
float read_feedback_velocity(struct device *dev);
uint32_t read_feedback_raw(struct device *feedback);
float read_feedback_elec_angle(struct device *dev);

#endif
