#include "foc.h"
extern void svpwm_seven_segment(float v_alpha, float v_beta, float *duty_a, float *duty_b,
				float *duty_c);

#undef M_PI
#define M_PI 3.14159265358979323846f
#undef RAD_TO_DEG
#define RAD_TO_DEG (180.0f / M_PI)

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

void foc_posloop_init(struct foc_data *f_data)
{
	struct foc_pid *pos_pi = &f_data->controller.postion;
#if 0
		kp = f_data->controller.postion.params->kp;
		ki = f_data->controller.postion.params->ki;
		foc_pid_init(pos_pi, kp, ki, 2000.0f);
#else
	foc_pid_init(pos_pi, 10.0f, 1.0f, 5000.0f); // 参数还需优化
#endif
}

void foc_posloop_deinit(struct foc_data *f_data)
{
	struct foc_pid *pos_pi = &f_data->controller.postion;
	foc_pid_reset(pos_pi);
}

void foc_posloop(struct foc_data *f_data, float tar, float dt)
{
	struct foc_pid *pos_pi = &f_data->controller.postion;
	f_data->ref.velocity = foc_pid_run(pos_pi, tar, f_data->meas.position, dt);
}

void foc_velocityloop_init(struct foc_data *f_data)
{
	struct foc_pid *velocity_pi = &f_data->controller.velocity;
#if 0
    	float kp,ki;
	kp = f_data->controller.velocity.params->kp;
	ki = f_data->controller.velocity.params->ki;
	foc_pid_init(velocity_pi, kp, ki, 13.0f);
#else
	foc_pid_init(velocity_pi, 0.0008f, 0.004, 10.0f);
#endif
}
void foc_velocityloop_deinit(struct foc_data *f_data)
{
	struct foc_pid *velocity_pi = &f_data->controller.velocity;
	foc_pid_reset(velocity_pi);
}
void foc_velocity_loop(struct foc_data *f_data, float tar, float dt)
{
	struct foc_pid *velocity_pi = &f_data->controller.velocity;
	float velocity = f_data->meas.velocity;
	// float acc;
	f_data->ref.id = 0.0f;
	f_data->ref.iq = foc_pid_run(velocity_pi, tar, velocity, dt);
	// acc = s_planner_get_acc(f_data->scp);
	// f_data->ref.iq += acc * 0.2f;
}

void foc_currentloop_init(struct foc_data *f_data)
{
	struct foc_pid *d_pi = &f_data->controller.id;
	struct foc_pid *q_pi = &f_data->controller.iq;
#if 0
	float kp, ki;
	kp = f_data->controller.id.params->kp;
	ki = f_data->controller.id.params->ki;
	foc_pid_init(d_pi, kp, ki, 13.0f);
	foc_pid_init(q_pi, kp, ki, 13.0f);
#else
	foc_pid_init(d_pi, 0.01f, 30.0f, 13.0f);
	foc_pid_init(q_pi, 0.01f, 30.0f, 13.0f);
#endif
}
void foc_currentloop_deinit(struct foc_data *f_data)
{
	struct foc_pid *d_pi = &f_data->controller.id;
	struct foc_pid *q_pi = &f_data->controller.iq;
	foc_pid_reset(d_pi);
	foc_pid_reset(q_pi);
}
void foc_currentloop(struct foc_data *f_data, float *ud, float *uq, float dt)
{
	struct foc_pid *d_pi, *q_pi;
	d_pi = &f_data->controller.id;
	q_pi = &f_data->controller.iq;
	float ud_req, uq_req;
	ud_req = foc_pid_run(d_pi, f_data->ref.id, f_data->meas.id, dt);
	uq_req = foc_pid_run(q_pi, f_data->ref.iq, f_data->meas.iq, dt);

	// 步骤 3: 优化的前馈解耦 (Decoupling)
	// float speed_elec = get_elec_velocity(feedback);
	// ud_req -= speed_elec * Lq * iq;
	// uq_req += speed_elec * (Ld * id + Flux_Linkage);

	float ud_final = ud_req;
	float uq_final = uq_req;
	foc_limit_voltage_vector(f_data->meas.vbus, &ud_final, &uq_final);
	foc_pid_saturation_feedback(d_pi, ud_final, ud_req);
	foc_pid_saturation_feedback(q_pi, uq_final, uq_req);

	*ud = ud_final;
	*uq = uq_final;
}
