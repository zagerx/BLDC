#include "./motorctrl.h"

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
    unsigned char state;
}motorctrl_t;
motorctrl_t g_Motor1 = {
    .state = MOTOR_INIT,
};
static float sg_MecThetaOffset = 0.0f;

extern alpbet_t _2r_2s(dq_t i_dq,float theta);
extern void _3s_2s(abc_t i_abc,alpbet_t *alp_bet);
extern void _2s_2r(alpbet_t i_alphabeta,float theta,dq_t *dq);
static lowfilter_t sg_elefilter[3];
static float _get_angleoffset(void);
void motortctrl_process(void)
{
    float theta = 100.0f;

    switch (g_Motor1.state)
    {
    case MOTOR_INIT:
        for (unsigned char  i = 0; i < 10; i++)
        {
            theta = (*(sensor_data_t*)sensor_user_read(SENSOR_MT6818)).cov_data;            
        }
        USER_DEBUG_NORMAL("theta %f\r\n",theta);
        sg_MecThetaOffset = _get_angleoffset();
        /*-----电流滤波器初始化-------*/
        lowfilter_init(&sg_elefilter[0],80);
        lowfilter_init(&sg_elefilter[1],80);
        lowfilter_init(&sg_elefilter[2],80);
        motor_enable();
        g_Motor1.state = MOTOR_RUNING;           
        break;
    case MOTOR_RUNING:
        {
        }
        break;
    case MOTOR_STOP:
        motor_disable();
        g_Motor1.state = MOTOR_INIT;
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
    theta = (*(sensor_data_t*)sensor_user_read(SENSOR_MT6818)).cov_data;
    motor_disable();
    return theta;
}
/*------------周期性被调用----------------*/
void motorctrl_foccalc(unsigned int *abc_vale,float _elec_theta)
{
/*----------------三相电流处理------------------------------*/    
    float Ia,Ib,Ic;    
    float a1,a2,a3;
    a1 = (float)abc_vale[0] - AD_OFFSET;
    a2 = (float)abc_vale[1] - AD_OFFSET;
    a3 = (float)abc_vale[2] - AD_OFFSET;  
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
    mech_theta = (*(sensor_data_t*)sensor_user_read(SENSOR_MT6818)).cov_data - sg_MecThetaOffset;
    elec_theta = mech_theta * MOTOR_PAIR;    

/*----------------------开环控制-------------------*/
    dq_t udq = {0.0f,1.0f};
    alpbet_t uab;
    uab = _2r_2s(udq, elec_theta);
    duty_t dut01;
    dut01 = _svpwm(uab.alpha,uab.beta);
    motor_set_pwm(dut01._a,dut01._b,dut01._c);
    /*--------------对电流进行反park变化-----------------------*/
    dq_t i_dq;
    alpbet_t i_alphbeta;    
    _3s_2s(i_abc,&i_alphbeta);
    _2s_2r(i_alphbeta,elec_theta - PI/2.0f,&i_dq);
/*-------------------观测三相电流--------------------*/
    ipc_write_data(PUBLIC_DATA_IA,i_abc.a);
    ipc_write_data(PUBLIC_DATA_IB,i_abc.b);
    ipc_write_data(PUBLIC_DATA_IC,i_abc.c);
    ipc_write_data(PUBLIC_DATA_IALPHA,i_alphbeta.alpha);
    ipc_write_data(PUBLIC_DATA_IBETA,i_alphbeta.beta);    
    ipc_write_data(PUBLIC_DATA_ID,i_dq.d);
    ipc_write_data(PUBLIC_DATA_IQ,i_dq.q);
    return;
}


