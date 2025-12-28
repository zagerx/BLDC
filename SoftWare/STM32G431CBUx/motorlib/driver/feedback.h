#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "device.h"
#include "stdint.h"
#include "stdbool.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif
#define LUT_SIZE 128
struct feedback_paramters {
	uint32_t cpr;       // 编码器每转计数
	uint8_t pole_pairs; // 电机极对数
	int8_t direction;   // 编码器方向: 1 或 -1
	uint32_t offset;
};

struct feedback_config {
	uint32_t (*get_raw)(void);
	float radius;
	struct feedback_paramters *params;
};

struct feedback_data {
	/* 原始数据 */
	uint32_t raw;
	float accumulated_dt;
	int32_t prev_counts;
	int64_t total_counts;
	/* 角度信息 */
	float elec_angle;
	float mech_angle;
	/* 速度信息 */
	float mech_vel; // 机械速度rad/s

	/* 其他状态 */
	float odom;
};

struct feedback_api {
	float (*calc_elec_angle)(struct device *);
	float (*calc_velocity)(struct device *);
	void (*reset)(struct device *);
	void (*set_offset)(struct device *, uint16_t);
};

struct feedback_t {
	struct feedback_config *config;
	struct feedback_data *data;
};

int feedback_init(struct feedback_t *feedback);
void update_feedback(struct feedback_t *feedback, float dt);

uint32_t read_feedback_cpr(struct feedback_t *feedback);
uint16_t read_feedback_pair(struct feedback_t *feedback);
float read_feedback_mech_angle(struct feedback_t *feedback);
float read_feedback_velocity(struct feedback_t *feedback);
float read_feedback_odome(struct feedback_t *feedback);
void clear_feedback_odome(struct feedback_t *feedback);
uint32_t read_feedback_raw(struct feedback_t *feedback);
float read_feedback_elec_angle(struct feedback_t *feedback);
#endif
