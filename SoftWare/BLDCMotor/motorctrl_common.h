#ifndef ___COMMON___H
#define ___COMMON___H



#include "stdint.h"
#include "pid.h"
#include "filter.h"

#ifndef ENCODER_TYPE_HALL    
#else
    #include "hall_sensor.h"    
#endif 

#define PI               3.14159260f
#define _2PI             6.2831852f
#define sqrt3            1.73205f
#define sqrt3_2          0.866025f
#define PI_2             (1.570796f)





#pragma pack(push,4)

typedef struct {  
    float  Valpha;    // Input: Stationary alfa-axis stator voltage 
    float  Vbeta;    // Input: Stationary beta-axis stator voltage 
    float  Ibeta;   // Input: Stationary beta-axis stator current 
    float  Ialpha;   // Input: Stationary alfa-axis stator current 

    float  Theta;     // Output: Compensated rotor angle 
    float  Zalpha;      // Output: Stationary alfa-axis sliding control 
    float  Zbeta;      // Output: Stationary beta-axis sliding control 

    float  Ealpha;   // Variable: Stationary alfa-axis back EMF 
    float  Ebeta;   // Variable: Stationary beta-axis back EMF 
    float  EstIalpha;   // Variable: Estimated stationary alfaaxis stator current 
    float  EstIbeta;    // Variable: Estimated stationary beta-axis statorcurrent 
    float  IalphaError; // Variable: Stationary alfaaxis current error                 
    float  IbetaError;  // Variable: Stationary betaaxis current error                 

    float  E0;

    float  Gsmopos;    // Parameter: Motor dependent control gain 
    float  Fsmopos;    // Parameter: Motor dependent plant matrix 
    float  Kslide;     // Parameter: Sliding control gain 
    float  Kslf;       // Parameter: Sliding control filter gain 
}smo_t; 
typedef struct mc_speed
{
    pid_cb_t pid;
    float tar;
    float real;
}mc_speed_t;
typedef struct mc_pos
{
    pid_cb_t pi_contrl;
    float tar;
    float real;

    float delte_dist;
    float total_dist;
    mc_speed_t *p_speed;   
}mc_pos_t;

typedef struct mc_currment
{
    float i_abc[3];
    float theta;
    float next_theta;
    float id_tar;
    float iq_tar;
    pid_cb_t d_pid;
    pid_cb_t q_pid;
    smo_t ti_smo;
}mc_currment_t;

typedef struct mc_encoder
{
    /*输出*/
    float speed;     //机械转速
    float ele_theta; //电角度
    
    lowfilter_t speedfilter;

    /*输入*/
#ifndef ENCODER_TYPE_HALL    
    int32_t raw_data;
    float mec_theta;
    float total_realmectheta;
    float pre_theta;
#else
    hall_sensor_t hallsensor;
#endif
}mc_encoder_t;


typedef struct _mc_param
{
    mc_pos_t pos_handle;
    mc_speed_t  speed_handle;
    mc_currment_t currment_handle;
    mc_encoder_t encoder_handle;
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
    float speed_real;

    float pos_targe;
    float pos_real;

    float ele_angle;
    float self_ele_theta;

    float vbus;
    float pid_kp;
    float pid_ki;
    float pid_kc;
    float pid_tar;
    float pid_out;
    unsigned short rec_cmd;
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
}dq_t;

typedef struct _alphabeta
{
    /* data */
    float alpha;
    float beta;
}alpbet_t;

typedef struct _abc
{
    /* data */
    float a;
    float b;
    float c;
}abc_t;

typedef struct curloop
{
    pid_cb_t d_pid;
    pid_cb_t q_pid;
}curloop_t;


// typedef struct smo
// {
//     alpbet_t i;
//     alpbet_t u;
// }smo_t;


#pragma pack(pop)

extern motordebug_t motordebug;
extern mc_param_t mc_param;

float _normalize_angle(float angle);
duty_t _svpwm(float ualpha,float ubeta);
alpbet_t _2r_2s(dq_t i_dq,float theta);
void _2s_2r(alpbet_t i_alphabeta,float theta,dq_t *dq);
void _3s_2s(abc_t i_abc,alpbet_t *alp_bet);
duty_t SVM(float alpha, float beta) ;

#endif
