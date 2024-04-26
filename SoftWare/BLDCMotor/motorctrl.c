#include "./motorctrl.h"
#include "debuglog.h"
#include "motorctrl_common.h"

#include "board.h"
#include "string.h"

#ifdef BOARD_STM32G431
#endif
#define Q15_PI_2                   (51471)
#define Q15_2PI                    (205884)
#define PI_2                       (1.570796f)
#define MOTORCTRL_PERCI            (1)

enum{
    MOTOR_ZERO,
    MOTOR_INIT,
    MOTOR_IDLE,
    MOTOR_START, 
    MOTOR_RUNING,
    MOTOR_STOP,
};

motordebug_t sg_motordebug = {0};
typedef struct
{
    unsigned char state;
    unsigned short cnt;
}motorctrl_t;

static dq_t _currmentloop(abc_t i_abc,float ele_theta);
static short _get_angleoffset(void);
static void motor_enable(void);
static void motor_disable(void);
static void motor_set_pwm(duty_t temp);
static void sg_debug_clear(void)
{
    sg_motordebug.id_targe =0.0f;
    sg_motordebug.iq_targe =0.0f;
    sg_motordebug.ele_angle = 0.0f;
    memset(&sg_motordebug,0,sizeof(sg_motordebug)-4);
}
alpbet_t _limit_voltagecircle(alpbet_t raw_uab);
static float motor_speedcale(float cur_theta);
static float _speedloop(float tar,float real);

motorctrl_t g_Motor1 = {
    .state = MOTOR_INIT,
    .cnt = 0,
};
static volatile float sg_MecThetaOffset = 0.0f;
static lowfilter_t sg_elefilter[3];
lowfilter_t sg_speedfilter = {0.0f};
pid_cb_t test_d_pid = {0};
static pid_cb_t *sgp_curloop_d_pid;
static pid_cb_t *sgp_curloop_q_pid;
static pid_cb_t *sgp_speed_pid;
static pid_cb_t sg_speed_pid;
static curloop_t sg_curloop_param;


