#ifndef MOTOR_PARAMS_API_H
#define MOTOR_PARAMS_API_H

#include <stdint.h>
#include <stdbool.h>
#include "motor.h"
struct motor_parameters;

// PI 控制器参数更新
void _update_d_axies_pi_params(struct motor_parameters *m_params, float kp, float ki);
void _update_q_axies_pi_params(struct motor_parameters *m_params, float kp, float ki);
void _update_vel_pi_params(struct motor_parameters *m_params, float kp, float ki);
void _update_pos_pi_params(struct motor_parameters *m_params, float kp, float ki);

// 反馈参数更新
void _update_feedback_dir_params(struct motor_parameters *m_params, int16_t dir);
void _update_feedback_pairs_params(struct motor_parameters *m_params, float pairs);
void _update_feedback_offset_params(struct motor_parameters *m_params, float offset);
void _update_feedback_cpr_params(struct motor_parameters *m_params, float cpr);

// 电流采样参数更新
void _update_currsmp_offset_params(struct motor_parameters *m_params,
                                   uint16_t offset_a,
                                   uint16_t offset_b,
                                   uint16_t offset_c);

#endif // MOTOR_PARAMS_API_H