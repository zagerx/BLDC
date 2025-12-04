#include "resistance_inductance_calib.h"
#include "device.h"
#include "inverter.h"
#include "coord_transform.h"
#include <math.h>
#include <string.h>

/* ---------------------------------------------------------
 * 启动电阻电感校准
 * --------------------------------------------------------- */
void motor_param_calib_start(struct device *motor_param_calib)
{
	if (!motor_param_calib || !motor_param_calib->data || !motor_param_calib->config) {
		return;
	}

	struct motor_param_calib_data *md = motor_param_calib->data;

	// 初始化状态
	md->state = MOTOR_PARAM_CALIB_STATE_ALIGN;
	md->time_acc = 0.0f;
	md->sample_count = 0;
	md->ac_angle = 0.0f;
	md->ac_time = 0.0f;

	// 设置固定的开环角度（例如0度）
	md->openloop_angle = 0.0f;

	// 清零测量数据
	md->vd_measure = 0.0f;
	md->vq_measure = 0.0f;
	md->id_measure = 0.0f;
	md->iq_measure = 0.0f;
	md->id_sum = 0.0f;
	md->iq_peak = 0.0f;
}

/* ---------------------------------------------------------
 * 主更新函数（开环方式）
 * --------------------------------------------------------- */
int32_t motor_param_calib_update(struct device *motor_param_calib, float dt)
{
	if (!motor_param_calib || !motor_param_calib->data || !motor_param_calib->config) {
		return -1;
	}

	int32_t ret = 0;
	struct motor_param_calib_data *md = motor_param_calib->data;
	struct motor_param_calib_config *cfg = motor_param_calib->config;
	struct device *inv = cfg->inverter;
	struct device *curr_sensor = cfg->current_sensor;

	// 读取三相电流
	float iabc[3];
	currsmp_update_currents(curr_sensor, iabc);

	// 使用固定的开环角度进行坐标变换
	float elec_angle = md->openloop_angle;
	float sin_angle, cos_angle;
	sin_cos_f32(elec_angle * 180.0f / M_PI, &sin_angle, &cos_angle);

	// Clarke变换：三相->两相
	float ialpha = iabc[0];
	float ibeta = (iabc[0] + 2.0f * iabc[1]) * 0.57735026919f;

	// Park变换：两相->DQ轴（使用固定角度）
	float id = ialpha * cos_angle + ibeta * sin_angle;
	float iq = -ialpha * sin_angle + ibeta * cos_angle;

	switch (md->state) {

	/* -----------------------------------------------------
	 * Step 1: 对齐阶段 - 使用固定角度将转子对齐
	 * ----------------------------------------------------- */
	case MOTOR_PARAM_CALIB_STATE_ALIGN: {
		// 施加固定角度的电压矢量（0度）
		float angle_deg = md->openloop_angle * 180.0f / M_PI;
		float s, c;
		sin_cos_f32(angle_deg, &s, &c);

		float abc[3];
		svm_set(cfg->resistance_calib_max_voltage * c,
			cfg->resistance_calib_max_voltage * s, abc);
		inverter_set_3phase_voltages(inv, abc[0], abc[1], abc[2]);

		md->time_acc += dt;
		if (md->time_acc >= cfg->align_duration) {
			md->time_acc = 0.0f;
			md->state = MOTOR_PARAM_CALIB_STATE_MEASURE_R;
		}
	} break;

	/* -----------------------------------------------------
	 * Step 2: 电阻测量 - D轴直流注入（使用固定角度）
	 * ----------------------------------------------------- */
	case MOTOR_PARAM_CALIB_STATE_MEASURE_R: {
		// 在D轴施加直流电压，Q轴电压为0（使用固定角度）
		float vd = cfg->resistance_calib_max_voltage;
		float vq = 0.0f;

		// 反Park变换到两相（使用固定角度）
		float v_alpha = vd * cos_angle - vq * sin_angle;
		float v_beta = vd * sin_angle + vq * cos_angle;

		// 反Clarke变换到三相
		float va = v_alpha;
		float vb = -0.5f * v_alpha + 0.86602540378f * v_beta;
		float vc = -0.5f * v_alpha - 0.86602540378f * v_beta;

		inverter_set_3phase_voltages(inv, va, vb, vc);

		// 累加D轴电流测量
		md->id_sum += id;
		md->sample_count++;

		// 记录测量值
		md->vd_measure = vd;
		md->id_measure = id;

		md->time_acc += dt;
		if (md->time_acc >= cfg->resistance_measure_duration) {
			// 计算平均D轴电流
			float id_avg = md->id_sum / md->sample_count;

			// 计算电阻：R = Vd / Id
			if (fabsf(id_avg) > 0.001f) {
				md->phase_resistance = md->vd_measure / id_avg;
			} else {
				md->state = MOTOR_PARAM_CALIB_STATE_ERROR;
				break;
			}

			// 重置测量状态
			md->time_acc = 0.0f;
			md->sample_count = 0;
			md->id_sum = 0.0f;
			md->state = MOTOR_PARAM_CALIB_STATE_MEASURE_L;
		}
	} break;

	/* -----------------------------------------------------
	 * Step 3: 电感测量 - Q轴交流注入（使用固定角度）
	 * ----------------------------------------------------- */
	case MOTOR_PARAM_CALIB_STATE_MEASURE_L: {
		// 更新交流角度（独立的时间累加）
		md->ac_time += dt;
		md->ac_angle = 2.0f * M_PI * cfg->ac_frequency * md->ac_time;

		// 在Q轴施加交流电压，D轴电压为0（使用固定角度）
		float vd = 0.0f;
		float vq = cfg->inductance_calib_max_voltage * sinf(md->ac_angle);

		// 反Park变换到两相（使用固定角度）
		float v_alpha = vd * cos_angle - vq * sin_angle;
		float v_beta = vd * sin_angle + vq * cos_angle;

		// 反Clarke变换到三相
		float va = v_alpha;
		float vb = -0.5f * v_alpha + 0.86602540378f * v_beta;
		float vc = -0.5f * v_alpha - 0.86602540378f * v_beta;

		inverter_set_3phase_voltages(inv, va, vb, vc);

		// 跟踪Q轴电流峰值
		if (fabsf(iq) > md->iq_peak) {
			md->iq_peak = fabsf(iq);
		}

		// 记录测量值
		md->vq_measure = cfg->inductance_calib_max_voltage;

		md->time_acc += dt;
		if (md->time_acc >= cfg->inductance_measure_duration) {
			// 计算电感：L = Vq_peak / (2 * π * f * Iq_peak)
			if (md->iq_peak > 0.001f) {
				md->phase_inductance =
					md->vq_measure /
					(2.0f * M_PI * cfg->ac_frequency * md->iq_peak);
			} else {
				md->state = MOTOR_PARAM_CALIB_STATE_ERROR;
				break;
			}

			md->state = MOTOR_PARAM_CALIB_STATE_CALCULATE;
		}
	} break;

	/* -----------------------------------------------------
	 * Step 4: 计算结果和合理性检查
	 * ----------------------------------------------------- */
	case MOTOR_PARAM_CALIB_STATE_CALCULATE: {
		// 合理性检查（与ODrive一致）
		if (md->phase_resistance > 0.001f && md->phase_resistance < 100.0f &&
		    md->phase_inductance > 0.000001f && md->phase_inductance < 0.1f) {
			md->state = MOTOR_PARAM_CALIB_STATE_COMPLETE;
		} else {
			md->state = MOTOR_PARAM_CALIB_STATE_ERROR;
		}
	} break;

	case MOTOR_PARAM_CALIB_STATE_COMPLETE:
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
		ret = 1;
		break;

	case MOTOR_PARAM_CALIB_STATE_ERROR:
		ret = -1;
		inverter_set_3phase_voltages(inv, 0.0f, 0.0f, 0.0f);
		break;

	default:
		md->state = MOTOR_PARAM_CALIB_STATE_IDLE;
		break;
	}

	return ret;
}

/* ---------------------------------------------------------
 * 获取校准结果
 * --------------------------------------------------------- */
void motor_param_calib_get_results(struct device *motor_param_calib, float *resistance,
				   float *inductance)
{
	if (!motor_param_calib || !motor_param_calib->data) {
		return;
	}

	struct motor_param_calib_data *md = motor_param_calib->data;

	if (resistance) {
		*resistance = md->phase_resistance;
	}
	if (inductance) {
		*inductance = md->phase_inductance;
	}
}
