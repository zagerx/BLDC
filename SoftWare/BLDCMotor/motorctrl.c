#include "./motorctrl.h"
#include "debuglog.h"
#include "focmethod.h"
#include "sensor.h"
#define MOTORCTRL_PERCI            (1)

enum{
    MOTOR_INIT,
    MOTOR_DIR1,
    MOTOR_DIR2,
    MOTOT_ANGIL,
    MOTOR_IDLE,
    MOTOR_START, 
    MOTOR_RUNING,
    MOTOR_RUNING2,  
    MOTOT_FALT,
    MOTOR_STOP,
    MOTOR_TEST,
    MOTOR_OFFSET,
};
typedef struct
{
    float ia;
    float ib;
    float ic;
    float i_alpha;
    float i_beta;
    float i_d;
    float i_q;

    float id_real;
    float iq_targe;
    float iq_real;

    float pid_Q_out;
    float pid_D_out;
    float ele_angle;
}motordebug_t;
motordebug_t sg_motordebug;
typedef struct
{
    unsigned char state;
    unsigned short cnt;
}motorctrl_t;
motorctrl_t g_Motor1 = {
    .state = MOTOR_INIT,
    .cnt = 0,
};
static float sg_MecThetaOffset = 0.0f;

extern alpbet_t _2r_2s(dq_t i_dq,float theta);
extern void _3s_2s(abc_t i_abc,alpbet_t *alp_bet);
extern void _2s_2r(alpbet_t i_alphabeta,float theta,dq_t *dq);
static lowfilter_t sg_elefilter[3];
static float _get_angleoffset(void);
void motortctrl_process(void)
{
    
    switch (g_Motor1.state)
    {
    case MOTOR_INIT:
        for (unsigned char  i = 0; i < 10; i++)
        {
            float theta = 100.0f;
            theta = (*(sensor_data_t*)sensor_user_read(SENSOR_01)).cov_data;            
        }
        sg_MecThetaOffset = _get_angleoffset();
        /*-----电流滤波器初始化-------*/
        lowfilter_init(&sg_elefilter[0],80);
        lowfilter_init(&sg_elefilter[1],80);
        lowfilter_init(&sg_elefilter[2],80);

        /*------PID参数初始化---------*/
        foc_paraminit();
        sg_motordebug.iq_targe = 2.0f;
        motor_enable();
        g_Motor1.state = MOTOR_RUNING;           
        break;

    case MOTOR_TEST:

        break;

    case MOTOR_RUNING:
        {
            if (!(++g_Motor1.cnt % 5000))
            {
                /* code */
                // g_Motor1.state = MOTOR_STOP;
            }
        }
        break;
    case MOTOR_STOP:
        motor_disable();
        break;
    default:
        break;
    }
}

static float _get_angleoffset(void)
{
    motor_enable_noirq();
    /*-----------设置alpha/beta坐标系------------------*/
    alpbet_t uab = {1.0f,0.0f};
    duty_t dut01;
    float theta;
    dut01 = _svpwm(uab.alpha,uab.beta);
    motor_set_pwm(dut01._a,dut01._b,dut01._c);
    HAL_Delay(200);
    theta = (*(sensor_data_t*)sensor_user_read(SENSOR_01)).cov_data;
    motor_disable();
    return theta;
}
/*------------周期性被调用----------------*/

