#include "t_trajectory.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#define UNUSED(x) ((void)(x))

int s_planner_init(struct device *dev, float start_pos, float start_vel, float start_acc,
		   float exex_cycle)
{
	UNUSED(start_pos);
	UNUSED(start_vel);
	UNUSED(start_acc);
	UNUSED(exex_cycle);
	s_tarj_data_t *d = dev->data;
	s_traj_exec_data_t *exec = &d->exec_data;
	s_traj_plan_data_t *plan = &d->plan_data;
	plan->plan_in.target_pos = 0.0f;
	exec->pos = 0.0f;
	exec->vel = 0.0f;
	return 0;
}
void s_planner_action(struct device *dev, float dt)
{
	UNUSED(dt);
	s_tarj_data_t *d = dev->data;
	s_traj_plan_data_t *plan = &d->plan_data;
	s_traj_exec_data_t *exec = &d->exec_data;
	exec->pos = plan->plan_in.target_pos;
	exec->vel = plan->plan_in.target_pos;

	return;
}

s_traj_plan_status_t s_planner_update_target(struct device *dev, float new_target_pos)
{
	s_tarj_data_t *d = dev->data;
	s_traj_plan_data_t *plan = &d->plan_data;
	plan->plan_in.target_pos = new_target_pos;

	return TRAJ_PLAN_OK;
}

float s_planner_get_pos(const struct device *dev)
{
	s_tarj_data_t *d = dev->data;
	s_traj_exec_data_t *exec = &d->exec_data;
	return exec->pos;
}

float s_planner_get_vel(const struct device *dev)
{
	s_tarj_data_t *d = dev->data;
	s_traj_exec_data_t *exec = &d->exec_data;
	return exec->vel;
}

float s_planner_get_acc(const struct device *dev)
{
	s_tarj_data_t *d = dev->data;
	s_traj_exec_data_t *exec = &d->exec_data;
	return exec->acc;
}
