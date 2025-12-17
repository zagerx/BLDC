#include <stddef.h>
#include "foc_parameters.h"
#include "motor.h"
#include <statemachine.h>
#include <stdint.h>
// #include "_current_calib.h"
// #include "_pp_ident.h"
#include "motor_cfg.h"
#include "motor_carible.h"
#include "coord_transform.h"
#include "currsmp.h"
#include "feedback.h"
#include "inverter.h"
#include "svpwm.h"
// #include "arm_math.h"
#include "foc_pid.h"
#include "t_trajectory.h"
#undef M_PI
#define M_PI 3.14159265358979323846f
#undef RAD_TO_DEG
#define RAD_TO_DEG (180.0f / M_PI)

fsm_rt_t motor_encoder_openloop_state(fsm_cb_t *obj);
fsm_rt_t motor_debug_state(fsm_cb_t *obj);
fsm_rt_t motor_running_state(fsm_cb_t *obj);

fsm_rt_t motor_idle_state(fsm_cb_t *obj)
{
	struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	struct foc_parameters *foc_param = &(m_data->foc_data);

	enum {
		RUNING = USER_STATUS,
	};

	switch (obj->phase) {
	case ENTER:
		obj->phase = RUNING;
		s_planner_init(m_data->scp, 0.0f, 0.0f, 0.0f, SPEED_LOOP_CYCLE);
		break;
	case RUNING:
		if (((++obj->count) * PWM_CYCLE) >= SPEED_LOOP_CYCLE) {
			s_planner_action(m_data->scp, SPEED_LOOP_CYCLE);
			obj->count = 0;
		}
		break;
	case EXIT:
		break;
	default:
		break;
	}
	return 0;
}

fsm_rt_t motor_ready_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};

	switch (obj->phase) {
	case ENTER:
		obj->phase = RUNING;
		break;
	case RUNING:
		break;
	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}

fsm_rt_t motor_stop_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;

	switch (obj->phase) {
	case ENTER:
		m_data->statue = MOTOR_STATE_STOP;
		break;
	case RUNING:
		break;
	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}

fsm_rt_t motor_falut_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;

	switch (obj->phase) {
	case ENTER:
		m_data->statue = MOTOR_STATE_FAULT;
		break;
	case RUNING:
		break;
	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}

fsm_rt_t motor_carible_state(fsm_cb_t *obj)
{
	enum {
		RUNNING = USER_STATUS,
	};
	struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;

	switch (obj->phase) {
	case ENTER:
		obj->phase = RUNNING;
		break;
	case RUNNING:
		if (motor_calib_update(m_data->calib, PWM_CYCLE) == 1) {
			TRAN_STATE(m_data->state_machine, motor_encoder_openloop_state);
		}
		break;
	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}

fsm_rt_t motor_encoder_openloop_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
		IDLE,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	struct motor_config *m_cfg = motor->config;
	struct device *feedback = m_cfg->feedback;
	struct foc_parameters *foc_param = &(m_data->foc_data);
	struct device *currsmp = m_cfg->currsmp;
	struct device *inverer = m_cfg->inverter;
	switch (obj->phase) {
	case ENTER:
		if (feedback_init(feedback)) {
			break;
		}
		obj->phase = RUNING;
		break;
	case RUNING: {
		update_feedback(feedback, PWM_CYCLE);
		float elec_angle = read_feedback_elec_angle(feedback) * RAD_TO_DEG;

		float i_abc[3];
		float i_alpha, i_beta;
		currsmp_update_currents(currsmp, i_abc);
		clarke_f32(i_abc[0], i_abc[1], &i_alpha, &i_beta);
		update_focparam_idq(foc_param, i_alpha, i_beta, elec_angle);

		float sin_val, cos_val;
		sin_cos_f32(elec_angle, &sin_val, &cos_val);
		float ud, uq;
		float ualpha, ubeta;
		ud = 0.0f;
		uq = -0.02f;
		inv_park_f32(ud, uq, &ualpha, &ubeta, sin_val, cos_val);
		float duty[3];
		svm_set(ualpha, ubeta, duty);
		inverter_set_3phase_voltages(inverer, duty[0], duty[1], duty[2]);
	} break;
	case EXIT:
		inverter_set_3phase_voltages(inverer, 0.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}

	return 0;
}

fsm_rt_t motor_debug_idle_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};

	switch (obj->phase) {
	case ENTER:
		obj->phase = RUNING;
		break;

	case RUNING: {
		if (++obj->count > 10000) {
			obj->count = 0;
			TRAN_STATE(obj, motor_debug_state);
		}
	} break;

	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}

