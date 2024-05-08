#ifndef __FILTER__H
#define __FILTER__H

/*----------------------?????----------------------------------*/
typedef struct lowfilter
{
    /* data */
    float freq;//
    float pre_val;
}lowfilter_t;
void lowfilter_init(lowfilter_t *pfilter,float freq);
float lowfilter_cale(lowfilter_t *pfilter,float val);


// 定义卡尔曼滤波器结构
typedef struct {
    float x;  // 状态估计值
    float P;  // 估计误差的方差
    float Q;  // 过程噪声的方差
    float R;  // 观测噪声的方差
} KalmanFilter;

void kalman_filter_init(KalmanFilter *kf, float initial_x, float initial_P, float process_noise, float measurement_noise);
void kalman_filter_step(KalmanFilter *kf, float measurement);


#endif
