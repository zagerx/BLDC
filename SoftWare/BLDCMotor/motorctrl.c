#include "./motorctrl.h"
#include "debuglog.h"
#include "motorctrl_common.h"
#ifdef BOARD_STM32G431
#include "mt6816.h"
#endif
#define Q15_PI_2                   (51471)
#define Q15_2PI                    (205884)
#define PI_2                       (1.570796f)
#define MOTORCTRL_PERCI            (1)

enum{
    MOTOR_INIT,
    MOTOR_IDLE,
    MOTOR_START, 
    MOTOR_RUNING,
    MOTOR_STOP,
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

    int32_t Q_ia;
    int32_t Q_ib;
    int32_t Q_ic;
    int32_t Q_ialpha;
    int32_t Q_ibeta;
    int32_t Q_id;
    int32_t Q_iq;

    float id_real;
    float iq_targe;
    float iq_real;

    float pid_Q_out;
    float pid_D_out;
    float ele_angle;
    int32_t Q_ele_angle;
}motordebug_t;
motordebug_t sg_motordebug;
typedef struct
{
    unsigned char state;
    unsigned short cnt;
}motorctrl_t;

static void _currmentloop(abc_t i_abc,float theta);
static float _get_angleoffset(void);
static void motor_enable(void);
static void motor_disable(void);
static void motor_set_pwm(duty_t temp);

motorctrl_t g_Motor1 = {
    .state = MOTOR_INIT,
    .cnt = 0,
};
static float sg_MecThetaOffset = 0.0f;
static int32_t sg_Q_MecThetaOffset;
static lowfilter_t sg_elefilter[3];
static pid_cb_t *sgp_curloop_d_pid;
static pid_cb_t *sgp_curloop_q_pid;
static curloop_t sg_curloop_param;