void motortctrl_process(void)
{
    char rec_buf[64];
    char cnt = 0;
    // if (readline_fromPC(rec_buf,sizeof(rec_buf)))
    // {
    //     motorprotocol_pause(rec_buf);
    // }
    motorprotocol_process();
    switch (g_Motor1.state)
    {
    case MOTOR_INIT:
        if (sg_motordebug.motor_stat != 4)
        {
            break;
        }
        foc_paraminit();
        lowfilter_init(&sg_speedfilter,80);

        motor_enable();
        g_Motor1.state = MOTOR_RUNING;
        break;

    case MOTOR_RUNING:
        {
            if (sg_motordebug.motor_statue_To == "motor stop")
            {
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
    float elec_theta,mech_theta;
    int32_t Q15_Mechtheta;    
    duty_t dut01;
    abc_t i_abc;
    dq_t udq;

/*----------------三相电流处理------------------------------*/    
#if 1
    float Ia,Ib,Ic;
    int32_t a1,b1,c1;
    a1 = abc_vale[0] - A_ADCCHANNL_OFFSET;
    b1 = abc_vale[1] - B_ADCCHANNL_OFFSET;
    c1 = abc_vale[2] - C_ADCCHANNL_OFFSET;
    sg_motordebug.Q_ia = a1;
    Ia = (float)a1*(float)A_VOLITETOCURRENT_FACTOR;
    Ib = (float)b1*(float)B_VOLITETOCURRENT_FACTOR;
    // Ic = (float)c1*(float)C_VOLITETOCURRENT_FACTOR;
    Ic = -Ia - Ib;
    i_abc.a = lowfilter_cale(&sg_elefilter[0],Ia);
    i_abc.b = lowfilter_cale(&sg_elefilter[1],Ib);
    i_abc.c = lowfilter_cale(&sg_elefilter[2],Ic);

#endif

    enum{
        OFFSET=0,
        OPENLOOP,
        CLOSELOOP,
        SPEEDLOOP,
        IDLE,
    };
    static short state0_ = OFFSET;
    switch (state0_)
    {
        case OFFSET:
            if (_get_angleoffset() != 0)
            {
                break;
            }
            USER_DEBUG_NORMAL("enter IDLE\r\n");
            state0_ = IDLE;
            break;
        case IDLE:
            if (sg_motordebug.id_targe != 0.0f || sg_motordebug.iq_targe != 0.0f)
            {
                sg_motordebug.ele_angle = 0.0f;
                state0_ = SPEEDLOOP;
            }
            break;
        case SPEEDLOOP:
            Q15_Mechtheta = ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_COV))[0];
            mech_theta = _IQ20toF(Q15_Mechtheta);
            sg_motordebug.real_speed = motor_speedcale(mech_theta);//每2ms更新一次
            elec_theta = mech_theta * MOTOR_PAIR;
            elec_theta -= sg_MecThetaOffset;

            elec_theta = _normalize_angle(elec_theta);
            _currmentloop(i_abc,elec_theta);

            sg_motordebug.ele_angle = elec_theta;    
            static unsigned short cnt = 0;
            if (cnt++ < 200)
            {
                // break;
            }
            cnt = 0;
            udq.q = 0.3f;//pid_contrl(sgp_speed_pid,300,sg_motordebug.real_speed);
            udq.d = 0;
            alpbet_t uab;
            uab = _2r_2s(udq, elec_theta - PI_2);  
            uab = _limit_voltagecircle(uab);
            motor_set_pwm(_svpwm(uab.alpha,uab.beta)); 
            break;
        case CLOSELOOP:
        {
            //获取电角度
            Q15_Mechtheta = ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_COV))[0];
            mech_theta = _IQ20toF(Q15_Mechtheta);
            sg_motordebug.real_speed = motor_speedcale(mech_theta);//每2ms更新一次

            elec_theta = mech_theta * MOTOR_PAIR;
            elec_theta -= sg_MecThetaOffset;

            elec_theta = _normalize_angle(elec_theta);
            sg_motordebug.ele_angle = elec_theta;

#if 1//开环
            _currmentloop(i_abc,elec_theta);
            dq_t udq = {0.0f,-0.2f,_IQ15(0.0f),_IQ15(0.0f)};
            alpbet_t uab,uab_q15;
            if(sg_motordebug.self_ele_theta>_2PI)
            {
                sg_motordebug.self_ele_theta = 0.0f;
            }
            sg_motordebug.self_ele_theta += 0.001f;
            uab = _2r_2s(udq, sg_motordebug.self_ele_theta); 
            dut01 = _svpwm(uab.alpha,uab.beta);
            motor_set_pwm(dut01);
#else
            dq_t udq = {0.0f,0.0f,_IQ15(0.0f),_IQ15(0.8f)};
            #if 0/*闭环控制*/
                udq = _currmentloop(i_abc,elec_theta - PI_2);
                // udq.d = 0.0f;
                // udq.q = 0.6f;
                // udq.d = sg_motordebug.id_targe;
                udq.q = sg_motordebug.iq_targe;
            #else
                if (sg_motordebug.iq_targe < 0.0f)
                {
                _currmentloop(i_abc,elec_theta + PI_2);
                }else{
                    i_abc.a = -i_abc.a;
                    i_abc.b = -i_abc.b;
                    i_abc.c = -i_abc.c;
                    _currmentloop(i_abc,elec_theta);
                }
                // udq.d = sg_motordebug.id_targe;
                udq.q = sg_motordebug.iq_targe;
                udq.d = 0.0f;
                // udq.q = _speedloop(300.0f,sg_motordebug.real_speed);
            #endif
            alpbet_t uab;
            uab = _2r_2s(udq, elec_theta);  
            uab = _limit_voltagecircle(uab);
            motor_set_pwm(_svpwm(uab.alpha,uab.beta)); 
#endif
        }
            break;
        default:
            break;
    }
}