#define DEBUG_D_PI    0
#define DEBUG_Q_PI    1
#define DEBUG_VEL_PI  2
#define CURRENT_DEBUG DEBUG_VEL_PI
#include "stdio.h"
fsm_rt_t motor_debug_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
		WAIT,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	struct motor_config *m_cfg = motor->config;
	struct device *feedback = m_cfg->feedback;
	struct foc_parameters *foc_param = &(m_data->foc_data);
	struct device *currsmp = m_cfg->currsmp;
	struct device *inverer = m_cfg->inverter;

	switch (obj->phase) {
	case ENTER:
		if (feedback_init(feedback)) {
			break;
		}

		float kp, ki;
		float data[2] = {0};
#if (CURRENT_DEBUG == DEBUG_D_PI)
		read_foc_param_(foc_param, INDEX_D_PI, data);
		kp = data[0];
		ki = data[1];
		foc_pid_init(&foc_param->id_pi_control, kp, ki, 13.0f);
#elif (CURRENT_DEBUG == DEBUG_Q_PI)
		foc_pid_init(&foc_param->id_pi_control, kp, ki, 13.0f);
#elif (CURRENT_DEBUG == DEBUG_VEL_PI)
		read_foc_param_(foc_param, INDEX_VELOCITY_PI, data);
		foc_param->velocity_tar = 0.0f;
		kp = data[0];
		ki = data[1];

		foc_pid_init(&foc_param->id_pi_control, 0.01f, 30.0f, 13.0f);
		foc_pid_init(&foc_param->iq_pi_control, 0.01f, 30.0f, 13.0f);
		foc_pid_init(&foc_param->velocity_pi_control, kp, ki, 10.0f); // 0.08f,3.0f
#else
#endif
		obj->count = 0;
		obj->phase = WAIT;
		break;
	case WAIT:
		update_feedback(feedback, PWM_CYCLE);
		if (++obj->count > 1000) {

			obj->count = 0;
			obj->phase = RUNING;
		}
		break;
	case RUNING: {
		update_feedback(feedback, PWM_CYCLE);
		float elec_angle = read_feedback_elec_angle(feedback) * RAD_TO_DEG;

		float i_abc[3];
		float i_alpha, i_beta;
		currsmp_update_currents(currsmp, i_abc);
		clarke_f32(i_abc[0], i_abc[1], &i_alpha, &i_beta);
		update_focparam_idq(foc_param, i_alpha, i_beta, elec_angle);

		float id, iq;
		read_focparam_idq(foc_param, &id, &iq);

		// 步骤 1: 动态获取母线电压
		float v_bus = 24.0f; // inverter_get_bus_voltage(inverer);

		// 留一点裕量 (Margin)，比如 0.95，防止MOS管死区时间和采样噪声导致削顶
		float v_max_abs = v_bus * 0.57735f * 0.96f; // 0.57735 = 1/sqrt(3)
		float v_max_sq = SQ(v_max_abs);
#if (CURRENT_DEBUG == DEBUG_VEL_PI)
		// 速度环计算
		if (((++obj->count) * PWM_CYCLE) >= SPEED_LOOP_CYCLE) {
			obj->count = 0;
			foc_param->id_ref = 0.0f;
			foc_param->iq_ref = foc_pid_run(
				&(foc_param->velocity_pi_control), foc_param->velocity_tar,
				read_feedback_velocity(feedback), SPEED_LOOP_CYCLE);
		}
#endif
		// 步骤 2: PID 计算
		float ud_req, uq_req;
#if (CURRENT_DEBUG == DEBUG_D_PI)
		ud_req = foc_pid_run(&(foc_param->id_pi_control), foc_param->id_ref, id, PWM_CYCLE);
		uq_req = 0.0f;
#elif (CURRENT_DEBUG == DEBUG_Q_PI)
		ud_req = 0.0f;
		uq_req = foc_pid_run(&(foc_param->iq_pi_control), foc_param->iq_ref, iq, PWM_CYCLE);
#elif (CURRENT_DEBUG == DEBUG_VEL_PI)
		ud_req = foc_pid_run(&(foc_param->id_pi_control), foc_param->id_ref, id, PWM_CYCLE);
		uq_req = foc_pid_run(&(foc_param->iq_pi_control), foc_param->iq_ref, iq, PWM_CYCLE);
#else

#endif
		// 步骤 3: 优化的前馈解耦 (Decoupling)
		// float speed_elec = get_elec_velocity(feedback);
		// ud_req -= speed_elec * Lq * iq;
		// uq_req += speed_elec * (Ld * id + Flux_Linkage);

		// 步骤 4: 圆形限幅
		float v_mag_sq = SQ(ud_req) + SQ(uq_req);
		float ud_final = ud_req;
		float uq_final = uq_req;

		// 只有当请求电压超过物理极限时，才开方，节省CPU
		if (v_mag_sq > v_max_sq) {
			float v_mag;
			sqrt_f32(v_mag_sq, &v_mag);
			float scale = v_max_abs / v_mag;

			// 等比例压缩
			ud_final = ud_req * scale;
			uq_final = uq_req * scale;
		}

		// 步骤 5: 核心优化 - 积分抗饱和回馈 (Anti-Windup Feedback)
		foc_pid_saturation_feedback(&(foc_param->id_pi_control), ud_final, ud_req);
		foc_pid_saturation_feedback(&(foc_param->iq_pi_control), uq_final, uq_req);

		// // 步骤 6: 坐标变换与输出
#if (CURRENT_DEBUG == DEBUG_D_PI)
		// ud_final = 0.0f;
		uq_final = 0.0f;
#elif (CURRENT_DEBUG == DEBUG_Q_PI)
		ud_final = 0.0f;
		// uq_final = 0.0f;
#else
#endif
		// ud_final = 0.0f;
		// uq_final = 0.2f;
		// 归一化处理
		ud_final *= (1 / (24.0f * 0.57735f));
		uq_final *= (1 / (24.0f * 0.57735f));
		float sin_val, cos_val;
		sin_cos_f32(elec_angle, &sin_val, &cos_val);
		float ualpha, ubeta;
		// 使用最终限幅后的 ud_final, uq_final
		inv_park_f32(ud_final, uq_final, &ualpha, &ubeta, sin_val, cos_val);

		float duty[3];
		// 此时送入SVM的 ualpha, ubeta 模长已经被严格限制在内切圆内，不会削顶
		svm_set(ualpha, ubeta, duty);

		inverter_set_3phase_voltages(inverer, duty[0], duty[1], duty[2]);

	} break;

	case EXIT:
		foc_pid_reset(&foc_param->id_pi_control);
		foc_pid_reset(&foc_param->iq_pi_control);
		foc_pid_reset(&foc_param->velocity_pi_control);
		inverter_set_3phase_voltages(inverer, 0.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}

	return 0;
}
fsm_rt_t motor_running_state(fsm_cb_t *obj)
{
	enum {
		RUNING = USER_STATUS,
	};
	switch (obj->phase) {
	case ENTER:

		obj->phase = RUNING;
		break;

	case RUNING: {
	} break;

	case EXIT:
		break;
	default:
		break;
	}

	return 0;
}
