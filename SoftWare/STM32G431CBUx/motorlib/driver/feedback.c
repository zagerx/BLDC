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
int feedback_init(struct device *dev)
{
	struct feedback_config *cfg = dev->config;
	struct feedback_data *data = dev->data;

	if (cfg->cpr == 0 || cfg->direction == 0 || cfg->pole_pairs == 0) {
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

#ifdef FEEDBACK_USE_VEL_FILTER
	// 默认控制频率为10kHz，截止频率设为50Hz
	lpf_init(&data->vel_filter, 50.0f, PWM_FREQ);
#endif
	return 0;
}

// 角度/速度更新
void update_feedback(struct device *dev, float dt)
{
	struct feedback_config *cfg = dev->config;
	struct feedback_data *data = dev->data;

	// --- 局部变量缓存配置和常用常量 ---
	const float two_pi = 2.0f * M_PI;
	const float cpr_f = (float)cfg->cpr;
	const int32_t cpr_i = (int32_t)cfg->cpr;
	const float pole_pairs_f = (float)cfg->pole_pairs;

	// 保存上一次的角度值
	float mech_angle_prev = data->mech_angle;
	data->mech_angle_prev = mech_angle_prev;
	// data->elec_angle_prev 已删除 (冗余)

	// 获取原始编码器值
	data->raw = cfg->get_raw();

	// 考虑编码器偏移量并转为调整后的原始值
	int32_t adjusted_raw = (int32_t)data->raw - (int32_t)cfg->zero_offset;
	adjusted_raw %= cpr_i;

	if (adjusted_raw < 0) {
		adjusted_raw += cpr_i;
	}

	// 计算机械角度、电角度（新的计算结果）
	float raw_angle = two_pi * (float)adjusted_raw / cpr_f;
	float current_mech_angle = raw_angle * (float)cfg->direction;
	float current_elec_angle = normalize_angle(current_mech_angle * pole_pairs_f);

	// 更新状态结构体中的角度值
	data->mech_angle = current_mech_angle;
	data->elec_angle = current_elec_angle;

	/* ========== 编译期二选一：差分法（默认）或 PLL（若定义 FEEDBACK_USE_PLL） ========== */
#ifdef FEEDBACK_USE_PLL

	/* 仅使用 PLL 路径：基于电角度的 PLL 估计位置与速度 */
	if (dt > 1e-6f) {
		// --- 局部变量缓存/更新 PLL 状态 ---
		float pll_pos = data->pll_pos;
		float pll_vel = data->pll_vel;

		/* 计算 PLL 误差（电角度） */
		float pll_error = angle_difference(current_elec_angle, pll_pos);

		/* 动态增益 */
		float bandwidth = fminf(0.05f * fabsf(pll_vel) + 1.0f, 10.0f);
		float kp = cfg->pll_kp * bandwidth;
		float ki = cfg->pll_ki * bandwidth;

		/* 更新 PLL 积分项（速度）和位置 */
		pll_vel += ki * pll_error * dt;
		pll_pos += pll_vel * dt + kp * pll_error;
		pll_pos = normalize_angle(pll_pos);

		// --- 写回 PLL 状态 ---
		data->pll_pos = pll_pos;
		data->pll_vel = pll_vel;

		/* 使用 PLL 输出作为电速度和机械速度估计 */
		data->vel_elec = pll_vel;
		if (cfg->pole_pairs != 0) {
			data->vel_estimate = data->vel_elec / pole_pairs_f;
		} else {
			data->vel_estimate = 0.0f;
		}
	}

#else

	/* 差分法路径（抗丢帧优化版） */
	float mech_angle_diff = angle_difference(current_mech_angle, mech_angle_prev);

	// 累加时间
	data->accumulated_dt += dt;

	// 只有当角度发生变化（检测到脉冲），或者时间累积太久（防止堵转误判）时，才计算速度
	// 1e-5f 是一个极小的角度阈值，约等于0.5个count (视CPR而定，这里假设防抖)
	// 0.05f 是最大等待时间（50ms），防止电机真停了但速度一直保持非0
	if (fabsf(mech_angle_diff) > 1e-5f || data->accumulated_dt > 0.05f) {

		// 使用累积的时间进行除法
		// 正常情况：accumulated_dt = dt, diff = 1, vel = 1/dt = 40
		// 丢帧情况：accumulated_dt = 2*dt, diff = 2, vel = 2/(2*dt) = 40 (完美解决！)
		float raw_vel = mech_angle_diff / data->accumulated_dt;

		// 重置累积时间
		data->accumulated_dt = 0.0f;

		// 在这里应用低通滤波 (系数根据需要调整，0.1-0.3)
		// 注意：这是对“计算出的速度”滤波，不是对角度
		const float filter_alpha = 0.15f; // 新值权重 15%
		data->vel_estimate =
			(1.0f - filter_alpha) * data->vel_estimate + filter_alpha * raw_vel;
	}
	/* 电速度 = 机械速度 * 极对数 */
	data->vel_elec = data->vel_estimate * pole_pairs_f;

#endif

	/* 位置估计（用于抗噪声） */
	data->pos_estimate = (1.0f - cfg->pos_estimate_weight) * data->pos_estimate +
			     cfg->pos_estimate_weight * current_mech_angle;

	/* 限制速度范围（对两种路径通用） */
	const float max_mech_vel = 100.0f;
	const float max_elec_vel = max_mech_vel * pole_pairs_f;

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

void feedback_reset(struct device *dev)
{
	struct feedback_data *data = dev->data;
#ifdef FEEDBACK_USE_PLL
	data->pll_pos = 0;
	data->pll_vel = 0;
#endif
	data->turns = 0;
	data->vel_estimate = 0;
	data->vel_elec = 0;
	data->pos_estimate = 0;
	data->mech_angle_prev = data->mech_angle;
}

void update_feedback_offset(struct device *dev, uint16_t offset)
{
	struct feedback_config *cfg = dev->config;
	cfg->zero_offset = offset;
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

float feedback_get_mech_angle(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->mech_angle;
}

float feedback_get_continuous_mech_angle(struct device *dev)
{
	struct feedback_data *data = dev->data;
	return data->mech_angle + 2.0f * M_PI * data->turns;
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
