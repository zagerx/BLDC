#include "ekf_estimator.h"

// 角度归一化函数
static float normalize_angle(float angle)
{
	angle = fmodf(angle, 2.0f * M_PI);
	if (angle < 0) {
		angle += 2.0f * M_PI;
	}
	return angle;
}

bool ekf_init(ekf_estimator_t *ekf, float bandwidth_hz, float dt)
{
	if (!ekf || bandwidth_hz <= 0 || dt <= 0) {
		return false;
	}

	// 保存用户参数
	ekf->bandwidth_hz = bandwidth_hz;
	ekf->dt = dt;

	// ====== 设置默认噪声参数（关键！需要根据系统调试）=====
	// 这些参数决定了滤波器在“信任模型预测”和“信任传感器测量”之间的平衡
	float base_q = 0.01f;        // 过程噪声基值
	ekf->q_pos = base_q * 0.1f;  // 位置过程噪声
	ekf->q_vel = base_q * 10.0f; // 速度过程噪声 (通常比位置噪声大)

	// 测量噪声：与编码器分辨率相关。假设编码器CPR=4096，则单次测量误差方差约 (2π/4096)^2
	// ≈ 2.4e-6 这里设为稍大的值以容忍磁场不均匀导致的周期性误差
	ekf->r_pos = 1.0e-5f; // 约等于(0.003 rad)^2，可过滤高频噪声

	// ====== 初始化状态 ======
	ekf->pos_est = 0.0f;
	ekf->vel_est = 0.0f;

	// 初始化协方差矩阵：较大的初始不确定度
	ekf->p00 = 1.0f;   // 位置方差初始值
	ekf->p11 = 100.0f; // 速度方差初始值 (通常更不确定)
	ekf->p01 = 0.0f;
	ekf->p10 = 0.0f;

	// ====== 初始化内部变量 ======
	ekf->last_raw_phase = 0.0f;
	ekf->last_raw_count = 0;
	ekf->total_revs = 0;

	ekf->initialized = true;

	// 初始化输出
	ekf->output_wrapped_angle = 0.0f;
	ekf->output_velocity = 0.0f;
	ekf->output_continuous_pos = 0.0f;

	return true;
}

bool ekf_update(ekf_estimator_t *ekf, int32_t adjusted_raw, float dt)
{
	if (!ekf || !ekf->initialized || dt <= 0) {
		return false;
	}

	// 步骤 1: 将 raw count 转换为包装相位 (0-2π)
	// 假设 adjusted_raw 对应编码器的 CPR (每转计数)
	const int32_t cpr = 16384; // 需要根据您的编码器实际CPR修改！
	float raw_phase = (2.0f * M_PI * (float)adjusted_raw) / (float)cpr;
	raw_phase = normalize_angle(raw_phase);

	// 步骤 2: 处理相位卷绕，更新连续位置计数
	float phase_diff = raw_phase - ekf->last_raw_phase;

	// 处理相位跳变（过零点）
	if (phase_diff > M_PI) {
		// 逆时针跨越零点：实际是负向运动
		phase_diff -= 2.0f * M_PI;
		ekf->total_revs--;
	} else if (phase_diff < -M_PI) {
		// 顺时针跨越零点
		phase_diff += 2.0f * M_PI;
		ekf->total_revs++;
	}

	// 计算连续传感器相位（仅用于参考，不是EKF的直接测量）
	float sensor_continuous_phase = raw_phase + 2.0f * M_PI * ekf->total_revs;

	// 保存当前 raw 值用于下一次
	ekf->last_raw_phase = raw_phase;
	ekf->last_raw_count = adjusted_raw;

	// ====== EKF 预测步骤 (Prediction) ======
	// 状态转移: x_k|k-1 = F * x_k-1
	// 使用匀速模型: pos_k = pos_k-1 + vel_k-1 * dt
	//               vel_k = vel_k-1
	float pos_pred = ekf->pos_est + ekf->vel_est * dt;
	float vel_pred = ekf->vel_est;

	// 协方差预测: P_k|k-1 = F * P_k-1 * F^T + Q
	// 状态转移矩阵 F = [[1, dt], [0, 1]]
	float f00 = 1.0f, f01 = dt, f10 = 0.0f, f11 = 1.0f;

	// 计算 F * P
	float fp00 = f00 * ekf->p00 + f01 * ekf->p10;
	float fp01 = f00 * ekf->p01 + f01 * ekf->p11;
	float fp10 = f10 * ekf->p00 + f11 * ekf->p10;
	float fp11 = f10 * ekf->p01 + f11 * ekf->p11;

	// 计算 (F * P) * F^T
	float p00_pred = fp00 * f00 + fp01 * f01 + ekf->q_pos;
	float p01_pred = fp00 * f10 + fp01 * f11;
	float p10_pred = fp10 * f00 + fp11 * f01;
	float p11_pred = fp10 * f10 + fp11 * f11 + ekf->q_vel;

	// ====== EKF 更新步骤 (Update) ======
	// 测量值: 包装相位 + 圈数信息 = 连续相位
	float z = sensor_continuous_phase; // 测量值

	// 测量残差 (Innovation): y = z - H * x_pred
	// 测量矩阵 H = [1, 0] (我们只测量位置)
	float y = z - pos_pred; // 位置残差

	// 处理角度残差的周期性问题（重要！）
	if (y > M_PI) {
		y -= 2.0f * M_PI;
	} else if (y < -M_PI) {
		y += 2.0f * M_PI;
	}

	// 残差协方差: S = H * P_pred * H^T + R
	// 由于 H = [1, 0], S = p00_pred + r_pos
	float s = p00_pred + ekf->r_pos;
	float s_inv = 1.0f / s; // S 的逆

	// 卡尔曼增益: K = P_pred * H^T * S^-1
	float k0 = p00_pred * s_inv;
	float k1 = p10_pred * s_inv;

	// 状态更新: x_k = x_pred + K * y
	ekf->pos_est = pos_pred + k0 * y;
	ekf->vel_est = vel_pred + k1 * y;

	// 协方差更新: P_k = (I - K * H) * P_pred
	// I - K * H = [[1-k0, 0], [-k1, 1]]
	ekf->p00 = (1.0f - k0) * p00_pred;
	ekf->p01 = (1.0f - k0) * p01_pred;
	ekf->p10 = -k1 * p00_pred + p10_pred;
	ekf->p11 = -k1 * p01_pred + p11_pred;

	// 确保协方差矩阵对称
	ekf->p01 = (ekf->p01 + ekf->p10) * 0.5f;
	ekf->p10 = ekf->p01;

	// 强制协方差正定性（防止数值问题）
	if (ekf->p00 < 0) {
		ekf->p00 = 0.001f;
	}
	if (ekf->p11 < 0) {
		ekf->p11 = 0.001f;
	}

	// ====== 设置输出（严格匹配PLL结构）=====
	ekf->output_wrapped_angle = normalize_angle(ekf->pos_est); // 包装角度
	ekf->output_velocity = ekf->vel_est;                       // 速度
	ekf->output_continuous_pos = ekf->pos_est;                 // 连续位置

	return true;
}