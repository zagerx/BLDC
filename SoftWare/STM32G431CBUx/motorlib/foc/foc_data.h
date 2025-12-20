#ifndef FOC_PARAMETERS_H
#define FOC_PARAMETERS_H
#include "foc_pid.h"
enum foc_data_index {
	INDEX_ID_REF = 0,
	INDEX_IQ_REF,
	INDEX_VELOCITY_REG,

	INDEX_POSITION_TAR,
	INDEX_D_PI,
	INDEX_Q_PI,
	INDEX_VELOCITY_PI,
};
struct foc_data {
	// Only Read
	float id;
	float iq;

	// RW
	float id_ref;
	float iq_ref;       // 速度环输出
	float velocity_ref; // 位置环输出

	float position_tar; // 目标位置
	float velocity_tar;
	float torque_tar;
	struct foc_pid id_pi_control;
	struct foc_pid iq_pi_control;
	struct foc_pid velocity_pi_control;
};
void update_focparam_idq(struct foc_data *f_data, float i_alpha, float i_beta, float eangle);
void read_focparam_idq(struct foc_data *f_data, float *id, float *iq);

void read_foc_data(struct foc_data *f_data, enum foc_data_index flag, float *data);
void write_foc_param_(struct foc_data *f_data, enum foc_data_index flag, float *data);
#endif