extern  pid_cb_t *sgp_curloop_d_pid;
extern pid_cb_t *sgp_curloop_q_pid;
float AAAAAA;
duty_t dut01,dut02;
void motorctrl_foccalc(unsigned int *abc_vale,float _elec_theta)
{
/*----------------三相电流处理------------------------------*/    
    float Ia,Ib,Ic;    
    float a1,a2,a3;
    a1 = (float)abc_vale[0] - AD_OFFSET;
    a2 = (float)abc_vale[2] - AD_OFFSET;
    a3 = (float)abc_vale[1] - AD_OFFSET;  
    Ia = (float)a1*(float)(3.3f/4096.0f/RA_S/BETA_);
    Ib = (float)a2*(float)(3.3f/4096.0f/RB_S/BETA_);
    Ic = (float)a3*(float)(3.3f/4096.0f/RC_S/BETA_);
#if 1
    abc_t i_abc;
    i_abc.a = lowfilter_cale(&sg_elefilter[0],Ia);
    i_abc.b = lowfilter_cale(&sg_elefilter[1],Ib);
    i_abc.c = lowfilter_cale(&sg_elefilter[2],Ic);
#else
    abc_t i_abc;
    i_abc.a = Ia;
    i_abc.b = Ib;
    i_abc.c = Ic;
#endif

/*---------------------获取当前转子角度-------------------------*/
    float mech_theta,elec_theta;
    static float pre_elec_theta = 0.0f;
    mech_theta = (*(sensor_data_t*)sensor_user_read(SENSOR_01)).cov_data - sg_MecThetaOffset;
    
    elec_theta = (mech_theta) * MOTOR_PAIR;    
    elec_theta = _normalize_angle(elec_theta);
    sg_motordebug.ele_angle = elec_theta;
    /*--------------对电流进行反park变化-----------------------*/
    // i_abc.a = cosf(elec_theta);
    // i_abc.b = cosf(elec_theta - 2.0/3.0f*PI);
    // i_abc.c = cosf(elec_theta + 2.0/3.0f*PI);
    dq_t i_dq;
    alpbet_t i_alphbeta;    
    _3s_2s(i_abc,&i_alphbeta);
    _2s_2r(i_alphbeta,elec_theta - PI/2.0F,&i_dq);
    sg_motordebug.id_real = i_dq.d *1000;
    sg_motordebug.iq_real = i_dq.q *1000;

#if 0
/*-------------------闭环控制-----------------------*/
    dq_t u_dq = {0.0f,2.0f};
    // u_dq.d = pid_contrl(sgp_curloop_d_pid,0.0f,i_dq.d);
    // u_dq.q = pid_contrl(sgp_curloop_q_pid,sg_motordebug.iq_targe,i_dq.q);
    sg_motordebug.pid_Q_out = u_dq.q;
    sg_motordebug.pid_D_out = u_dq.d;
    alpbet_t uab;
    uab = _2r_2s(u_dq, elec_theta);
    pre_elec_theta = elec_theta;
    duty_t dut01;
    dut01 = _svpwm(uab.alpha,uab.beta);
    motor_set_pwm(dut01._a,dut01._b,dut01._c);
#else
/*------------------------
开环控制
100us调用匹配参数  
    42BLDC  theta += 0.004f;  uq = 1.0f
    yateng  theta += 0.004f;  uq = 1.0f    
    qita    theta += 0.004f;  uq = 1.0f
---------------------------*/

    dq_t udq = {0.0f,0.60f};
    alpbet_t uab;
    #if 1//强拖
        {
            static float theta = 0.0f;
            if (theta >= _2PI)
            {
                /* code */
                theta = 0.0f;
            }
            uab = _2r_2s(udq, theta);
            theta += 0.001f;
        }
    #else //使用传感器
        uab = _2r_2s(udq, elec_theta);
        pre_elec_theta = elec_theta; 
    #endif

    dut02 = _svpwm_q15(_IQ15(uab.alpha),_IQ15(uab.beta));
    
    dut01 = _svpwm(uab.alpha,uab.beta);



    motor_set_pwm(dut01._a,dut01._b,dut01._c);
#endif

/*-------------------观测三相电流--------------------*/
    ipc_write_data(PUBLIC_DATA_IA,i_abc.a);
    ipc_write_data(PUBLIC_DATA_IB,i_abc.b);
    ipc_write_data(PUBLIC_DATA_IC,i_abc.c);
    ipc_write_data(PUBLIC_DATA_IALPHA,i_alphbeta.alpha);
    ipc_write_data(PUBLIC_DATA_IBETA,i_alphbeta.beta);    
    ipc_write_data(PUBLIC_DATA_ID,i_dq.d);
    // ipc_write_data(PUBLIC_DATA_IQ,i_dq.q);
    return;
}


