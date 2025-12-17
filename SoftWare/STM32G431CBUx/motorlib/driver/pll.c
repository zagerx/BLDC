#include "pll.h"
/* ============================================
 * PLL初始化
 * 参数：带宽（Hz），采样周期（s）
 * ============================================ */
bool pll_init(pll_estimator_t *pll, float bandwidth_hz, float dt)
{
	if (!pll || bandwidth_hz <= 0 || dt <= 0) {
		return false;
	}

	memset(pll, 0, sizeof(pll_estimator_t));

	// ODRIVER参数计算方法
	float omega_n = 2.0f * 3.1415926535f * bandwidth_hz;
	pll->kp = 2.0f * omega_n;    // 临界阻尼：kp = 2ξω_n, ξ=1
	pll->ki = omega_n * omega_n; // ki = ω_n²
	pll->bandwidth = bandwidth_hz;

	pll->initialized = true;
	return true;
}

/* ============================================
 * PLL更新函数（符合ODRIVER原理）
 *
 * 关键改进：
 * 1. 使用正确的相位误差计算方法
 * 2. 实现积分抗饱和
 * 3. 保证位置的连续性和平滑性
 * ============================================ */
bool pll_update(pll_estimator_t *pll, int32_t adjusted_raw, float dt)
{
	if (!pll || !pll->initialized || dt <= 0.0f) {
		return false;
	}

	// ============ 1. 将原始值转换为相位（0-2π） ============
	const float cpr = 16384.0f;
	const float rad_per_count = 2.0f * 3.1415926535f / cpr;

	float phase = (float)adjusted_raw * rad_per_count;

	// 确保相位在[0, 2π)
	if (phase < 0) {
		phase += 2.0f * 3.1415926535f;
	}
	if (phase >= 2.0f * 3.1415926535f) {
		phase -= 2.0f * 3.1415926535f;
	}

	// 首次调用时初始化
	if (pll->last_phase == 0.0f && pll->pos_est == 0.0f) {
		pll->last_phase = phase;
		pll->pos_est = phase; // 初始位置设置为当前相位
		pll->output_wrapped_angle = phase;
		pll->output_continuous_pos = phase;
		return true;
	}

	// ============ 2. 计算相位误差（ODRIVER方式） ============
	// 将PLL估计位置包装到[0, 2π)
	float wrapped_pll_pos = pll->pos_est;
	while (wrapped_pll_pos < 0) {
		wrapped_pll_pos += 2.0f * 3.1415926535f;
	}
	while (wrapped_pll_pos >= 2.0f * 3.1415926535f) {
		wrapped_pll_pos -= 2.0f * 3.1415926535f;
	}

	// 计算相位差（当前测量相位与包装后的PLL位置之差）
	float phase_error = phase - wrapped_pll_pos;

	// 将相位误差归一化到[-π, π]
	if (phase_error > 3.1415926535f) {
		phase_error -= 2.0f * 3.1415926535f;
	} else if (phase_error < -3.1415926535f) {
		phase_error += 2.0f * 3.1415926535f;
	}

	// ============ 3. 更新积分器（先于速度计算） ============
	float integrator_new = pll->integrator + pll->ki * phase_error * dt;

	// ============ 4. 计算速度命令 ============
	float vel_cmd = pll->kp * phase_error + integrator_new;

	// ============ 5. 速度限制和积分抗饱和 ============
	// ODRIVER方式：根据带宽限制最大速度
	float max_vel = 2.0f * 3.1415926535f * pll->bandwidth;

	if (vel_cmd > max_vel) {
		vel_cmd = max_vel;
		// 积分抗饱和：如果速度超限，且误差方向与超限方向一致，则停止积分
		if (phase_error > 0) {
			integrator_new = pll->integrator; // 不更新积分器
		}
	} else if (vel_cmd < -max_vel) {
		vel_cmd = -max_vel;
		if (phase_error < 0) {
			integrator_new = pll->integrator; // 不更新积分器
		}
	}

	// 更新积分器
	pll->integrator = integrator_new;

	// ============ 6. 速度低通滤波 ============
	// 一阶低通滤波器，时间常数由带宽决定
	float alpha = 2.0f * 3.1415926535f * pll->bandwidth * dt;
	alpha = alpha / (1.0f + alpha); // 离散化系数

	float vel_est_new = (1.0f - alpha) * pll->vel_est + alpha * vel_cmd;

	// ============ 7. 更新连续位置估计 ============
	// 这是最关键的一步：通过积分速度得到连续位置
	float pos_est_new = pll->pos_est + vel_est_new * dt;

	// ============ 8. 检查角度连续性 ============
	// 计算测量相位的增量（处理2π跳变）
	float phase_diff = phase - pll->last_phase;
	if (phase_diff > 3.1415926535f) {
		phase_diff -= 2.0f * 3.1415926535f;
	} else if (phase_diff < -3.1415926535f) {
		phase_diff += 2.0f * 3.1415926535f;
	}

	// 如果测量相位有大幅跳变（可能是PLL失锁），重新对齐
	if (fabsf(phase_error) > 1.0f && fabsf(phase_diff) < 0.1f) {
		// 相位误差大但实际相位变化小，说明PLL估计有问题
		// 直接将位置对齐到测量相位（保持连续性）
		float pos_correction = phase_error;
		pos_est_new = pll->pos_est + pos_correction;
		vel_est_new = phase_diff / dt; // 使用测量增量作为速度
	}

	// ============ 9. 更新状态和输出 ============
	pll->vel_est = vel_est_new;
	pll->pos_est = pos_est_new;
	pll->last_phase = phase;

	// 包装输出角度
	float output_angle = pos_est_new;
	while (output_angle < 0) {
		output_angle += 2.0f * 3.1415926535f;
	}
	while (output_angle >= 2.0f * 3.1415926535f) {
		output_angle -= 2.0f * 3.1415926535f;
	}

	pll->output_wrapped_angle = output_angle;
	pll->output_velocity = vel_est_new;
	pll->output_continuous_pos = pos_est_new;

	return true;
}