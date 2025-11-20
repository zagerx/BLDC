#include "feedback.h"
#include "device.h"
#include <stdint.h>
#include <math.h>

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
static float angle_difference(float current, float previous)
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
void feedback_init(struct device *dev)
{
	struct feedback_config *cfg = dev->config;
	struct feedback_data *data = dev->data;

	// 设置默认值
	if (cfg->cpr == 0) {
		cfg->cpr = AS5047_CPR;
	}
	if (cfg->direction == 0) {
		cfg->direction = 1; // 默认正向
	}
	if (cfg->pole_pairs == 0) {
		cfg->pole_pairs = 7; // 默认7极对数，根据实际电机调整
	}
	if (cfg->pll_kp == 0) {
		cfg->pll_kp = 100.0f; // 默认PLL参数
	}
	if (cfg->pll_ki == 0) {
		cfg->pll_ki = 1000.0f;
	}
	if (cfg->pos_estimate_weight == 0) {
		cfg->pos_estimate_weight = 0.1f;
	}

	data->pll_kp_scale = 1.0f;
}

void feedback_update_angle_vel(struct device *dev, float dt)
{
	struct feedback_config *cfg = dev->config;
	struct feedback_data *data = dev->data;

	// 保存上一次的值
	data->raw_prev = data->raw;
	data->raw_angle_prev = data->raw_angle;
	data->mech_angle_prev = data->mech_angle;
	data->elec_angle_prev = data->elec_angle;

	// 获取原始编码器值
	data->raw = cfg->get_raw();

	// 考虑编码器偏移量
	int32_t adjusted_raw = (int32_t)data->raw - (int32_t)cfg->offset;
	if (adjusted_raw < 0) {
		adjusted_raw += cfg->cpr;
	}
	adjusted_raw %= cfg->cpr;

	// 计算原始机械角度 (0 到 2PI)
	data->phase = (float)adjusted_raw / (float)cfg->cpr;
	data->raw_angle = 2.0f * M_PI * data->phase;

	// 应用编码器方向
	data->mech_angle = data->raw_angle * (float)cfg->direction;

	// 计算电角度 = 机械角度 * 极对数
	data->elec_angle = data->mech_angle * (float)cfg->pole_pairs;
	data->elec_angle = normalize_angle(data->elec_angle);

	// 计算机械角度差（考虑方向）
	float mech_angle_diff = angle_difference(data->mech_angle, data->mech_angle_prev);

	// 计算电角度差
	float elec_angle_diff = angle_difference(data->elec_angle, data->elec_angle_prev);

	// 方法1: 直接差分计算机械速度
	if (dt > 1e-6f) {
		data->vel_estimate = mech_angle_diff / dt;
	}

	// 计算电速度 = 机械速度 * 极对数
	data->vel_elec = data->vel_estimate * (float)cfg->pole_pairs;

	// 方法2: PLL速度估计 (基于电角度)
	if (cfg->pll_kp > 0 && dt > 1e-6f) {
		// 计算PLL误差
		float pll_error = angle_difference(data->elec_angle, data->pll_pos);

		// 动态调整PLL增益（基于速度）
		float bandwidth = fminf(0.05f * fabsf(data->pll_vel) + 1.0f, 10.0f);
		float kp = cfg->pll_kp * bandwidth;
		float ki = cfg->pll_ki * bandwidth;

		// 更新PLL速度
		data->pll_vel += ki * pll_error * dt;

		// 更新PLL位置
		data->pll_pos += data->pll_vel * dt + kp * pll_error;
		data->pll_pos = normalize_angle(data->pll_pos);

		// 使用PLL速度作为最终电速度估计
		data->vel_elec = data->pll_vel;

		// 根据电速度计算机械速度
		data->vel_estimate = data->vel_elec / (float)cfg->pole_pairs;
	}

	// 位置估计（用于抗噪声）
	data->pos_estimate = (1.0f - cfg->pos_estimate_weight) * data->pos_estimate +
			     cfg->pos_estimate_weight * data->mech_angle;

	// 限制速度范围
	const float max_mech_vel = 100.0f;                                // 最大机械速度 (rad/s)
	const float max_elec_vel = max_mech_vel * (float)cfg->pole_pairs; // 最大电速度

	if (data->vel_estimate > max_mech_vel) {
		data->vel_estimate = max_mech_vel;
	} else if (data->vel_estimate < -max_mech_vel) {
		data->vel_estimate = -max_mech_vel;
	}

	if (data->vel_elec > max_elec_vel) {
		data->vel_elec = max_elec_vel;
	} else if (data->vel_elec < -max_elec_vel) {
		data->vel_elec = -max_elec_vel;
	}
}

// 计算电角度
float feedback_calc_elec_angle(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->elec_angle;
}

// 计算机械速度
float feedback_calc_velocity(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->vel_estimate;
}

// 计算电速度
float feedback_calc_elec_velocity(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->vel_elec;
}

// 重置反馈状态
void feedback_reset(struct device *dev)
{
	struct feedback_data *data = dev->data;
	data->pll_pos = 0;
	data->pll_vel = 0;
	data->turns = 0;
	data->vel_estimate = 0;
	data->vel_elec = 0;
	data->pos_estimate = 0;
	data->raw_angle_prev = data->raw_angle;
	data->mech_angle_prev = data->mech_angle;
	data->elec_angle_prev = data->elec_angle;
}

// 设置编码器偏移量
void feedback_set_offset(struct device *dev, uint16_t offset)
{
	struct feedback_config *cfg = dev->config;
	cfg->offset = offset;
}

// 设置极对数
void feedback_set_pole_pairs(struct device *dev, uint8_t pole_pairs)
{
	struct feedback_config *cfg = dev->config;
	cfg->pole_pairs = pole_pairs;
}

// 设置编码器方向
void feedback_set_direction(struct device *dev, int8_t direction)
{
	struct feedback_config *cfg = dev->config;
	cfg->direction = direction;
}

// 获取机械角度
float feedback_get_mech_angle(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->mech_angle;
}

// 获取连续机械角度（考虑圈数）
float feedback_get_continuous_mech_angle(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->mech_angle + 2.0f * M_PI * data->turns;
}