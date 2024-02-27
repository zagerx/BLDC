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
    float ialpha;
    float ibeta;
    float id;
    float iq;

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
    float self_ele_theta;
    int32_t Q_ele_angle;
    int32_t Q15_mec_angle;
}motordebug_t;
motordebug_t sg_motordebug = {0};
typedef struct
{
    unsigned char state;
    unsigned short cnt;
}motorctrl_t;

static dq_t _currmentloop(abc_t i_abc,float ele_theta);
static float _get_angleoffset(void);
static void motor_enable(void);
static void motor_disable(void);
static void motor_set_pwm(duty_t temp);

motorctrl_t g_Motor1 = {
    .state = MOTOR_INIT,
    .cnt = 0,
};
static volatile float sg_MecThetaOffset = 0.0f;
static lowfilter_t sg_elefilter[3];
static pid_cb_t *sgp_curloop_d_pid;
static pid_cb_t *sgp_curloop_q_pid;
static curloop_t sg_curloop_param;
float pre_angle_test = 0.0f;
float next_angle_test = 0.0f;
float raw_angle = 0.0f;
float test_ia = 0.0f;
float test_ib = 0.0f;
float test_ic = 0.0f;
extern float RTT_test_Id;
void motortctrl_process(void)
{
    switch (g_Motor1.state)
    {
    case MOTOR_INIT:
        sg_MecThetaOffset = _get_angleoffset();
        foc_paraminit();
        USER_DEBUG_NORMAL("motor init\r\n");
        HAL_Delay(20);
        motor_enable();
        g_Motor1.state = MOTOR_RUNING;           
        break;

    case MOTOR_RUNING:
        {
            if (!(++g_Motor1.cnt % 15000))
            {
                // USER_DEBUG_NORMAL("motor 1s\r\n");
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
unsigned int test_val = 0;
float cur_angle = 0.0f;
void _50uscycle_process(unsigned int *abc_vale,float _elec_theta)
{
#ifdef BOARD_STM32G431    
    static unsigned int cnt = 0;
    if (cnt++%3000)
    {
        return;
    }
#endif
    test_val = 20;

    duty_t dut01,dut02;
/*----------------三相电流处理------------------------------*/    
    float Ia,Ib,Ic;
    int32_t a1,b1,c1;
    a1 = abc_vale[0] - A_ADCCHANNL_OFFSET;
    b1 = abc_vale[1] - B_ADCCHANNL_OFFSET;
    c1 = abc_vale[2] - C_ADCCHANNL_OFFSET;
    sg_motordebug.Q_ia = a1;
    sg_motordebug.Q_ib = b1;
    sg_motordebug.Q_ic = c1;    
    Ia = (float)a1*(float)A_VOLITETOCURRENT_FACTOR;
    Ib = (float)b1*(float)B_VOLITETOCURRENT_FACTOR;
    Ic = (float)c1*(float)C_VOLITETOCURRENT_FACTOR;
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
#ifdef BOARD_STM32H723
    float elec_theta,mech_theta;
    int32_t Q15_Mechtheta;
    Q15_Mechtheta = ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_COV))[0];
    mech_theta = _IQ15toF(Q15_Mechtheta);
    raw_angle = mech_theta;
    mech_theta = (mech_theta - sg_MecThetaOffset);
    elec_theta = mech_theta * MOTOR_PAIR;
    elec_theta += ANGLE_COMPENSATA;
    elec_theta = _normalize_angle(elec_theta);
    sg_motordebug.ele_angle = elec_theta;
    // _currmentloop(i_abc,sg_motordebug.self_ele_theta);
    // _currmentloop(elec_theta);
#endif

#ifdef BOARD_STM32G431

#endif
/*--------------------------------------------------------------
开环控制
100us调用匹配参数  
    42BLDC  theta += 0.004f;  uq = 1.0f
    yateng  theta += 0.004f;  uq = 1.0f
    qita    theta += 0.004f;  uq = 1.0f
--------------------------------------------------------------*/
    #if 1//强拖
        {            
            #ifdef BOARD_STM32H723
            dq_t udq = {0.0f,0.8f,_IQ15(0.0f),_IQ15(0.8f)};
            alpbet_t uab,uab_q15;            
            if (sg_motordebug.self_ele_theta > 6.28f)
            {
                sg_motordebug.self_ele_theta = 0.0f;
            }
            uab = _2r_2s(udq, sg_motordebug.self_ele_theta);
            dut01 = _svpwm(uab.alpha,uab.beta);
            motor_set_pwm(dut01);            
                sg_motordebug.self_ele_theta += 0.001f;
            #endif
            #ifdef BOARD_STM32G431
                float elec_theta,mech_theta;
                int32_t Q15_Mechtheta;
                Q15_Mechtheta = ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_COV))[0];
                mech_theta = _IQ15toF(Q15_Mechtheta);

                elec_theta = mech_theta * MOTOR_PAIR;
                elec_theta -= sg_MecThetaOffset;

                elec_theta = _normalize_angle(elec_theta);
                sg_motordebug.ele_angle = elec_theta;
                /*方法1*/
                // mech_theta = mech_theta + _2PI - sg_MecThetaOffset;
                // elec_theta = mech_theta * 2 - sg_MecThetaOffset/4.0f;
                // elec_theta = _IQ15toF(_IQ15(elec_theta) % (Q15_2PI));
                // sg_motordebug.ele_angle = elec_theta;

                dq_t udq = {0.0f,1.0f,_IQ15(0.0f),_IQ15(0.8f)};
                alpbet_t uab,uab_q15;
                if (sg_motordebug.self_ele_theta >= _2PI)
                {
                    sg_motordebug.self_ele_theta -= _2PI;
                }

                uab = _2r_2s(udq, sg_motordebug.self_ele_theta);cur_angle = sg_motordebug.self_ele_theta;
                dut01 = _svpwm(uab.alpha,uab.beta);
                motor_set_pwm(dut01);
                sg_motordebug.self_ele_theta += 0.31415926f;
                // sg_motordebug.self_ele_theta += 0.002f;
            #endif
        }
    #else //使用传感器
    {
        dq_t udq = {0.0f,-1.0f,_IQ15(0.0f),_IQ15(0.8f)};
        alpbet_t uab,uab_q15;
        #if 1/*闭环控制*/
            // udq = _currmentloop(i_abc,elec_theta);
        #endif
        uab = _2r_2s(udq, elec_theta - PI_2);
        dut01 = _svpwm(uab.alpha,uab.beta);
        motor_set_pwm(dut01);
    }
    #endif

    return;
}
void _limit_voltagecircle(uint32_t u_ref,uint32_t elec_theta)
{
    uint32_t u_alpha,u_beta;
    u_alpha = u_ref * cosf(elec_theta);
    u_beta = u_ref * sinf(elec_theta);
}
dq_t _currmentloop(abc_t i_abc,float ele_theta)
{
#if 0
    /*--------------对电流进行反Clark变化-----------------------*/
    alpbet_t i_alphbeta;
    dq_t i_dq;
    // i_abc.a = cosf(ele_theta);
    // i_abc.b = cosf(ele_theta - _2PI/3.0f);
    // i_abc.c = cosf(ele_theta + _2PI/3.0f);
    sg_motordebug.ia = i_abc.a;
    sg_motordebug.ib = i_abc.b;
    sg_motordebug.ic = i_abc.c;
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
    _2s_2r_Q(i_alphbeta,(uint32_t)_IQ15((ele_theta - PI/2.0f)),&i_dq);
    sg_motordebug.Q_id = (i_dq.Q_d);
    sg_motordebug.Q_iq = (i_dq.Q_q);
#else
    alpbet_t i_alphbeta;
    dq_t i_dq;
    test_ia = cosf(ele_theta);
    test_ib = cosf(ele_theta - _2PI/3.0f);
    test_ic = cosf(ele_theta + _2PI/3.0f);

    // i_abc.a = test_ib;
    // i_abc.b = test_ic;
    // i_abc.c = test_ia;

    // i_abc.a = test_ia;
    // i_abc.b = test_ib;
    // i_abc.c = test_ic;

    sg_motordebug.ia = i_abc.a;
    sg_motordebug.ib = i_abc.b;
    sg_motordebug.ic = i_abc.c;
    _3s_2s(i_abc,&i_alphbeta);
    sg_motordebug.ialpha = i_alphbeta.alpha;
    sg_motordebug.ibeta = i_alphbeta.beta;    
    _2s_2r(i_alphbeta,((ele_theta)),&i_dq);
    sg_motordebug.id = (i_dq.d);
    sg_motordebug.iq = (i_dq.q);

#if 1
    dq_t udq;
    udq.d =pid_contrl(sgp_curloop_d_pid,RTT_test_Id,i_dq.d);
    udq.q = 0.9f;
#endif
    return udq;
#endif
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
    HAL_Delay(500);
    #ifdef BOARD_STM32H723
        tim_encode_writecnt(0);
        theta = 0.0f;
        motor_disable();
        return theta;
    #endif

    #ifdef BOARD_STM32G431
        motor_disable();
        int32_t Q15_theta;
        int32_t Q_thetaSum = 0;
        for (uint8_t i = 0; i < 10; i++)
        {
            Q_thetaSum += ((int32_t*)(sensor_user_read(SENSOR_01,EN_SENSORDATA_COV)))[0];
        }
        Q15_theta = Q_thetaSum/10;        
        theta = _IQ15toF(Q15_theta);
        return theta;
    #endif
}

void foc_paraminit(void)
{
    sgp_curloop_d_pid = &sg_curloop_param.d_pid;
    sgp_curloop_q_pid = &sg_curloop_param.q_pid;
    pid_init(sgp_curloop_d_pid,0.4f,0.000f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(sgp_curloop_q_pid,0.020f,0.001f,1.0f,D_MAX_VAL,D_MIN_VAL);
    lowfilter_init(&sg_elefilter[0],80);
    lowfilter_init(&sg_elefilter[1],80);
    lowfilter_init(&sg_elefilter[2],80);
}