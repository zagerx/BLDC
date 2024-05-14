#include "./motorctrl.h"
#include "debuglog.h"
#include "motorctrl_common.h"
#include "motor_protocol.h"
#include "board.h"
#include "string.h"

enum{
    MOTOR_INIT,
    MOTOR_START, 
    MOTOR_RUNING,
    MOTOR_STOP,
};

static void mc_param_init(void);
static void mc_param_deinit(void);

static dq_t _currmentloop(abc_t i_abc,float ele_theta);
static short _get_angleoffset(void);
static abc_t _convert_current(unsigned int *abc_vale);
static float _get_electricalangle(void);
alpbet_t _limit_voltagecircle(alpbet_t raw_uab);
static float _speedcale(float cur_theta);
static float _speedloop(float tar,float real);

static void trigger_software_reset(void);
static void motor_enable(void);
static void motor_disable(void);
static void motor_set_pwm(duty_t temp);

static float sg_MecThetaOffset = 0.0f;

mt_param_t mt_param = {0};
motordebug_t motordebug = {0};

void motortctrl_process(void)
{
    static uint16_t _state = MOTOR_INIT;

    motorprotocol_process();
    switch (_state)
    {
    case MOTOR_INIT:
        if(strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_START].cmd)))
        {
            break;
        }
        _state = MOTOR_START;
    case MOTOR_START:
        mc_param_init();
        motor_enable();
        _state = MOTOR_RUNING;
        motorprotocol_transmit(sg_commandmap[CMD_SET_START].res_cmd,strlen(sg_commandmap[CMD_SET_START].res_cmd));
        break;

    case MOTOR_RUNING:
        if(strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_START].cmd)))
        {
            motorprotocol_transmit(sg_commandmap[CMD_SET_STOP].res_cmd,strlen(sg_commandmap[CMD_SET_STOP].res_cmd));
            _state = MOTOR_STOP;
        }
        break;

    case MOTOR_STOP:
        trigger_software_reset();
        motor_disable();
        mc_param_deinit();
        _state = MOTOR_INIT;
        break;
    default:
        break;
    }
}

void _50uscycle_process(unsigned int *abc_vale,float _elec_theta)
{
    enum{
        OFFSET=0,
        OPENLOOP,
        CLOSELOOP,
        SPEEDLOOP,
        IDLE,
    };
    static short state0_ = OFFSET;

    abc_t i_abc;
    i_abc = _convert_current(abc_vale);
    float elec_theta;
    elec_theta = _get_electricalangle();

    switch (state0_)
    {
        case OFFSET:
            if (_get_angleoffset() != 0)
            {
                break;
            }
            state0_ = CLOSELOOP;
            break;
        case CLOSELOOP:
        {
#if 0//开环 测试SVPWM功能
            dq_t udq = {0.0f,0.4f};
            if(motordebug.self_ele_theta>_2PI)
            {
                motordebug.self_ele_theta = 0.0f;
            }
            motordebug.self_ele_theta += 0.001f;
            alpbet_t uab;
            uab = _2r_2s(udq, motordebug.self_ele_theta); 
            static float test_a,test_b,test_c;
            SVM(uab.alpha,uab.beta,&test_a,&test_b,&test_c);            
            motor_set_pwm(_svpwm(uab.alpha,uab.beta));
#else
            dq_t udq = {0.0f,0.4f,_IQ15(0.0f),_IQ15(0.8f)};
            float pwm_phase;// = elec_theta + 1.5f * current_meas_period * (motordebug.real_speed);
            motordebug.self_ele_theta = pwm_phase;
            alpbet_t uab;
            uab = _limit_voltagecircle(_2r_2s(udq, pwm_phase));
            motor_set_pwm(_svpwm(uab.alpha,uab.beta)); 
#endif
        }
            break;
        default:
            break;
    }
}




static abc_t _convert_current(unsigned int *abc_vale)
{
    float Ia,Ib,Ic;
    int32_t a1,b1,c1;
    abc_t i_abc;
    a1 = abc_vale[0] - A_ADCCHANNL_OFFSET;
    b1 = abc_vale[1] - B_ADCCHANNL_OFFSET;
    c1 = abc_vale[2] - C_ADCCHANNL_OFFSET;
    Ia = (float)a1*(float)A_VOLITETOCURRENT_FACTOR;
    Ib = (float)b1*(float)B_VOLITETOCURRENT_FACTOR;
    Ic = (float)c1*(float)C_VOLITETOCURRENT_FACTOR;
    Ia = -Ic - Ib;
    i_abc.a = lowfilter_cale(&(mt_param.elefitler[0]),Ia);
    i_abc.b = lowfilter_cale(&(mt_param.elefitler[1]),Ib);
    i_abc.c = lowfilter_cale(&(mt_param.elefitler[2]),Ic);
    motordebug.ia = i_abc.a;
    motordebug.ib = i_abc.b;
    return i_abc;
}

