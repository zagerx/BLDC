#ifndef _MOTOR_PARAM_CALIB_H
#define _MOTOR_PARAM_CALIB_H

#include <stdint.h>
#include <stdbool.h>
#include "device.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

enum motor_param_calib_state {
    MOTOR_PARAM_CALIB_STATE_IDLE = 0,
    MOTOR_PARAM_CALIB_STATE_ALIGN,           // 对齐阶段（固定角度）
    MOTOR_PARAM_CALIB_STATE_MEASURE_R,       // 测量电阻 (D轴直流)
    MOTOR_PARAM_CALIB_STATE_MEASURE_L,       // 测量电感 (Q轴交流)
    MOTOR_PARAM_CALIB_STATE_CALCULATE,       // 计算结果
    MOTOR_PARAM_CALIB_STATE_COMPLETE,
    MOTOR_PARAM_CALIB_STATE_ERROR,
};

struct motor_param_calib_config {
    struct device *inverter;           // 逆变器设备
    struct device *current_sensor;     // 电流传感器设备
    
    // 校准参数（与ODrive保持一致）
    float calib_current;              // 校准电流
    float resistance_calib_max_voltage; // 电阻校准最大电压
    float inductance_calib_max_voltage; // 电感校准最大电压
    
    // 时间参数
    float align_duration;             // 对齐时间
    float resistance_measure_duration; // 电阻测量时间
    float inductance_measure_duration; // 电感测量时间
    
    // 电感测量频率
    float ac_frequency;               // 交流频率
};

struct motor_param_calib_data {
    enum motor_param_calib_state state;
    
    float time_acc;
    uint32_t sample_count;
    
    // 开环角度控制
    float openloop_angle;             // 开环电角度（固定值）
    
    // 测量数据
    float vd_measure;                 // D轴测量电压
    float vq_measure;                 // Q轴测量电压
    float id_measure;                 // D轴测量电流
    float iq_measure;                 // Q轴测量电流
    float id_sum;                     // D轴电流累加
    float iq_peak;                    // Q轴电流峰值
    
    // 交流信号生成
    float ac_angle;                   // 交流角度
    float ac_time;                    // 交流时间累加
    
    // 结果
    float phase_resistance;           // 相电阻 (Ohm)
    float phase_inductance;           // 相电感 (H)
};

// 函数声明
void motor_param_calib_start(struct device *motor_param_calib);
int32_t motor_param_calib_update(struct device *motor_param_calib, float dt);
void motor_param_calib_get_results(struct device *motor_param_calib, 
                                  float *resistance, float *inductance);

#endif /* _MOTOR_PARAM_CALIB_H */