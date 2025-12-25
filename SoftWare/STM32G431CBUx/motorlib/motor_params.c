#include "motor.h"
#include <stdbool.h>
void _update_d_axies_pi_params(struct motor_parameters *m_params, float kp, float ki)
{
	if (m_params == NULL) {
		return;
	}
	m_params->d_pi_params.kp = kp;
	m_params->d_pi_params.ki = ki;
}

void _update_q_axies_pi_params(struct motor_parameters *m_params, float kp, float ki)
{
	if (m_params == NULL) {
		return;
	}
	m_params->q_pi_params.kp = kp;
	m_params->q_pi_params.ki = ki;
}

void _update_feedback_dir_params(struct motor_parameters *m_params, int16_t dir)
{
	if (m_params == NULL) {
		return;
	}
	m_params->feedback_params.direction = dir;
}
void _update_feedback_pairs_params(struct motor_parameters *m_params, float pairs)
{
	if (m_params == NULL) {
		return;
	}
	m_params->feedback_params.pole_pairs = pairs;
}
void _update_feedback_offset_params(struct motor_parameters *m_params, float offset)
{
	if (m_params == NULL) {
		return;
	}
	m_params->feedback_params.offset = offset;
}
void _update_feedback_cpr_params(struct motor_parameters *m_params, float cpr)
{
	if (m_params == NULL) {
		return;
	}
	m_params->feedback_params.cpr = cpr;
}
void _update_currsmp_offset_params(struct motor_parameters *m_params, uint16_t offset_a,
				   uint16_t offset_b, uint16_t offset_c)
{
	if (m_params == NULL) {
		return;
	}
	m_params->currsmp_params.offset_a = offset_a;
	m_params->currsmp_params.offset_b = offset_b;
	m_params->currsmp_params.offset_c = offset_c;
}
