#include <stddef.h>
#include "foc_data.h"
#include "motor.h"
#include <statemachine.h>
#include <stdint.h>
#include "motor_cfg.h"
#include "motor_carible.h"
#include "coord_transform.h"
#include "currsmp.h"
#include "feedback.h"
#include "inverter.h"
#include "svpwm.h"
#include "foc_pid.h"
#include "t_trajectory.h"
#include "foc.h"

fsm_rt_t motor_encoder_openloop_state(fsm_cb_t *obj);
fsm_rt_t motor_debug_state(fsm_cb_t *obj);
fsm_rt_t motor_running_state(fsm_cb_t *obj);

fsm_rt_t motor_idle_state(fsm_cb_t *obj)
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
		if (calibration_modules_update(&m_data->calib, PWM_CYCLE) == 1) {
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
	struct feedback_t *feedback = m_cfg->feedback;
	struct foc_data *f_data = &(m_data->foc_data);
	struct currsmp_t *currsmp = m_cfg->currsmp;
	struct inverter_t *inverter = m_cfg->inverter;

	struct foc_pid *velocity_pi = &f_data->controller.velocity;
	switch (obj->phase) {
	case ENTER:
		obj->count = 0;
		if (feedback_init(feedback)) {
			break;
		}
		obj->phase = IDLE;
		break;
	case IDLE: {
		update_feedback(feedback, PWM_CYCLE);
		float elec_angle = read_feedback_elec_angle(feedback);
		float i_abc[3];
		currsmp_update_currents(currsmp, i_abc);
		foc_update_current_idq(f_data, i_abc, elec_angle);
	}; break;
	case RUNING: {
		update_feedback(feedback, PWM_CYCLE);
		if (((obj->count) % VELOCITY_LOOP_DIVIDER == 0)) {
			f_data->meas.velocity = read_feedback_velocity(feedback, SPEED_LOOP_CYCLE);
			f_data->ref.id = 0.0f;
			f_data->ref.iq = foc_pid_run(velocity_pi, f_data->ref.velocity,
						     f_data->meas.velocity, SPEED_LOOP_CYCLE);
		}
		obj->count++;
		float elec_angle = read_feedback_elec_angle(feedback);
		float i_abc[3];
		currsmp_update_currents(currsmp, i_abc);
		foc_update_current_idq(f_data, i_abc, elec_angle);
		float ud, uq;
		ud = 0.0f;
		uq = -0.02f;
		foc_apply_voltage_dq(inverter, ud, uq, elec_angle);
	} break;
	case EXIT:
		inverter_set_3phase_voltages(inverter, 0.0f, 0.0f, 0.0f);
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
			TRAN_STATE(obj, motor_running_state);
		}
	} break;

	case EXIT:
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
		WAIT,
	};
	const struct device *motor = obj->p1;
	struct motor_data *m_data = motor->data;
	struct motor_config *m_cfg = motor->config;
	struct feedback_t *feedback = m_cfg->feedback;
	struct currsmp_t *currsmp = m_cfg->currsmp;
	struct inverter_t *inverter = m_cfg->inverter;

	struct foc_data *f_data = &(m_data->foc_data);
	struct foc_pid *d_pi = &f_data->controller.id;
	struct foc_pid *q_pi = &f_data->controller.iq;
	struct foc_pid *velocity_pi = &f_data->controller.velocity;
	struct foc_pid *pos_pi = &f_data->controller.postion;
	struct device *scp = m_data->scp;
	update_feedback(feedback, PWM_CYCLE);
	switch (obj->phase) {
	case ENTER:
		if (feedback_init(feedback)) {
			break;
		}
		s_planner_init(scp, read_feedback_odome(feedback) / 1000.0f, 0.0f, 0.0f, 0.0f);
		foc_pid_init(d_pi, 0.01f, 30.0f, 13.0f);
		foc_pid_init(q_pi, 0.01f, 30.0f, 13.0f);

#if 0
		float kp, ki;
		kp = f_data->controller.velocity.params->kp;
		ki = f_data->controller.velocity.params->ki;
		foc_pid_init(velocity_pi, kp, ki, 10.0f);

		kp = f_data->controller.postion.params->kp;
		ki = f_data->controller.postion.params->ki;
		foc_pid_init(pos_pi, kp, ki, 5000.0f);
#else
		foc_pid_init(velocity_pi, 0.0008f, 0.004, 10.0f);
		foc_pid_init(pos_pi, 10.0f, 1.0f, 5000.0f); // 参数还需优化
#endif
		obj->count = 0;
		obj->phase = RUNING;
		break;
	case RUNING: {
		float elec_angle = read_feedback_elec_angle(feedback);

		float i_abc[3];
		currsmp_update_currents(currsmp, i_abc);
		foc_update_current_idq(f_data, i_abc, elec_angle);
		float id, iq;
		id = f_data->meas.id;
		iq = f_data->meas.iq;

		// 步骤 1: 动态获取母线电压
		float v_bus = get_currsmp_vbus(currsmp);

		// 留一点裕量 (Margin)，比如 0.95，防止MOS管死区时间和采样噪声导致削顶
		float v_max_abs = v_bus * 0.57735f * 0.96f; // 0.57735 = 1/sqrt(3)
		float v_max_sq = SQ(v_max_abs);
		float pos, vel;
		float pos_loop_out, vel_in;
		static float test_pos, test_vel;
		(void)test_pos;
		(void)test_vel;

		if (((obj->count) % POSITION_LOOP_DIVIDER == 0)) {
			s_planner_action(scp, 0.01f);
			pos = s_planner_get_pos(scp) * 1000.0f;
			vel = s_planner_get_vel(scp) * 1000.0f;
			test_pos = pos;
			test_vel = vel;
			pos_loop_out = foc_pid_run(pos_pi, pos, read_feedback_odome(feedback),
						   POS_LOOP_CYCLE);
			vel_in = pos_loop_out + vel;
			f_data->ref.velocity = vel_in;
		}
		if (((obj->count) % VELOCITY_LOOP_DIVIDER == 0)) {
			f_data->meas.velocity = read_feedback_velocity(feedback, SPEED_LOOP_CYCLE);
			f_data->ref.id = 0.0f;
			f_data->ref.iq = foc_pid_run(velocity_pi, f_data->ref.velocity,
						     f_data->meas.velocity, SPEED_LOOP_CYCLE);
		}
		obj->count++;

		float ud_req, uq_req;
		ud_req = foc_pid_run(d_pi, f_data->ref.id, id, PWM_CYCLE);
		uq_req = foc_pid_run(q_pi, f_data->ref.iq, iq, PWM_CYCLE);

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
		foc_pid_saturation_feedback(d_pi, ud_final, ud_req);
		foc_pid_saturation_feedback(q_pi, uq_final, uq_req);

		// 归一化处理
		ud_final *= (1 / (v_bus * 0.57735f));
		uq_final *= (1 / (v_bus * 0.57735f));
		foc_apply_voltage_dq(inverter, ud_final, uq_final, elec_angle);
	} break;

	case EXIT:
		foc_pid_reset(d_pi);
		foc_pid_reset(q_pi);
		foc_pid_reset(velocity_pi);
		foc_pid_reset(pos_pi);

		inverter_set_3phase_voltages(inverter, 0.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}

	return 0;
}
#if 0
#define DEBUG_D_PI     0
#define DEBUG_Q_PI     1
#define DEBUG_VEL_PI   2
#define DEBUG_POS_MODE 3