alpbet_t _limit_voltagecircle(alpbet_t raw_uab) {  
    float max_voltage = D_MAX_VAL;// 这里应该是你的最大允许电压值，例如对于三相电机通常是直流母线电压的根号3/2倍  
    float voltage_magnitude = sqrt(raw_uab.alpha * raw_uab.alpha + raw_uab.beta * raw_uab.beta); // 计算当前矢量的长度（模）  
    alpbet_t uab;
    if (voltage_magnitude > max_voltage) {  
        // 如果矢量长度超过最大电压，则按比例缩放矢量以使其落在电压矢量圆内  
        float scale_factor = max_voltage / voltage_magnitude;  
        uab.alpha = raw_uab.alpha * scale_factor;  
        uab.beta = raw_uab.beta * scale_factor;  
    } else {  
        // 如果矢量长度在允许范围内，则不进行任何改变  
        uab.alpha = raw_uab.alpha;  
        uab.beta = raw_uab.beta;  
    }
    return uab;
}
dq_t _currmentloop(abc_t i_abc,float ele_theta)
{
#if 1
    alpbet_t i_alphbeta;
    float _tempa,_tempb,_tempc;
    dq_t i_dq;
    _tempa = i_abc.a;
    _tempb = i_abc.b;
    _tempc = i_abc.c;
#ifdef BOARD_STM32G431
    /*bac  */
    i_abc.a = -_tempa;
    i_abc.b = -_tempc;
    i_abc.c = -_tempb;
    sg_motordebug.ia = i_abc.a;
    sg_motordebug.ib = i_abc.b;
    sg_motordebug.ic = i_abc.c;
#endif
    _3s_2s(i_abc,&i_alphbeta);   
    _2s_2r(i_alphbeta,((ele_theta)),&i_dq);
    sg_motordebug.id = (i_dq.d);
    sg_motordebug.iq = (i_dq.q);
    dq_t udq = {0.0f};
#if 1
    float tar_id = 0.0f,tar_iq = 0.0f;
    tar_id = sg_motordebug.id_targe;
    tar_iq = sg_motordebug.iq_targe;
    udq.d = pid_contrl(sgp_curloop_d_pid,tar_id,i_dq.d);
    sg_motordebug.id_real = i_dq.d;
    sg_motordebug.pid_D_out = udq.d;
    udq.q =pid_contrl(sgp_curloop_q_pid,tar_iq,i_dq.q);
    sg_motordebug.iq_real = i_dq.q;
    sg_motordebug.pid_Q_out = udq.q;
#endif
    return udq;
#endif
}
static float _speedloop(float tar,float real)
{
   return pid_contrl(sgp_speed_pid,tar,real);
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

static float motor_speedcale(float cur_theta)
{
    float delta_theta = 0.0f;
    static unsigned short cnt = 0;
    if (cnt++ > 20)
    {
        return;
    }
    cnt = 0;
    static float pre_theta = 0.0f;
    delta_theta = (cur_theta -pre_theta)*100.0f;

    // 处理角度跨越2π边界的情况  
    if (delta_theta > _2PI) {  
        delta_theta -= (int)(delta_theta / _2PI +1) * _2PI;  
    } else if (delta_theta < -_2PI) {  
        delta_theta += (int)(-delta_theta / _2PI + 1) * _2PI;  
    }  

    delta_theta = lowfilter_cale(&sg_speedfilter,delta_theta);
    float omeg_c = (delta_theta)/0.002f/100.0f;
    float N_rpm = omeg_c * 9.5492965855f;//60.0f/_2PI
    pre_theta = cur_theta;
    // N_rpm = lowfilter_cale(&sg_speedfilter,N_rpm);
    return N_rpm;
}





static short _get_angleoffset(void)
{
    static short state_ = 1;

    /*-----------设置alpha/beta坐标系------------------*/
    alpbet_t uab;
    dq_t udq = {0.4f,0.0f};
    duty_t dut01;
    float theta;
    static short cnt_ = 0;
    int32_t Q15_theta = 0;
    int32_t Q_thetaSum = 0;
    switch (state_)
    {
    case 1://读取偏移值
        uab = _2r_2s(udq,0.0f);
        dut01 = _svpwm(uab.alpha,uab.beta);
        motor_set_pwm(dut01);
        state_ = 2;
        break;

    case 2:
        if (cnt_++ > 15000)
        {
            cnt_ = 0;
            state_ = 3;
            // dut01 = _svpwm(0.0f,00.0f);
            // motor_set_pwm(dut01);             
        }
        break;

    case 3:
   
        // for (uint8_t i = 0; i < 10; i++)
        // {
        //     Q_thetaSum += ((int32_t*)(sensor_user_read(SENSOR_01,EN_SENSORDATA_COV)))[0];
        // }
        // Q15_theta = Q_thetaSum/10;
        Q15_theta = ((int32_t*)(sensor_user_read(SENSOR_01,EN_SENSORDATA_COV)))[0];
        theta = _IQ20toF(Q15_theta);
        sg_MecThetaOffset = theta;
        state_ = 4;
        break;

    case 4:

        break;             
    default:
        break;
    }
    if (state_ == 4)
    {
        return 0;
    }
    return 1;
}

void foc_paraminit(void)
{
    sgp_curloop_d_pid = &sg_curloop_param.d_pid;
    sgp_curloop_q_pid = &sg_curloop_param.q_pid;
    sgp_speed_pid = &sg_speed_pid;
    pid_init(sgp_curloop_d_pid,sg_motordebug.pid_d_kp,sg_motordebug.pid_d_ki,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(sgp_curloop_q_pid,sg_motordebug.pid_d_kp,sg_motordebug.pid_d_ki,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(sgp_speed_pid,sg_motordebug.pid_d_kp,sg_motordebug.pid_d_ki,1.0f,3.0,-3.0);

    lowfilter_init(&sg_elefilter[0],80);
    lowfilter_init(&sg_elefilter[1],80);
    lowfilter_init(&sg_elefilter[2],80);
}

