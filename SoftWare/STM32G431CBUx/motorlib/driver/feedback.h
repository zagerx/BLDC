#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "device.h"
#include "stdint.h"
#include "stdbool.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

struct feedback_config {
	uint32_t (*get_raw)(void);

	uint32_t cpr;       // 编码器每转计数
	uint8_t pole_pairs; // 电机极对数
	int8_t direction;   // 编码器方向: 1 或 -1
	uint32_t offset;
};

struct feedback_data {
	/* 原始数据 */
	uint32_t raw;
	float accumulated_dt;
	int32_t prev_counts;
	int32_t total_counts;
	int32_t total_counts_prev;
	/* 角度信息 */
	float elec_angle;

	/* 速度信息 */
	float mech_vel;
	float odom;
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
float read_feedback_odome(struct device *dev);
void update_feedback_odome(struct device *dev);
uint32_t read_feedback_raw(struct device *feedback);
float read_feedback_elec_angle(struct device *dev);
#endif
