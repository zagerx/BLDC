#ifndef ___COMMON___H
#define ___COMMON___H



#include "stdint.h"
#include "pid.h"
#include "filter.h"
#include "trajectory_planning.h"

#include "board.h"
#if (ENCODER_TYPE == ENCODER_TYPE_ABS)
    #include "abs_sensor.h"
#elif(ENCODER_TYPE == ENCODER_TYPE_HALL || ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ)
    #include "hall_sensor.h"
#endif

#define PI               3.14159260f
#define _2PI             6.2831852f
#define sqrt3            1.73205f
#define sqrt3_2          0.866025f
#define PI_2             (1.570796f)

#pragma pack(push,1)
typedef struct _motor_romparam
{
  char name[32];
  float fbuf[4];
}motor_Romparam_t;
#pragma pack(pop)



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
    linear_in_t linear;
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
    float i_alpbe[2];
    float i_dq[2];

    float id_tar;
    float iq_tar;

    pid_cb_t d_pid;
    pid_cb_t q_pid;
    smo_t ti_smo;
}mc_currment_t;

typedef struct mc_encoder mc_encoder_t;
struct mc_encoder
{
    /*输出*/
    float speed;     //机械转速
    float ele_theta; //电角度
    float total_realmectheta;
    float self_theta;//调试使用
    lowfilter_t speedfilter;
    float self_te;
    uint8_t runflag;
    /*输入*/
#if (ENCODER_TYPE == ENCODER_TYPE_ABS)
    abs_sensor_t* sensor;
#elif(ENCODER_TYPE == ENCODER_TYPE_HALL || ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ)
    hall_sensor_t* sensor;
#endif    
};

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

    float pid_debug_target;
}mc_debug_t;


typedef struct _motor
{
    mc_pos_t pos_handle;
    mc_speed_t  speed_handle;
    mc_currment_t currment_handle;
    mc_encoder_t encoder_handle;
    mc_debug_t debug;

    int16_t curmode;
    int16_t lastmode;
    int16_t curMotorstate;

    void  (*enable)(void);
    void  (*disable)(void);
    void  (*setpwm)(float,float,float);
    // void  (*reset_system)(void);
    // void  (*bsptransmit)(uint8_t*,uint16_t);
    // void  (*read)(void*, uint16_t);
    // void  (*write)(void*, uint16_t);
}motor_t;

#pragma pack(pop)



#endif
