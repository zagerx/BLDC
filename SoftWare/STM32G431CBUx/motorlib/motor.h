#ifndef MOTOR_H
#define MOTOR_H
#include "device.h"
#include "foc_pid.h"
#include "motor_pp_ident.h"
#include "statemachine.h"
#include "stdint.h"
/**
 * @brief FOC电机控制状态枚举
 */
enum motor_fault_code {
	MOTOR_FAULTCODE_NOERR = 0,
	MOTOR_FAULTCODE_OVERCURRMENT,
	MOTOR_FAULTCODE_UNDERVOL,
};

enum motor_state {
	MOTOR_STATE_IDLE = 2,    // 空闲状态，未使能  TODO
	MOTOR_STATE_INIT,        // 初始化状态
	MOTOR_STATE_READY,       // 就绪态 已使能，准备进入闭环前的状态
	MOTOR_STATE_ALIGN,       // 电机对齐状态(初始位置校准)
	MOTOR_STATE_CLOSED_LOOP, // 闭环运行状态
	MOTOR_STATE_FAULT,       // 故障状态
	MOTOR_STATE_CALIBRATION, // 校准状态(参数辨识)
	MOTOR_STATE_STOP,        // 受控停止状态
	MOTOR_STATE_EMERGENCY    // 紧急停止状态
};

enum motor_mode {
	MOTOR_MODE_SPEED = 1,
	MOTOR_MODE_POSI,
	MOTOR_MODE_TORQUE,
	MOTOR_MODE_SELFCHECK,
	MOTOR_MODE_IDLE
};

struct motor_config {
	struct device *currsmp;
	struct device *inverter;
	struct device *feedback;

	// struct device *pp_ident;
};
struct motor_parameters {
	float phase_resistance;   // 相电阻 (Ω)
	float phase_inductance_d; // D轴电感 (H)
	float phase_inductance_q; // Q轴电感 (H)

	uint16_t pole_pairs; // 极对数
	float flux_linkage;  // 磁链 (Wb)
};
struct motor_currment_parameters {
	pid_cb_t pi;
};
struct motot_velocity_parameters {
	pid_cb_t pi;
	// 速度规划的相关参数，
};

struct motor_control_parameters {
	// d轴PI参数
	struct motor_currment_parameters d_axis;
	// q轴PI参数
	struct motor_currment_parameters q_axis;
	// 速度环参数
	struct motot_velocity_parameters velocity;
	//...
};

struct foc_parameters {
	// only read
	float ia;
	float ib;
	float ic;
	float id;
	float iq;

	float eangle;
	float velocity;
	float position;

	//
	float id_ref;
	float iq_ref;       // 速度环输出
	float velocity_ref; // 位置环输出

	float position_tar; // 目标位置
	float velocity_tar;
	float torque_tar;
};

enum motor_flag {
	PARAM_NEVER_LOADED = 0, /* 板子首次上电，外部 FLASH 无有效数据 */
	PARAM_LOAD_PENDING,     /* 其它任务正在读 FLASH，数据尚未就绪   */
	PARAM_UPD_YES           /* 参数已成功更新，当前可用             */
};
struct motor_calibration_modules {
	struct device *pp_ident;
	struct device *current_calibration;
	struct device *rl_ident;
};

struct motor_data {
	struct motor_parameters model_paramters;
	struct motor_control_parameters control_paramters;
	struct foc_parameters foc_data;
	struct motor_calibration_modules *calib;
	enum motor_flag flag;
	enum motor_mode mode;
	enum motor_state statue;
	enum motor_fault_code faultcode;
	fsm_cb_t *fsm_mode;
};
void foc_curr_regulator(uint32_t *adc_raw);
#endif