static float _get_electricalangle(void)
{
    //获取电角度
    int32_t temp;
    float mec_theta,elec_theta;
    temp = ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_COV))[0];
    mec_theta = _IQ20toF(temp) - sg_MecThetaOffset;
    elec_theta = _normalize_angle(mec_theta * MOTOR_PAIR);
    motordebug.real_speed = _IQ15toF(((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_FILTER))[0]) * MOTOR_PAIR;
    motordebug.ele_angle = elec_theta;
    return elec_theta;
}

static short _get_angleoffset(void)
{
    static short state_ = 1;
    alpbet_t uab;
    dq_t udq = {0.4f,0.0f};
    float theta;
    static short cnt_ = 0;
    int32_t Q15_theta = 0;
    switch (state_)
    {
    case 1://读取偏移值
        uab = _2r_2s(udq,0.0f);
        motor_set_pwm(_svpwm(uab.alpha,uab.beta));
        state_ = 2;
        break;

    case 2:
        if (cnt_++ > 15000)
        {
            cnt_ = 0;
            state_ = 3;          
        }
        break;

    case 3:
        Q15_theta = ((int32_t*)(sensor_user_read(SENSOR_01,EN_SENSORDATA_COV)))[0];
        theta = _IQ20toF(Q15_theta);
        sg_MecThetaOffset = theta;
        sg_MecThetaOffset = 0.56f;
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

static void mc_param_init(void)
{

    pid_init(&(mt_param.daxis_pi),1.0f,0.0f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mt_param.qaxis_pi),1.0f,0.0f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mt_param.speedloop_pi),1.0f,0.0f,1.0f,D_MAX_VAL,D_MIN_VAL);

    lowfilter_init(&(mt_param.elefitler[0]),80);
    lowfilter_init(&(mt_param.elefitler[1]),80);
    lowfilter_init(&(mt_param.elefitler[2]),80);
}

static void mc_param_deinit(void)
{
    memset(&mt_param,0,sizeof(mt_param));
    memset(&motordebug,0,sizeof(motordebug));
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
    tim_set_pwm(temp._a ,temp._b,temp._c);
}
static void trigger_software_reset(void)
{
    HAL_NVIC_SystemReset();
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
    i_abc.a = -_tempa;
    i_abc.b = -_tempc;
    i_abc.c = -_tempb;
    motordebug.ia = i_abc.a;
    motordebug.ib = i_abc.b;
    motordebug.ic = i_abc.c;
#endif
    _3s_2s(i_abc,&i_alphbeta);   
    _2s_2r(i_alphbeta,((ele_theta)),&i_dq);
    motordebug.id = (i_dq.d);
    motordebug.iq = (i_dq.q);
    dq_t udq = {0.0f};
#if 1
    float tar_id = 0.0f,tar_iq = 0.0f;
    tar_id = motordebug.id_targe;
    tar_iq = motordebug.iq_targe;
    udq.d = pid_contrl(&(mt_param.daxis_pi),tar_id,i_dq.d);
    motordebug.id_real = i_dq.d;
    motordebug.pid_D_out = udq.d;
    udq.q =pid_contrl(&(mt_param.qaxis_pi),tar_iq,i_dq.q);
    motordebug.iq_real = i_dq.q;
    motordebug.pid_Q_out = udq.q;
#endif
    return udq;
#endif
}
static float _speedloop(float tar,float real)
{
   return pid_contrl(&(mt_param.speedloop_pi),tar,real);
}

static float _speedcale(float cur_theta)
{
    float delta_theta = 0.0f;
    static unsigned short cnt = 0;
    static float pre_speed = 0.0f;
    if (cnt++ > 20)
    {
        return pre_speed;
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

    delta_theta = lowfilter_cale(&(mt_param.speedfilter),delta_theta);
    float omeg_c = (delta_theta)/0.002f/100.0f;
    float N_rpm = omeg_c * 9.5492965855f;//60.0f/_2PI
    pre_theta = cur_theta;
    pre_speed = N_rpm;
    return N_rpm;
}
