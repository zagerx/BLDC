#ifndef LPF_H
#define LPF_H

#include <stdint.h>

/**
 * @file lpf.h
 * @brief 一阶低通滤波器模块
 * 参数配置参考（基于10kHz采样频率）：
 * - 强滤波（平滑但响应慢）：20Hz截止频率，适合高精度定位应用
 *   lpf_init(&filter, 20.0f, 10000.0f);
 * 
 * - 中等滤波（平衡性能）：50Hz截止频率，适合大多数通用应用
 *   lpf_init(&filter, 50.0f, 10000.0f);
 * 
 * - 弱滤波（响应快但有噪声）：100Hz截止频率，适合需要快速响应的场景
 *   lpf_init(&filter, 100.0f, 10000.0f);
 * 
 * - 接近直通（调试使用）：1kHz截止频率，滤波效果很弱，接近原始信号
 *   lpf_init(&filter, 1000.0f, 10000.0f);
 * 
 * 设计原则：
 * - 截止频率应远小于采样频率（通常fc < fs/10）
 * - 截止频率越高，滤波效果越弱，响应越快
 * - 截止频率越低，滤波效果越强，响应越慢
 */

/**
 * @brief 一阶低通滤波器结构体
 */
struct first_order_lpf {
    float last_output; 
    float alpha;
};

/**
 * @brief 初始化一阶低通滤波器
 * 
 * @param filter 滤波器实例指针
 * @param cutoff_freq 截止频率 (Hz)，决定滤波强度
 * @param sample_freq 采样频率 (Hz)，应与实际调用lpf_calc的频率一致
 * 
 * @note 截止频率与采样频率的关系：
 *       - 截止频率应远小于采样频率（推荐fc < fs/10）
 *       - 截止频率越高，滤波效果越弱，相位延迟越小
 *       - 截止频率越低，滤波效果越强，相位延迟越大
 */
void lpf_init(struct first_order_lpf *filter, float cutoff_freq, float sample_freq);

float lpf_calc(struct first_order_lpf *filter, float input);

void lpf_reset(struct first_order_lpf *filter, float value);

#endif /* LPF_H */