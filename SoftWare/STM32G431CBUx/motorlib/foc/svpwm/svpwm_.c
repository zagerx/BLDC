#include "foc_data.h"
#include <math.h>
#include <float.h>
#undef RAD_TO_DEG
#define RAD_TO_DEG (180.0f / M_PI)
static inline float clamp_0_1(float x)
{
	if (x < 0.0f) {
		return 0.0f;
	}
	if (x > 1.0f) {
		return 1.0f;
	}
	return x;
}

/*
 * Seven-Segment SVPWM (Projection Method)
 */
void svpwm_seven_segment(float v_alpha, float v_beta, float *duty_a, float *duty_b, float *duty_c)
{
	/* ----------- 奇点处理 ----------- */
	if (fabsf(v_alpha) < FLT_EPSILON && fabsf(v_beta) < FLT_EPSILON) {
		*duty_a = 0.5f;
		*duty_b = 0.5f;
		*duty_c = 0.5f;
		return;
	}

	/* ----------- αβ → abc 投影（α 轴对齐 A 相） ----------- */
	float va = v_alpha;
	float vb = -0.5f * v_alpha + 0.8660254037844386f * v_beta; // +sqrt(3)/2
	float vc = -0.5f * v_alpha - 0.8660254037844386f * v_beta;

	/* ----------- 排序：max / mid / min ----------- */
	float vmax = fmaxf(va, fmaxf(vb, vc));
	float vmin = fminf(va, fminf(vb, vc));

	/* ----------- 关键增益校准（封版） ----------- */
	const float GAIN = 2.0f / 3.0f;

	float T1_plus_T2 = (vmax - vmin) * GAIN;
	T1_plus_T2 = clamp_0_1(T1_plus_T2);

	float T0 = 1.0f - T1_plus_T2;
	T0 = clamp_0_1(T0);

	float offset = 0.5f * T0;

	/* ----------- 七段式等效占空比 ----------- */
	float da = (va - vmin) * GAIN + offset;
	float db = (vb - vmin) * GAIN + offset;
	float dc = (vc - vmin) * GAIN + offset;

	*duty_a = clamp_0_1(da);
	*duty_b = clamp_0_1(db);
	*duty_c = clamp_0_1(dc);
}

/**
 * @brief foc 矢量调制执行
 * * 职责：逆 park 变换 -> 电压归一化 -> svpwm 映射
 * 注意：输入 ud/uq 必须在外部已完成模长限幅，本函数仅负责数学映射
 */
void foc_modulate_dq(float eangle, float vbus, float ud, float uq, float *ualpha, float *ubeta)
{
	float sin_val, cos_val;
	float alpha, beta;

	/* ----------- 1. 坐标逆变换 (dq -> αβ) ----------- */
	sin_cos_f32(eangle * RAD_TO_DEG, &sin_val, &cos_val);
	inv_park_f32(ud, uq, &alpha, &beta, sin_val, cos_val);

	/* ----------- 2. 归一化 (物理电压 -> 调制比) ----------- */
	if (vbus > 0.001f) {
		// 1.732 是 sqrt(3)
		const float inv_vbase = 1.73205080757f / vbus;
		alpha *= inv_vbase;
		beta *= inv_vbase;
	} else {
		alpha = 0.0f;
		beta = 0.0f;
	}
	*ualpha = alpha;
	*ubeta = beta;
}

/**
 * @brief 电压矢量等比例限幅
 * @param f_data: 包含母线电压 vbus 的结构体
 * @param ud: 指向 d 轴电压的指针（输入并原地修改）
 * @param uq: 指向 q 轴电压的指针（输入并原地修改）
 * * 逻辑：
 * 1. 计算线性区极限 u_limit = Vbus / sqrt(3)
 * 2. 如果 sqrt(ud^2 + uq^2) > u_limit，则等比例缩减 ud 和 uq
 */
void foc_limit_voltage_vector(float vbus, float *ud, float *uq)
{
	// 1. 计算线性区极限 (Vdc * (1/sqrt(3)))
	// 0.57735027f 是 1.0 / sqrt(3)
	float u_limit = vbus * 0.57735027f;

	// 2. 计算当前矢量的模长平方
	float u_sq = (*ud) * (*ud) + (*uq) * (*uq);
	float u_limit_sq = u_limit * u_limit;

	// 3. 比较平方值，避免不必要的开方运算以节省算力
	if (u_sq > u_limit_sq) {
		// 计算缩放系数 factor = u_limit / sqrt(u_sq)
		float u_mag = sqrtf(u_sq);
		float factor = u_limit / u_mag;

		// 4. 等比例缩放，保持相位不变
		*ud *= factor;
		*uq *= factor;
	}
}
