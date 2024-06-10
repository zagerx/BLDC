#ifndef ___COMMON___H
#define ___COMMON___H
#include "./motorctrl_cfg.h"

#define PI               3.14159260f
#define _2PI             6.2831852f
#define sqrt3            1.73205f
#define sqrt3_2          0.866025f
#define Q15_PI_2                   (51471)
#define Q15_2PI                    (205884)
#define PI_2                       (1.570796f)





#pragma pack(push,4)
typedef struct _mc_param
{
    pid_cb_t daxis_pi;
    pid_cb_t qaxis_pi;
    pid_cb_t speedloop_pi;
    lowfilter_t elefitler[3];
    lowfilter_t speedfilter;
}mc_param_t;

typedef struct _motor
{

    mc_param_t ctrlparam;

    float pairs;
    float torqueconst;

    void (*enable)(void);
    void (*disable)(void);
}mt_t;

typedef struct
{
    float ia_real;
    float ib_real;
    float ic_real;

    float id_real;
    float id_targe;

    float iq_real;
    float iq_targe;

    float speed_targe;
    float real_speed;

    float vbus;


    float Pid_q_kp;
    float pid_q_ki;
    float pid_D_out;
    float pid_d_kp;
    float pid_d_ki;
    float ele_angle;
    float self_ele_theta;
    int32_t motor_stat;
    char *cur_cmd;
}motordebug_t;



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
    int32_t Q_d;
    int32_t Q_q;
}dq_t;

typedef struct _alphabeta
{
    /* data */
    float alpha;
    float beta;
    int32_t Q_alpha;
    int32_t Q_beta;
}alpbet_t;

typedef struct _abc
{
    /* data */
    float a;
    float b;
    float c;

    int32_t Q_a;
    int32_t Q_b;
    int32_t Q_c;
}abc_t;

typedef struct curloop
{
    pid_cb_t d_pid;
    pid_cb_t q_pid;
}curloop_t;
#pragma pack(pop)

extern motordebug_t motordebug;

float _normalize_angle(float angle);
duty_t _svpwm(float ualpha,float ubeta);
duty_t _svpwm_Q(int32_t ualpha,int32_t ubeta);
alpbet_t _2r_2s(dq_t i_dq,float theta);
void _2s_2r(alpbet_t i_alphabeta,float theta,dq_t *dq);
void _3s_2s(abc_t i_abc,alpbet_t *alp_bet);

alpbet_t _2r_2s_Q(dq_t i_dq,int32_t theta);
void _2s_2r_Q(alpbet_t i_alphabeta,int32_t theta,dq_t *dq);
void _3s_2s_Q(abc_t i_abc,alpbet_t *alp_bet);

duty_t SVM(float alpha, float beta) ;

#endif
