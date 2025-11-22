#ifndef FOC_PARAMETERS_H
#define FOC_PARAMETERS_H
#include "foc_pid.h"
struct foc_parameters {
	float id;
	float iq;

	float id_ref;
	float iq_ref;       // 速度环输出
	float velocity_ref; // 位置环输出

	float position_tar; // 目标位置
	float velocity_tar;
	float torque_tar;
	struct foc_pid id_pi_control;
	struct foc_pid iq_pi_control;
};
void update_focparam_idq(struct foc_parameters *foc_param, float i_alpha, float i_beta,
			 float eangle);
void read_focparam_idq(struct foc_parameters *foc_param, float *id, float *iq);
void foc_pid_saturation_feedback(struct foc_pid *pid, float output_real, float output_desire);
void debug_update_iq_ref(struct foc_parameters *foc_param, float iq_ref);

#endif
