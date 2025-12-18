#include "feedback.h"
#include "device.h"
#include <stdint.h>
#include <math.h>
static float get_interpolated_error(const struct feedback_config *cfg, float pos);

// 角度归一化到 [0, 2PI]
static float normalize_angle(float angle)
{
	angle = fmodf(angle, 2.0f * M_PI);
	if (angle < 0) {
		angle += 2.0f * M_PI;
	}
	return angle;
}

// 处理角度环绕，返回连续角度差
static float inline angle_difference(float current, float previous)
{
	float diff = current - previous;
	if (diff > M_PI) {
		diff -= 2.0f * M_PI;
	} else if (diff < -M_PI) {
		diff += 2.0f * M_PI;
	}
	return diff;
}

// 初始化反馈模块
int feedback_init(struct device *dev)
{
	struct feedback_config *cfg = dev->config;
	struct feedback_data *data = dev->data;

	if (cfg->cpr == 0 || cfg->direction == 0 || cfg->pole_pairs == 0) {
		return -1;
	}
	data->accumulated_dt = 0.0f;
	data->total_counts = 0;
	data->prev_counts = 0;
	return 0;
}

void update_feedback(struct device *dev, float dt)
{
	struct feedback_config *cfg = dev->config;
	struct feedback_data *data = dev->data;

	const float two_pi = 2.0f * M_PI;
	const float cpr_f = (float)cfg->cpr;
	const int32_t cpr_i = (int32_t)cfg->cpr;
	const float pole_pairs_f = (float)cfg->pole_pairs;

	uint32_t current_raw = cfg->get_raw();
	data->raw = current_raw;

	int32_t adjusted_raw = (int32_t)current_raw - (int32_t)cfg->offset;
	adjusted_raw %= cpr_i;
	if (adjusted_raw < 0) {
		adjusted_raw += cpr_i;
	}
#if 0
	// 计算经LUT校正后的编码器计数
	float corrected_counts_f =
		(float)adjusted_raw - get_interpolated_error(cfg, (float)adjusted_raw);

	// 将校正后的计数规整到 [0, cpr) 范围内
	// 因为误差校正可能导致值略微超出范围
	corrected_counts_f = fmodf(corrected_counts_f, cpr_f);
	if (corrected_counts_f < 0.0f) {
		corrected_counts_f += cpr_f;
	}
	int32_t corrected_counts = (int32_t)roundf(corrected_counts_f);

	int32_t delta_counts = corrected_counts - data->prev_counts;
	if (delta_counts > cpr_i / 2) {
		delta_counts -= cpr_i;
	} else if (delta_counts < -cpr_i / 2) {
		delta_counts += cpr_i;
	}

	data->total_counts += delta_counts;
	data->prev_counts = corrected_counts; // 存储校正后的值供下次使用
#else
	int32_t delta_counts = adjusted_raw - data->prev_counts;
	if (delta_counts > cpr_i / 2) {
		delta_counts -= cpr_i;
	} else if (delta_counts < -cpr_i / 2) {
		delta_counts += cpr_i;
	}
	data->total_counts += delta_counts;
	data->prev_counts = adjusted_raw;
#endif
	float current_mech_angle =
		(two_pi / cpr_f) * (float)data->total_counts * (float)cfg->direction;

	data->elec_angle = normalize_angle(current_mech_angle * pole_pairs_f);

	data->accumulated_dt += dt;
	int32_t counts_diff = data->total_counts - data->total_counts_prev;

	if (data->accumulated_dt > 0.001f) {
		float angle_diff = (two_pi / cpr_f) * counts_diff * (float)cfg->direction;
		float raw_vel = angle_diff / data->accumulated_dt;
		data->accumulated_dt = 0.0f;
		data->total_counts_prev = data->total_counts;

		float filter_alpha = 0.15f;
		data->mech_vel = (1.0f - filter_alpha) * data->mech_vel + filter_alpha * raw_vel;
	}
}
static float get_interpolated_error(const struct feedback_config *cfg, float pos)
{
	// 1. 将位置映射到LUT的“连续索引”
	// LUT覆盖了从0到CPR的整个范围，pos_float的范围是 [0, cpr)
	float index_float = (pos * (float)LUT_SIZE) / ((float)cfg->cpr);

	// 2. 处理索引，确保其在LUT范围内（考虑周期性）
	// 由于角度是周期性的，我们对LUT索引也取模
	if (index_float < 0) {
		index_float += (float)LUT_SIZE;
	}
	if (index_float >= LUT_SIZE) {
		index_float -= (float)LUT_SIZE;
	}

	// 3. 找到用于插值的两个索引（i0 和 i1）
	int i0 = (int)floorf(index_float); // 下界索引
	float t = index_float - (float)i0; // 小数部分，即插值权重 (0 <= t < 1)

	// 由于LUT是周期性的，确保i0在[0, LUT_SIZE-1]内，i1处理“绕回”情况
	i0 = i0 % LUT_SIZE;
	int i1 = (i0 + 1) % LUT_SIZE; // 上界索引，在末尾处绕回0

	// 4. 获取这两个索引对应的误差值
	float error0 = (float)(cfg->lut[i0]);
	float error1 = (float)(cfg->lut[i1]);

	// 5. 执行线性插值： error = error0 + (error1 - error0) * t
	return error0 + (error1 - error0) * t;
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
	return data->mech_vel;
}
float read_feedback_odome(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->odom;
}

void update_feedback_odome(struct device *dev)
{
	struct feedback_data *data = dev->data;
	data->odom = 0.0f;
}

void feedback_reset(struct device *dev)
{
	struct feedback_data *data = dev->data;
	data->mech_vel = 0;
	data->odom = 0;
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
	if (!direction) {
		return;
	}
	struct feedback_config *cfg = dev->config;
	cfg->direction = direction;
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
