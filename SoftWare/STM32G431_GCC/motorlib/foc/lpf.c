#include "lpf.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

void lpf_init(struct first_order_lpf *filter, float cutoff_freq, float sample_freq)
{
    if (filter == NULL) return;
    
    if (cutoff_freq <= 0.0f || sample_freq <= 0.0f) {
        filter->alpha = 1.0f; 
        filter->last_output = 0.0f;
        return;
    }
    
    // 计算alpha系数
    // 使用公式: alpha = 1 - exp(-2 * π * fc / fs)
    float alpha = 1.0f - expf(-2.0f * M_PI * cutoff_freq / sample_freq);
    
    // 限制alpha在合理范围内
    if (alpha > 1.0f) alpha = 1.0f;
    if (alpha < 0.001f) alpha = 0.001f;
    
    filter->alpha = alpha;
    filter->last_output = 0.0f; 
}

float lpf_calc(struct first_order_lpf *filter, float input)
{
    if (filter == NULL) return input;
    
    float output = filter->alpha * input + (1.0f - filter->alpha) * filter->last_output;
    
    filter->last_output = output;
    
    return output;
}

void lpf_reset(struct first_order_lpf *filter, float value)
{
    if (filter == NULL) return;
    filter->last_output = value;
}