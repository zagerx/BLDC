#ifndef MOTOR_CFG_H
#define MOTOR_CFG_H
// #define PWM_FREQ 20000.0f  // 10KHZ
// #define PWM_CYCLE 0.00005f // 100us

// 核心频率配置
#define PWM_FREQUENCY_HZ 20000     // 20 kHz
#define CURRENT_LOOP_FREQ_HZ 20000 // 20 kHz
#define VELOCITY_LOOP_FREQ_HZ 4000 // 4 kHz
#define POSITION_LOOP_FREQ_HZ 1000 // 1 kHz

// 分频系数（基于20kHz PWM）
#define VELOCITY_LOOP_DIVIDER 5  // 20k/5 = 4k
#define POSITION_LOOP_DIVIDER 20 // 20k/20 = 1k

// 对应的周期时间
#define DT_CURRENT 0.00005f  // 50 μs
#define DT_VELOCITY 0.00025f // 250 μs
#define DT_POSITION 0.001f   // 1 ms

// 各环路的预期带宽
#define CURRENT_BANDWIDTH_HZ 1000 // 1 kHz
#define VELOCITY_BANDWIDTH_HZ 150 // 150 Hz
#define POSITION_BANDWIDTH_HZ 30  // 30 Hz

#endif
