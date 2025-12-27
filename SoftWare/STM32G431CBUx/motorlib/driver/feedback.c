#include "feedback.h"
#include "device.h"
#include <math.h>
#include <stdint.h>

#define K_LINE 1.0f // 3.81971863420f
// 角度归一化到 [0, 2PI]
static float normalize_angle(float angle)
{
	angle = fmodf(angle, 2.0f * M_PI);
	if (angle < 0) {
		angle += 2.0f * M_PI;
	}
	return angle;
}

// 初始化反馈模块
int feedback_init(struct feedback_t *feedback)
{
	struct feedback_config *cfg = feedback->config;
	struct feedback_data *data = feedback->data;

	if (cfg->params->cpr == 0 || cfg->params->direction == 0 || cfg->params->pole_pairs == 0) {
		return -1;
	}
	data->accumulated_dt = 0.0f;
	return 0;
}

// 角度/速度更新
void update_feedback(struct feedback_t *feedback, float dt)
{
	struct feedback_config *cfg = feedback->config;
	struct feedback_data *data = feedback->data;

	const float two_pi = 2.0f * M_PI;
	const float cpr_f = (float)cfg->params->cpr;
	const int32_t cpr_i = (int32_t)cfg->params->cpr;
	const float pole_pairs_f = (float)cfg->params->pole_pairs;

	uint32_t current_raw = cfg->get_raw();
	data->raw = current_raw;

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

	// === 正确的速度计算方法 ===
	data->accumulated_dt += dt;

	// 使用累积的总计数变化计算角度差（更准确）
	static int32_t total_counts_prev = 0;
	int32_t counts_diff = data->total_counts - total_counts_prev;

	if (data->accumulated_dt > 0.0002f) // 或 0.002f
	{
		// 基于计数差计算角度变化
		float angle_diff = (two_pi / cpr_f) * counts_diff * (float)cfg->params->direction;

		float raw_vel = angle_diff / data->accumulated_dt;
		// 重置累积时间和计数
		data->accumulated_dt = 0.0f;
		total_counts_prev = data->total_counts;

		// 滤波
		float filter_alpha = 1.0f;
		data->mech_vel =
			(1.0f - filter_alpha) * data->mech_vel + filter_alpha * raw_vel * K_LINE;
	}

	// 更新位置
	data->odom = current_mech_angle * K_LINE;
}
/* 其余函数保持不变 */
float read_feedback_elec_angle(struct feedback_t *feedback)
{
	struct feedback_data *data = feedback->data;
	return data->elec_angle;
}

float read_feedback_velocity(struct feedback_t *feedback)
{
	struct feedback_data *data = feedback->data;
	return data->mech_vel;
}
float read_feedback_odome(struct feedback_t *feedback)
{
	struct feedback_data *data = feedback->data;
	return data->odom;
}
void clear_feedback_odome(struct feedback_t *feedback)
{
	struct feedback_data *data = feedback->data;
	data->odom = 0.0f;
}
void feedback_reset(struct feedback_t *feedback)
{
	struct feedback_data *data = feedback->data;
	data->mech_vel = 0;
	data->odom = 0;
}

uint32_t read_feedback_raw(struct feedback_t *feedback)
{
	struct feedback_config *fb_cfg = feedback->config;
	return fb_cfg->get_raw();
}

uint16_t read_feedback_pair(struct feedback_t *feedback)
{
	struct feedback_config *cfg = feedback->config;
	return cfg->params->pole_pairs;
}