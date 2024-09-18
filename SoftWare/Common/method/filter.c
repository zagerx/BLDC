#include "filter.h"
void lowfilter_init(lowfilter_t *pfilter,float freq)
{
    if (!pfilter)
    {
        return;
    }
    
    pfilter->freq = 1.0f/freq;
    pfilter->pre_val = 0.0f;
}

float lowfilter_cale(lowfilter_t *pfilter,float val)
{
    if (!pfilter)
    {
        return val;
    }
    
    float curval = 0.0f;
    curval =   pfilter->freq * val  + (1.0f-pfilter->freq) * pfilter->pre_val ;
    pfilter->pre_val = curval;
    return curval;
}

// 初始化卡尔曼滤波器
void kalman_filter_init(KalmanFilter *kf, float initial_x, float initial_P, float process_noise, float measurement_noise)
{
    kf->x = initial_x;
    kf->P = initial_P;
    kf->Q = process_noise;
    kf->R = measurement_noise;
}

// 卡尔曼滤波器更新步骤
void kalman_filter_step(KalmanFilter *kf, float measurement) 
{
    // 预测步骤
    kf->P += kf->Q; 
    // 计算卡尔曼增益
    float K = kf->P / (kf->P + kf->R);
    // 更新状态估计值
    kf->x += K * (measurement - kf->x);
    // 更新估计误差的方差
    kf->P *= (1 - K);
}


