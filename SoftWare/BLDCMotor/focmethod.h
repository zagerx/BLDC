#ifndef __FOCMETHOD__H
#define __FOCMETHOD__H
#include "./bldcmotor_cfg.h"

#define PI               3.14159260f
#define _2PI             6.2831852f
#define sqrt3            1.73205f
#define sqrt3_2          0.866025f

#pragma pack(push,4)
typedef struct _duty
{
    /* data */
    float _a;
    float _b;
    float _c;
}duty_t;

typedef struct _dq
{
    /* data */
    float d;
    float q;
    float theta;
}dq_t;

typedef struct _alphabeta
{
    /* data */
    float alpha;
    float beta;
    float theta;
}alpbet_t;

typedef struct _abc
{
    /* data */
    float a;
    float b;
    float c;
}abc_t;

/*------����������--------*/
typedef struct curloop
{
    pid_cb_t d_pid;
    pid_cb_t q_pid;
}curloop_t;
#pragma pack(pop)

float _normalize_angle(float angle);
duty_t _svpwm(float ualpha,float ubeta);
duty_t foc_curloopcale(abc_t i_abc,float theta);
void foc_paraminit(void);

duty_t _svpwm_q15(uint64_t ualpha,uint64_t ubeta);
#endif
