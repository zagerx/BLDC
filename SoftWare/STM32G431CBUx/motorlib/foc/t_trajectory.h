#ifndef T_TRAJECTORY_H
#define T_TRAJECTORY_H

#include "device.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	TRAJ_EXEC_IDLE = 0,
	TRAJ_EXEC_RUNNING,
	TRAJ_EXEC_END,
} trajectory_actor_state_t;

typedef struct {
	float t; /* 本段持续时间 (s) */
	float a; /* 加速度 */
} t_traj_seg_t;

typedef struct {
	/* ===== 执行状态 ===== */
	int cur_seg;    /* 当前段索引 */
	int seg_cnt;    /* 段总数 */
	float seg_time; /* 当前段已运行时间 */

	/* ===== 物理状态（连续量）===== */
	float pos; /* 当前位置 (m) */
	float vel; /* 当前速度 (m/s) */
	float acc; /* 当前加速度 (m/s^2) */

	/* ===== 状态机 ===== */
	trajectory_actor_state_t state; /* IDLE / RUNNING / END */

	/* ===== 轨迹段指针 ===== */
	const t_traj_seg_t *segs;

} t_traj_exec_data_t;

typedef enum {
	TRAJ_PLAN_OK = 0,         // 规划成功
	TRAJ_PLAN_NOT_NEEDED,     // 无需规划
	TRAJ_PLAN_ERR_PARAM,      // 参数错误
	TRAJ_PLAN_ERR_NO_SOLUTION // 轨迹不可达
} t_traj_plan_status_t;

typedef struct {
	float p0; // 起始位置
	float v0; // 起始速度
	float a0; // 起始加速度

	float target_pos; // 目标位置

	float v_max; // 最大速度
	float a_max; // 最大加速度
	float t_min; // 最小时间片
} t_traj_plan_input_t;

typedef struct {
	t_traj_seg_t segs[7]; // 最多7段轨迹
	int seg_cnt;
} t_traj_plan_output_t;

typedef struct {
	t_traj_plan_input_t plan_in;
	t_traj_plan_output_t plan_out;
	float pre_pos;  // 上一次的目标位置
	bool plan_done; // 规划器的状态
} t_traj_plan_data_t;

typedef struct {
	t_traj_plan_data_t plan_data;
	t_traj_exec_data_t exec_data;
	float test_delat_pos;
} t_tarj_data_t;

typedef struct {
	float acc;  // 最大加速度
	float vmax; // 最大速度
} t_tarj_config_t;
struct t_tarjectory_t {
	t_tarj_config_t *config;
	t_tarj_data_t *data;
};

/* ================= API ================= */
int s_planner_init(struct device *t_tarj, float start_pos, float start_vel, float start_acc,
		   float exex_cycle);
void s_planner_action(struct device *t_tarj, const float dt);
t_traj_plan_status_t s_planner_update_target(struct device *t_tarj, float new_target_pos);

float s_planner_get_pos(const struct device *t_tarj);
float s_planner_get_vel(const struct device *t_tarj);
float s_planner_get_acc(const struct device *t_tarj);

#endif
