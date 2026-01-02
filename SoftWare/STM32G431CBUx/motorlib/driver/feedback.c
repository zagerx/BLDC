#include "feedback.h"
#include "device.h"
#include <math.h>
#include <stdint.h>

// 角度归一化到 [0, 2PI]
static float normalize_angle(float angle)
{
	angle = fmodf(angle, 2.0f * M_PI);
	if (angle < 0) {
		angle += 2.0f * M_PI;
	}
	return angle;
}
static float feedback_calc_mOmega(struct feedback_t *feedback, float dt);

// 初始化反馈模块
int feedback_init(struct feedback_t *feedback)
{
	struct feedback_config *cfg = feedback->config;

	if (cfg->params->cpr == 0 || cfg->params->direction == 0 || cfg->params->pole_pairs == 0) {
		return -1;
	}
	struct feedback_data *data = feedback->data;
	data->tim_acc = 0.0f;
	return 0;
}
void update_feedback_raw(struct feedback_t *feedback)
{
	struct feedback_config *cfg = feedback->config;
	struct feedback_data *data = feedback->data;
	data->raw = cfg->get_raw();
}
// 角度/速度更新
void update_feedback(struct feedback_t *feedback, float dt)
{
	(void)dt;
	struct feedback_config *cfg = feedback->config;
	struct feedback_data *data = feedback->data;
	// if (cfg->params->cpr == 0 || cfg->params->direction == 0 || cfg->params->pole_pairs == 0)
	// { 	return;
	// }
	const float two_pi = 2.0f * M_PI;
	const float cpr_f = (float)cfg->params->cpr;
	const int32_t cpr_i = (int32_t)cfg->params->cpr;
	const float pole_pairs_f = (float)cfg->params->pole_pairs;
	uint32_t current_raw = data->raw;
	// data->raw = current_raw;

	int32_t adjusted_raw = (int32_t)current_raw - (int32_t)cfg->params->offset;

	int32_t delta_counts = adjusted_raw - data->prev_counts;

	// 处理计数溢出（越过CPR边界）
	if (delta_counts > cpr_i / 2) {
		delta_counts -= cpr_i;
	} else if (delta_counts < -cpr_i / 2) {
		delta_counts += cpr_i;
	}

	// 更新总计数
	data->total_counts += delta_counts;
	data->prev_counts = adjusted_raw;

	// 计算机械角度
	float current_mech_angle =
		(two_pi / cpr_f) * data->total_counts * (float)cfg->params->direction;

	// 计算电角度（包装的）
	data->elec_angle = normalize_angle(current_mech_angle * pole_pairs_f);

	data->mech_angle_acc = current_mech_angle;

	if (data->tim_acc >= SPEED_LOOP_CYCLE) {
		data->tim_acc = 0.0f;
		feedback_calc_mOmega(feedback, SPEED_LOOP_CYCLE);
	} else {
		data->tim_acc += dt;
	}
}
float read_feedback_mech_angle(struct feedback_t *feedback)
{
	struct feedback_data *data = feedback->data;
	return data->mech_angle_acc;
}

/* 其余函数保持不变 */
float read_feedback_elec_angle(struct feedback_t *feedback)
{
	struct feedback_data *data = feedback->data;
	return data->elec_angle;
}
static float feedback_calc_mOmega(struct feedback_t *feedback, float dt)
{
	struct feedback_data *data = feedback->data;
	float cur_mech_angle = data->mech_angle_acc;

	float dtheta = cur_mech_angle - data->mech_angle_acc_prev;
	if (dtheta > M_PI) {
		dtheta -= 2.0f * M_PI;
	} else if (dtheta < -M_PI) {
		dtheta += 2.0f * M_PI;
	}

	float speed_raw = dtheta / dt;

	/* 一阶低通滤波（基于时间常数） */
	float tau = 0.02f; // 20ms
	float alpha = dt / (tau + dt);

	data->mech_omega = (1.0f - alpha) * data->mech_omega + alpha * speed_raw;

	data->mech_angle_acc_prev = cur_mech_angle;
	return data->mech_omega;
}
float read_feedback_velocity(struct feedback_t *feedback)
{
	struct feedback_config *cfg = feedback->config;
	struct feedback_data *data = feedback->data;
	float radius = cfg->radius;
	data->liner_vel = data->mech_omega * radius;
	return data->liner_vel;
}

float read_feedback_odome(struct feedback_t *feedback)
{
	struct feedback_data *data = feedback->data;
	struct feedback_config *cfg = feedback->config;
	// 更新里程计
	data->odom = data->mech_angle_acc * cfg->radius;
	return data->odom;
}

void feedback_reset(struct feedback_t *feedback)
{
	struct feedback_data *data = feedback->data;
	data->mech_omega = 0;
	data->odom = 0;
}

uint32_t read_feedback_raw(struct feedback_t *feedback)
{
	struct feedback_data *data = feedback->data;
	return data->raw;
}