#define CURRENT_DEBUG DEBUG_POS_MODE
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
	struct feedback_t *feedback = m_cfg->feedback;
	struct currsmp_t *currsmp = m_cfg->currsmp;
	struct inverter_t *inverter = m_cfg->inverter;

	struct foc_data *f_data = &(m_data->foc_data);
	struct foc_pid *d_pi = &f_data->controller.id;
	struct foc_pid *q_pi = &f_data->controller.iq;
	struct foc_pid *velocity_pi = &f_data->controller.velocity;
	struct foc_pid *pos_pi = &f_data->controller.postion;
	struct device *scp = m_data->scp;

	switch (obj->phase) {
	case ENTER:
		if (feedback_init(feedback)) {
			break;
		}
		s_planner_init(scp, read_feedback_odome(feedback) / 1000.0f, 0.0f, 0.0f, 0.0f);
		float kp, ki;
#if (CURRENT_DEBUG == DEBUG_D_PI)
		read_foc_data(f_data, INDEX_D_PI, data);
		kp = data[0];
		ki = data[1];
		foc_pid_init(&f_data->id_pi_control, kp, ki, 13.0f);
#elif (CURRENT_DEBUG == DEBUG_Q_PI)
		foc_pid_init(&f_data->id_pi_control, kp, ki, 13.0f);
#elif (CURRENT_DEBUG == DEBUG_VEL_PI)
		kp = f_data->controller.velocity.params->kp;
		ki = f_data->controller.velocity.params->ki;

		foc_pid_init(d_pi, 0.01f, 30.0f, 13.0f);
		foc_pid_init(q_pi, 0.01f, 30.0f, 13.0f);
#if 1
		foc_pid_init(velocity_pi, kp, ki, 10.0f); // 0.08f,3.0f
#else
		foc_pid_init(velocity_pi, 0.06f, 3.0f, 10.0f); // 0.08f,3.0f
#endif

#elif (CURRENT_DEBUG == DEBUG_POS_MODE)
		foc_pid_init(d_pi, 0.01f, 30.0f, 13.0f);
		foc_pid_init(q_pi, 0.01f, 30.0f, 13.0f);
		foc_pid_init(velocity_pi, f_data->controller.velocity.params->kp,
			     f_data->controller.velocity.params->ki, 10.0f);
		foc_pid_init(pos_pi, f_data->controller.postion.params->kp,
			     f_data->controller.postion.params->ki, 5000.0f);
#else
#endif
		obj->count = 0;
		obj->phase = WAIT;
		break;
	case WAIT: // TODO 禁止删除
		update_feedback(feedback, PWM_CYCLE);
		if (++obj->count > 1000) {

			obj->count = 0;
			obj->phase = RUNING;
		}
		break;
	case RUNING: {
		update_feedback(feedback, PWM_CYCLE);
		float elec_angle = read_feedback_elec_angle(feedback);

		float i_abc[3];
		currsmp_update_currents(currsmp, i_abc);
		foc_update_current_idq(f_data, i_abc, elec_angle);
		float id, iq;
		id = f_data->meas.id;
		iq = f_data->meas.iq;

		// 步骤 1: 动态获取母线电压
		float v_bus = get_currsmp_vbus(currsmp);

		// 留一点裕量 (Margin)，比如 0.95，防止MOS管死区时间和采样噪声导致削顶
		float v_max_abs = v_bus * 0.57735f * 0.96f; // 0.57735 = 1/sqrt(3)
		float v_max_sq = SQ(v_max_abs);
#if (CURRENT_DEBUG == DEBUG_VEL_PI)
		s_planner_action(scp, SPEED_LOOP_CYCLE);
		f_data->ref.velocity = s_planner_get_vel(scp);
		// 速度环计算
		if (((++obj->count) * PWM_CYCLE) >= SPEED_LOOP_CYCLE) {
			obj->count = 0;
			f_data->ref.id = 0.0f;
			f_data->ref.iq =
				foc_pid_run(velocity_pi, f_data->ref.velocity,
					    read_feedback_velocity(feedback), SPEED_LOOP_CYCLE);
		}
#elif (CURRENT_DEBUG == DEBUG_POS_MODE)
		float pos, vel;
		float pos_loop_out, vel_in;
		static float test_pos, test_vel;
		if (((obj->count) % POSITION_LOOP_DIVIDER == 0)) {
			s_planner_action(scp, 0.01f);
			pos = s_planner_get_pos(scp) * 1000.0f;
			vel = s_planner_get_vel(scp) * 1000.0f;
			test_pos = pos;
			test_vel = vel;
			pos_loop_out = foc_pid_run(pos_pi, pos, read_feedback_odome(feedback),
						   POS_LOOP_CYCLE);
			vel_in = pos_loop_out + vel;
			f_data->ref.velocity = vel_in;
		}
		if (((obj->count) % VELOCITY_LOOP_DIVIDER == 0)) {
			f_data->meas.velocity = read_feedback_velocity(feedback, SPEED_LOOP_CYCLE);
			f_data->ref.id = 0.0f;
			f_data->ref.iq = foc_pid_run(velocity_pi, f_data->ref.velocity,
						     f_data->meas.velocity, SPEED_LOOP_CYCLE);
		}
		obj->count++;

		float ud_req, uq_req;
		ud_req = foc_pid_run(d_pi, f_data->ref.id, id, PWM_CYCLE);
		uq_req = foc_pid_run(q_pi, f_data->ref.iq, iq, PWM_CYCLE);

#endif
		// 步骤 2: PID 计算
		// float ud_req, uq_req;
#if (CURRENT_DEBUG == DEBUG_D_PI)
		ud_req = foc_pid_run(&(f_data->id_pi_control), f_data->id_ref, id, PWM_CYCLE);
		uq_req = 0.0f;
#elif (CURRENT_DEBUG == DEBUG_Q_PI)
		ud_req = 0.0f;
		uq_req = foc_pid_run(&(f_data->iq_pi_control), f_data->iq_ref, iq, PWM_CYCLE);
#elif (CURRENT_DEBUG == DEBUG_VEL_PI)
		ud_req = foc_pid_run(d_pi, f_data->ref.id, id, PWM_CYCLE);
		uq_req = foc_pid_run(q_pi, f_data->ref.iq, iq, PWM_CYCLE);
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
		foc_pid_saturation_feedback(d_pi, ud_final, ud_req);
		foc_pid_saturation_feedback(q_pi, uq_final, uq_req);

		// // 步骤 6: 坐标变换与输出
#if (CURRENT_DEBUG == DEBUG_D_PI)
		// ud_final = 0.0f;
		uq_final = 0.0f;
#elif (CURRENT_DEBUG == DEBUG_Q_PI)
		ud_final = 0.0f;
		// uq_final = 0.0f;
#else
#endif
		// 归一化处理
		ud_final *= (1 / (v_bus * 0.57735f));
		uq_final *= (1 / (v_bus * 0.57735f));
		foc_apply_voltage_dq(inverter, ud_final, uq_final, elec_angle);
	} break;

	case EXIT:
		foc_pid_reset(d_pi);
		foc_pid_reset(q_pi);
		foc_pid_reset(velocity_pi);
		inverter_set_3phase_voltages(inverter, 0.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}

	return 0;
}
#endif