/* openloop_voltage.h */
#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct device;

/* 开环模式 */
typedef enum {
	OPENLOOP_MODE_HOLD_ANGLE = 0, /* 固定电角度施压 */
	OPENLOOP_MODE_CONST_SPEED,    /* 匀速旋转 */
} openloop_mode_t;

/* Open-loop 电压控制器（集成逆变器） */
typedef struct {
	/* ===== 配置参数 ===== */
	float voltage;    /* uq 电压幅值（SVPWM 归一化 0~1） */
	float elec_speed; /* 电角速度 rad/s（仅 CONST_SPEED 有效） */
	float elec_angle; /* 当前电角度 rad */
	openloop_mode_t mode;

	/* ===== 依赖 ===== */
	struct device *inverter;

	/* ===== 运行状态 ===== */
	bool enabled;
} openloop_voltage_t;

/* 初始化 */
void openloop_voltage_init(openloop_voltage_t *ol, struct device *inverter);

/* 使能 / 失能 */
void openloop_voltage_enable(openloop_voltage_t *ol);
void openloop_voltage_disable(openloop_voltage_t *ol);

/* 参数设置 */
void openloop_voltage_set_mode(openloop_voltage_t *ol, openloop_mode_t mode);
void openloop_voltage_set_voltage(openloop_voltage_t *ol, float voltage);
void openloop_voltage_set_speed(openloop_voltage_t *ol, float elec_speed);
void openloop_voltage_set_angle(openloop_voltage_t *ol, float elec_angle);

/* 周期更新（每个控制周期调用） */
void openloop_voltage_update(openloop_voltage_t *ol, float dt);

/* 获取当前电角度 */
float openloop_voltage_get_angle(const openloop_voltage_t *ol);

#ifdef __cplusplus
}
#endif
