#include "feedback.h"
#include "device.h"
#include <stdint.h>
#include <math.h>
// #define K_LINE 1.0F

#define K_LINE 3.81971863420f
// 角度归一化到 [0, 2PI]
static float normalize_angle(float angle)
{
	angle = fmodf(angle, 2.0f * M_PI);
	if (angle < 0)
	{
		angle += 2.0f * M_PI;
	}
	return angle;
}

// 处理角度环绕，返回连续角度差
static float angle_difference(float current, float previous)
{
	float diff = current - previous;
	if (diff > M_PI)
	{
		diff -= 2.0f * M_PI;
	}
	else if (diff < -M_PI)
	{
		diff += 2.0f * M_PI;
	}
	return diff;
}

// 初始化反馈模块
int feedback_init(struct device *dev)
{
	struct feedback_config *cfg = dev->config;
	struct feedback_data *data = dev->data;

	if (cfg->cpr == 0 || cfg->direction == 0 || cfg->pole_pairs == 0)
	{
		return -1;
	}
	cfg->pos_estimate_weight = 0.1f;
	data->accumulated_dt = 0.0f;
#ifdef FEEDBACK_USE_PLL
	/* PLL 参数默认 */
	cfg->pll_kp = 100.0f; // 默认PLL参数
	cfg->pll_ki = 1000.0f;
	// data->pll_kp_scale 已删除 (冗余)

	/* 初始化 PLL 状态 */
	data->pll_pos = 0.0f;
	data->pll_vel = 0.0f;
#endif

	return 0;
}

// 角度/速度更新
void update_feedback(struct device *dev, float dt)
{
	struct feedback_config *cfg = dev->config;
	struct feedback_data *data = dev->data;

	const float two_pi = 2.0f * M_PI;
	const float cpr_f = (float)cfg->cpr;
	const int32_t cpr_i = (int32_t)cfg->cpr;
	const float pole_pairs_f = (float)cfg->pole_pairs;

	// 保存上一次的原始计数
	static int32_t prev_counts = 0;
	// 获取原始编码器值
	uint32_t current_raw = cfg->get_raw();
	data->raw = current_raw;

	// 考虑编码器偏移量
	int32_t adjusted_raw = (int32_t)current_raw - (int32_t)cfg->offset;

	// 处理计数溢出（使用有符号32位）
	// 假设编码器不会在相邻两次采样间转动超过2^31个计数
	static int32_t total_counts = 0;
	int32_t delta_counts = adjusted_raw - prev_counts;

	// 处理计数溢出（越过CPR边界）
	if (delta_counts > cpr_i / 2)
	{
		delta_counts -= cpr_i;
	}
	else if (delta_counts < -cpr_i / 2)
	{
		delta_counts += cpr_i;
	}

	// 更新总计数
	total_counts += delta_counts;
	prev_counts = adjusted_raw;

	// 计算机械角度（连续，不包装）
	float current_mech_angle = (two_pi / cpr_f) * total_counts * (float)cfg->direction;

	// 保存当前角度
	// float mech_angle_prev = data->mech_angle;
	data->mech_angle = current_mech_angle * K_LINE;
	// data->mech_angle_prev = mech_angle_prev;

	// 计算电角度（包装的）
	data->elec_angle = normalize_angle(current_mech_angle * pole_pairs_f);

	// === 正确的速度计算方法 ===
	data->accumulated_dt += dt;

	// 使用累积的总计数变化计算角度差（更准确）
	static int32_t total_counts_prev = 0;
	int32_t counts_diff = total_counts - total_counts_prev;

	if (data->accumulated_dt > 0.0002f) // 或 0.002f
	{
		// 基于计数差计算角度变化
		float angle_diff = (two_pi / cpr_f * K_LINE) * counts_diff * (float)cfg->direction;

		float raw_vel = angle_diff / data->accumulated_dt;
		data->vel_mech1 = (data->mech_angle - data->mech_angle_prev) / data->accumulated_dt;
		data->mech_angle_prev = data->mech_angle;
		// 重置累积时间和计数
		data->accumulated_dt = 0.0f;
		total_counts_prev = total_counts;

		// 滤波
		float filter_alpha = 1.0f;
		data->vel_estimate = (1.0f - filter_alpha) * data->vel_estimate + filter_alpha * raw_vel;
	}

	// 更新位置估计（使用累积计数）
	data->pos_estimate = current_mech_angle * K_LINE;

	// 电速度
	data->vel_elec = data->vel_estimate * pole_pairs_f;
}
/* 其余函数保持不变 */
float read_feedback_elec_angle(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->elec_angle;
}

float read_feedback_velocity(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->vel_estimate;
}
float read_feedback_odome(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->pos_estimate;
}
void clear_feedback_odome(struct device *dev)
{
	struct feedback_data *data = dev->data;
	data->pos_estimate = 0.0f;
}
float feedback_calc_elec_velocity(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->vel_elec;
}

void feedback_reset(struct device *dev)
{
	struct feedback_data *data = dev->data;
#ifdef FEEDBACK_USE_PLL
	data->pll_pos = 0;
	data->pll_vel = 0;
#endif
	data->vel_estimate = 0;
	data->vel_elec = 0;
	data->pos_estimate = 0;
	data->mech_angle_prev = data->mech_angle;
}

void update_feedback_offset(struct device *dev, uint16_t offset)
{
	struct feedback_config *cfg = dev->config;
	cfg->offset = offset;
}

void update_feedback_pole_pairs(struct device *dev, uint8_t pole_pairs)
{
	struct feedback_config *cfg = dev->config;
	cfg->pole_pairs = pole_pairs;
}

void update_feedback_direction(struct device *dev, int8_t direction)
{
	if (!direction)
	{
		return;
	}
	struct feedback_config *cfg = dev->config;
	cfg->direction = direction;
}

float feedback_get_mech_angle(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->mech_angle;
}

uint32_t read_feedback_raw(struct device *feedback)
{
	struct feedback_config *fb_cfg = feedback->config;
	return fb_cfg->get_raw();
}

void update_feedback_cpr(struct device *feedback, uint32_t cpr)
{
	struct feedback_config *fb_cfg = feedback->config;
	fb_cfg->cpr = cpr;
}
