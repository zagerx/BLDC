#ifndef FOC_PARAMETERS_H
#define FOC_PARAMETERS_H
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
void update_focparam_idq(struct foc_parameters *foc_param, float i_alpha, float i_beta,
			 float eangle);
#endif
