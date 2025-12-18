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
#include "foc.h"
fsm_rt_t motor_lut_test_state(fsm_cb_t *obj)
{
	enum {
		TEST_ENTER = USER_STATUS,
		TEST_INIT_PARAMS,
		TEST_DRIVE_TO_POINT,
		TEST_WAIT_STABLE,
		TEST_SAMPLE_ERROR,
		TEST_NEXT_POINT,
		TEST_NEXT_CYCLE,
		TEST_CALC_AVERAGE,
		TEST_COMPLETE,
		TEST_EXIT
	};

	const struct device *motor = obj->p1;
	struct motor_config *m_cfg = motor->config;
	struct device *feedback = m_cfg->feedback;
	struct device *inverter = m_cfg->inverter;
	struct feedback_config *fb_cfg = feedback->config;

	// --- 静态变量（测试控制与数据）---
	static uint8_t s_cycle_cnt;      // 已完成循环次数 (0~5)
	static uint16_t s_point_idx;     // 当前采样点索引 (0~POINTS_TOTAL-1)
	static float s_target_angle_rad; // 当前目标电角度 (弧度)
	static float s_timer;            // 通用计时器

// --- 测试参数 (可根据需要调整) ---
// 总循环次数
#define CYCLES_TOTAL     5
// 总采样点数: 3圈 * 64点/圈 = 192点
#define POINTS_PER_CYCLE 64 * 3
	const float STABLE_TIME = 0.1f; // 驱动后稳定等待时间 (秒)
	const float VOLTAGE = 0.08f;    // 开环驱动电压

// 角度常数
#define TWO_PI    (2.0f * M_PI)
#define THREE_REV (3.0f * TWO_PI)
	// const float  = 180.0f / M_PI; // 弧度转度

	// --- 误差数据存储 (累加法) ---
	static float s_error_sum[POINTS_PER_CYCLE];     // 误差累加和数组 (弧度)
	static float s_avg_error[POINTS_PER_CYCLE];     // 最终平均误差数组 (弧度)
	static float s_cmd_angle_rad[POINTS_PER_CYCLE]; // 每个采样点对应的指令电角度 (弧度)

	static float test_error;
	update_feedback(feedback, PWM_CYCLE); // PWM_CYCLE = 0.0001F

	switch (obj->phase) {
	case ENTER:
		// 1. 初始化所有控制变量
		s_cycle_cnt = 0;
		s_point_idx = 0;
		s_timer = 0.0f;

		// 2. 清零误差累加和数组
		for (int i = 0; i < POINTS_PER_CYCLE; i++) {
			s_error_sum[i] = 0.0f;
			s_avg_error[i] = 0.0f;
			s_cmd_angle_rad[i] = 0.0f;
		}

		// 3. 初始化反馈模块
		feedback_init(feedback);
		obj->phase = TEST_INIT_PARAMS;
		break;

	case TEST_INIT_PARAMS:
		// 计算当前目标电角度 (弧度)
		// 总机械角度 = (当前点索引 / 总点数) * (3圈 * 2π)
		// 目标电角度 = 总机械角度 * 极对数
		{
			float total_mech_angle_rad =
				((float)s_point_idx / (float)POINTS_PER_CYCLE) * THREE_REV;
			s_target_angle_rad = total_mech_angle_rad * (float)fb_cfg->pole_pairs;

			// 存储指令角度（仅在第一轮记录一次）
			if (s_cycle_cnt == 0) {
				s_cmd_angle_rad[s_point_idx] = s_target_angle_rad;
			}
		}
		s_timer = 0.0f;
		obj->phase = TEST_DRIVE_TO_POINT;
		break;

	case TEST_DRIVE_TO_POINT:
		// 开环驱动到目标电角度，需要将弧度转换为度
		_open_loop_voltage_vector_drive(inverter, s_target_angle_rad * RAD_TO_DEG, VOLTAGE);
		obj->phase = TEST_WAIT_STABLE;
		break;

	case TEST_WAIT_STABLE:
		// 等待转子稳定
		s_timer += PWM_CYCLE;
		if (s_timer >= STABLE_TIME) {
			s_timer = 0.0f;
			obj->phase = TEST_SAMPLE_ERROR;
		}
		break;

	case TEST_SAMPLE_ERROR:
		// 采样一次误差：误差 = 反馈电角度 - 指令电角度 (全部使用弧度)
		{
			float feedback_elec_angle_rad = read_feedback_elec_angle(feedback);

			// 计算角度误差（弧度），并确保在±π范围内（最短路径）
			float error_rad = feedback_elec_angle_rad - s_target_angle_rad;

			// 将误差归一化到[-π, π)区间
			if (error_rad > M_PI) {
				error_rad -= TWO_PI;
			} else if (error_rad < -M_PI) {
				error_rad += TWO_PI;
			}

			// 累加误差
			s_error_sum[s_point_idx] += error_rad;
		}
		obj->phase = TEST_NEXT_POINT;
		break;

	case TEST_NEXT_POINT:
		// 移动到下一个采样点
		s_point_idx++;

		if (s_point_idx >= POINTS_PER_CYCLE) {
			// 已完成一个完整循环（3圈）
			s_point_idx = 0;
			obj->phase = TEST_NEXT_CYCLE;
		} else {
			// 继续当前循环的下一个点
			obj->phase = TEST_INIT_PARAMS;
		}
		break;

	case TEST_NEXT_CYCLE:
		// 准备下一个循环
		s_cycle_cnt++;
		if (s_cycle_cnt >= CYCLES_TOTAL) {
			// 所有循环完成，进入结果计算
			obj->phase = TEST_CALC_AVERAGE;
		} else {
			// 开始新一轮循环
			obj->phase = TEST_INIT_PARAMS;
		}
		break;

	case TEST_CALC_AVERAGE:
		// 计算平均误差 (弧度)
		for (int i = 0; i < POINTS_PER_CYCLE; i++) {
			s_avg_error[i] = s_error_sum[i] / (float)CYCLES_TOTAL;
			test_error = s_avg_error[i];
		}

		// --- 可选的：添加结果分析代码 ---
		// 例如：计算整体误差的峰峰值、标准差等
		// 将 s_avg_error 数组复制到全局变量供J-Scope观察

		obj->phase = TEST_COMPLETE;
		break;

	case TEST_COMPLETE:
		// 停止驱动，测试结束
		inverter_set_3phase_voltages(inverter, 0.0f, 0.0f, 0.0f);
		obj->phase = TEST_EXIT;
		break;

	case TEST_EXIT:
		return 0; // 返回成功状态

	default:
		break;
	}

	return 0; // 返回进行中状态
}