void motortctrl_process(void)
{
    switch (g_Motor1.state)
    {
    case MOTOR_INIT:
        #ifdef BOARD_STM32G431
            sg_MecThetaOffset = _get_angleoffset();
        #endif    
        foc_paraminit();
        motor_enable();
        g_Motor1.state = MOTOR_RUNING;           
        break;

    case MOTOR_RUNING:
        {
            if (!(++g_Motor1.cnt % 15000))
            {
                // USER_DEBUG_NORMAL("motor 1s\r\n");
                /* code */
                g_Motor1.state = MOTOR_STOP;
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
/*------------周期性被调用----------------*/
void _50uscycle_process(unsigned int *abc_vale,float _elec_theta)
{
    duty_t dut01,dut02;
/*----------------三相电流处理------------------------------*/    
    float Ia,Ib,Ic;
    float a1 = 0,a2,a3 = 0;
    a1 = (float)abc_vale[0] - AD_OFFSET;
    a3 = (float)abc_vale[1] - AD_OFFSET - 15;
    a2 = (float)abc_vale[2] - AD_OFFSET - 34;
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

#if 0
/*-------------------闭环控制-----------------------*/
    _currmentloop(i_abc,theata);
#else
/*------------------------
开环控制
100us调用匹配参数  
    42BLDC  theta += 0.004f;  uq = 1.0f
    yateng  theta += 0.004f;  uq = 1.0f
    qita    theta += 0.004f;  uq = 1.0f
---------------------------*/
    dq_t udq = {0.0f,0.8f,_IQ15(0.0f),_IQ15(0.8f)};
    alpbet_t uab,uab_q15;
    #if 1//强拖
        {
            static float theta = 0.0f;
            if (theta >= _2PI)
            {
                theta = 0.0f;
            }
            uab = _2r_2s(udq, theta);
            dut01 = _svpwm(uab.alpha,uab.beta);
            // uab = _2r_2s_Q(udq, _IQ15(theta));
            motor_set_pwm(dut01);
            theta += 0.002f;
        }
    #else //使用传感器
    {
        float elec_theta,mech_theta;
        mech_theta = *(float *)mt6816_read()- sg_MecThetaOffset;
        elec_theta = mech_theta*MOTOR_PAIR ;
        elec_theta = _normalize_angle(elec_theta);
        sg_motordebug.ele_angle = elec_theta;
        uab = _2r_2s(udq, elec_theta);
        dut01 = _svpwm(uab.alpha,uab.beta);
        motor_set_pwm(dut01);
    }
        

        // int32_t elec_theta;
        // uab = _2r_2s(udq, elec_theta);
        // elec_theta = ((*(sensor_data_t *)sensor_user_read(SENSOR_01)).covdata_buf[0] - (int32_t)sg_Q_MecThetaOffset);
        // elec_theta = _IQ15mpy(elec_theta,2<<15);
        // if (elec_theta >= Q15_2PI)
        // {
        //     /* code */
        //     elec_theta -= Q15_2PI;
        // }
        // elec_theta > 0? elec_theta:(elec_theta+Q15_2PI);
        // sg_motordebug.Q_ele_angle = elec_theta;
        // uab = _2r_2s_Q(udq, (elec_theta));
        // dut02 = _svpwm_Q(uab.Q_alpha,(uab.Q_beta));
        // motor_set_pwm(dut02);
    #endif
    // dut02 = _svpwm_Q(uab.Q_alpha,(uab.Q_beta));
    // dut01 = dut02;
    // dut01 = _svpwm(uab.alpha,uab.beta);
    // motor_set_pwm(dut01);
#endif 
    return;
}
void _limit_voltagecircle(uint32_t u_ref,uint32_t elec_theta)
{
    uint32_t u_alpha,u_beta;
    u_alpha = u_ref * cosf(elec_theta);
    u_beta = u_ref * sinf(elec_theta);

}
void _currmentloop(abc_t i_abc,float theta)
{
/*---------------------获取当前转子角度-------------------------*/
    float mech_theta,elec_theta;
    static float pre_elec_theta = 0.0f;
    // mech_theta = (*(sensor_data_t*)sensor_user_read(SENSOR_01)).cov_data - sg_MecThetaOffset;
    elec_theta = (mech_theta) * MOTOR_PAIR;    
    elec_theta = _normalize_angle(elec_theta);
    sg_motordebug.ele_angle = elec_theta;
    /*--------------对电流进行反Clark变化-----------------------*/
    alpbet_t i_alphbeta;
    dq_t i_dq;
    i_abc.Q_a = _IQ15(i_abc.a);
    i_abc.Q_b = _IQ15(i_abc.b);
    i_abc.Q_c = _IQ15(i_abc.c);
    sg_motordebug.Q_ia = i_abc.Q_a;
    sg_motordebug.Q_ib = i_abc.Q_b;
    sg_motordebug.Q_ic = i_abc.Q_c;

    _3s_2s_Q(i_abc,&i_alphbeta);
    sg_motordebug.Q_ialpha = i_alphbeta.Q_alpha;
    sg_motordebug.Q_ibeta = i_alphbeta.Q_beta;

    /*-------------对电流进行反Park变换------------------------*/
    _2s_2r_Q(i_alphbeta,(uint32_t)_IQ15(_normalize_angle(elec_theta - PI/2.0f)),&i_dq);
    sg_motordebug.Q_id = (i_dq.Q_d);
    sg_motordebug.Q_iq = (i_dq.Q_q);

}
static void motor_enable(void)
{
#ifdef BOARD_STM32H723
    gpio_setmotor_power();
#endif
    /*pwm 使能*/
    tim_pwm_enable();
    tim_tigger_adc();
    adc_start();
}
static void motor_enable_noirq(void)
{
#ifdef BOARD_STM32H723
    gpio_setmotor_power();
#endif    
    /*pwm 使能*/
    tim_pwm_enable();
}
static void motor_disable(void)
{
#ifdef BOARD_STM32H723
    gpio_setmotor_powerdown();
#endif    
    tim_pwm_disable();
    adc_stop();
}
static void motor_set_pwm(duty_t temp)
{
    tim_set_pwm(temp._a,temp._b,temp._c);
}
static float _get_angleoffset(void)
{
    motor_enable_noirq();
    /*-----------设置alpha/beta坐标系------------------*/
    alpbet_t uab = {1.0f,0.0f};
    duty_t dut01;
    float theta;
    dut01 = _svpwm(uab.alpha,uab.beta);
    motor_set_pwm(dut01);
    HAL_Delay(200);
    // theta = (*(sensor_data_t *)sensor_user_read(SENSOR_01)).covdata_buf[0];
    // sg_Q_MecThetaOffset = (*(sensor_data_t *)sensor_user_read(SENSOR_01)).covdata_buf[0];
    // theta = *(float *)mt6816_read();
    motor_disable();
    
    return theta;
}

void foc_paraminit(void)
{
    sgp_curloop_d_pid = &sg_curloop_param.d_pid;
    sgp_curloop_q_pid = &sg_curloop_param.q_pid;
    pid_init(sgp_curloop_d_pid,0.08f,0.001f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(sgp_curloop_q_pid,0.020f,0.001f,1.0f,D_MAX_VAL,D_MIN_VAL);
    lowfilter_init(&sg_elefilter[0],80);
    lowfilter_init(&sg_elefilter[1],80);
    lowfilter_init(&sg_elefilter[2],80);
}