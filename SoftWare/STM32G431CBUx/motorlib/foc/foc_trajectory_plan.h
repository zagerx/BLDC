#ifndef FOC_TRAJECTORY_PLAN_H
#define FOC_TRAJECTORY_PLAN_H

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
	float j; /* 本段 jerk (m/s^3) */
} s_traj_seg_t;

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
	const s_traj_seg_t *segs;

} s_traj_exec_data_t;

typedef enum {
	TRAJ_PLAN_OK = 0,         // 规划成功
	TRAJ_PLAN_NOT_NEEDED,     // 无需规划
	TRAJ_PLAN_ERR_PARAM,      // 参数错误
	TRAJ_PLAN_ERR_NO_SOLUTION // 轨迹不可达
} s_traj_plan_status_t;
typedef enum {
	TRAJ_TYPE_SEVEN_SEGMENT = 0, // 完整七段（有匀速段）
	TRAJ_TYPE_FIVE_SEGMENT,      // 五段（无匀速，有加加速度段）
	TRAJ_TYPE_THREE_SEGMENT,     // 三段（无匀速，简单加速减速）
	TRAJ_TYPE_INVALID            //
} trajectory_type_t;
typedef struct {
	float p0;
	float v0;
	float a0;

	float p1;
	float v1;
	float a1;

	float v_max;
	float a_max;
	float j_max;
	float t_min;
} s_traj_plan_input_t;

typedef struct {
	s_traj_seg_t segs[7];
	int seg_cnt;
} s_traj_plan_output_t;

typedef struct {
	s_traj_plan_input_t plan_in;
	s_traj_plan_output_t plan_out;
	bool plan_done; // 规划器的状态
	float target_pos;
} s_traj_plan_data_t;

typedef struct {
	s_traj_plan_data_t plan_data;
	s_traj_exec_data_t exec_data;
} s_tarj_data_t;

typedef struct {
	float acc;
	float vmax;
	float jerk;
} s_tarj_config_t;

/* ================= API ================= */

int s_planner_init(struct device *dev, float start_pos, float start_vel, float start_acc,
		   float exex_cycle);
void s_planner_action(struct device *dev, const float dt);
void s_planner_test_plan(struct device *dev);
s_traj_plan_status_t s_planner_update_target(struct device *dev, float new_target_pos);

float s_planner_get_pos(const struct device *dev);
float s_planner_get_vel(const struct device *dev);
float s_planner_get_acc(const struct device *dev);

#endif /* FOC_TRAJECTORY_PLAN_H */
