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


static dq_t _currmentloop(abc_t i_abc,float ele_theta);
static short _get_angleoffset(void);
static void motor_enable(void);
static void motor_disable(void);
static void motor_set_pwm(duty_t temp);
static void mc_param_init(void);
static void mc_param_deinit(void);

alpbet_t _limit_voltagecircle(alpbet_t raw_uab);
static float motor_speedcale(float cur_theta);
static float _speedloop(float tar,float real);
static void trigger_software_reset(void);


static volatile float sg_MecThetaOffset = 0.0f;
extern const float current_meas_period;

mt_param_t mt_param = {0};
motordebug_t motordebug = {0};

void motortctrl_process(void)
{
    static uint16_t _state = MOTOR_INIT;
    char rec_buf[64];
    char cnt = 0;

    motorprotocol_process();
    switch (_state)
    {
    case MOTOR_INIT:
        // if (motordebug.cur_cmd != "motor start")//sg_commandmap[CMD_SET_START].cmd)
        if(strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_START].cmd)))
        {
            break;
        }
        _state = MOTOR_START;
    case MOTOR_START:
        mc_param_init();
        motor_enable();
        _state = MOTOR_RUNING;
        // motorprotocol_tramit(g_respondmap[RES_RUNING].respond,g_respondmap[RES_RUNING].respond_len);
        motorprotocol_tramit(sg_commandmap[CMD_SET_START].res_cmd,strlen(sg_commandmap[CMD_SET_START].res_cmd));
        break;

    case MOTOR_RUNING:
        // if (motordebug.cur_cmd == "motor stop")
        if(strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_START].cmd)))
        {
            // motorprotocol_tramit(g_respondmap[RES_STOP].respond,g_respondmap[RES_STOP].respond_len);
            motorprotocol_tramit(sg_commandmap[CMD_SET_STOP].res_cmd,strlen(sg_commandmap[CMD_SET_STOP].res_cmd));
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





/*------------周期性被调用----------------*/
float speed_real;
int32_t a1,b1,c1;
float test_a,test_b,test_c;
void _50uscycle_process(unsigned int *abc_vale,float _elec_theta)
{
    float elec_theta,mech_theta;
    int32_t mec_theta;
    duty_t dut01;
    abc_t i_abc;
    dq_t udq;

/*----------------三相电流处理------------------------------*/    
#if 1
    float Ia,Ib,Ic;
    a1 = abc_vale[0] - A_ADCCHANNL_OFFSET;
    b1 = abc_vale[1] - B_ADCCHANNL_OFFSET;
    c1 = abc_vale[2] - C_ADCCHANNL_OFFSET;
    // Ia = (float)a1*(float)A_VOLITETOCURRENT_FACTOR;
    Ib = (float)b1*(float)B_VOLITETOCURRENT_FACTOR;
    Ic = (float)c1*(float)C_VOLITETOCURRENT_FACTOR;
    Ia = -Ic - Ib;
    i_abc.a = lowfilter_cale(&(mt_param.elefitler[0]),Ia);
    i_abc.b = lowfilter_cale(&(mt_param.elefitler[1]),Ib);
    i_abc.c = lowfilter_cale(&(mt_param.elefitler[2]),Ic);
    motordebug.ia = i_abc.a;
    motordebug.ib = i_abc.b;
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
            state0_ = CLOSELOOP;
            break;
        case IDLE:
            if (motordebug.id_targe != 0.0f || motordebug.iq_targe != 0.0f)
            {
                motordebug.ele_angle = 0.0f;
                state0_ = SPEEDLOOP;
            }
            break;
        case SPEEDLOOP:
            mec_theta = ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_COV))[0];
            mech_theta = _IQ20toF(mec_theta);
            motordebug.real_speed = motor_speedcale(mech_theta);//每2ms更新一次
            elec_theta = mech_theta * MOTOR_PAIR;
            elec_theta -= 0.56f;//sg_MecThetaOffset;

            elec_theta = _normalize_angle(elec_theta);
            _currmentloop(i_abc,elec_theta);

            motordebug.ele_angle = elec_theta;    
            static unsigned short cnt = 0;
            if (cnt++ < 200)
            {
                // break;
            }
            cnt = 0;
            udq.q = 0.3f;
            udq.d = 0;
            alpbet_t uab;
            uab = _2r_2s(udq, elec_theta - PI_2);  
            uab = _limit_voltagecircle(uab);
            motor_set_pwm(_svpwm(uab.alpha,uab.beta)); 
            break;
        case CLOSELOOP:
        {
            //获取电角度
            mec_theta = ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_COV))[0];
            mech_theta = _IQ20toF(mec_theta);
            motordebug.real_speed = motor_speedcale(mech_theta);//每2ms更新一次

            elec_theta = mech_theta * MOTOR_PAIR;
            elec_theta -= sg_MecThetaOffset;

            elec_theta = _normalize_angle(elec_theta);
            speed_real = _IQ15toF(((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_FILTER))[0]);
            motordebug.ele_angle = elec_theta;

#if 0//开环
            // _currmentloop(i_abc,elec_theta);
            dq_t udq = {0.0f,0.4f,_IQ15(0.0f),_IQ15(0.0f)};
            alpbet_t uab,uab_q15;
            if(motordebug.self_ele_theta>_2PI)
            {
                motordebug.self_ele_theta = 0.0f;
            }
            motordebug.self_ele_theta += 0.001f;
            uab = _2r_2s(udq, motordebug.self_ele_theta); 
            dut01 = _svpwm(uab.alpha,uab.beta);
            motor_set_pwm(dut01);
#else
            dq_t udq = {0.0f,0.0f,_IQ15(0.0f),_IQ15(0.8f)};
            #if 0/*闭环控制*/
                udq = _currmentloop(i_abc,elec_theta - PI_2);
                // udq.d = 0.0f;
                // udq.q = 0.6f;
                // udq.d = motordebug.id_targe;
                udq.q = motordebug.iq_targe;
            #else
                if (motordebug.iq_targe < 0.0f)
                {
                _currmentloop(i_abc,elec_theta + PI_2);
                }else{
                    i_abc.a = -i_abc.a;
                    i_abc.b = -i_abc.b;
                    i_abc.c = -i_abc.c;
                    _currmentloop(i_abc,elec_theta);
                }
                // udq.d = motordebug.id_targe;
                udq.q = 0.4f;
                udq.d = 0.0f;
                // udq.q = _speedloop(300.0f,motordebug.real_speed);
            #endif
            alpbet_t uab;
            float pwm_phase = elec_theta + 1.5f * current_meas_period * speed_real*7.0;
            motordebug.self_ele_theta = pwm_phase;
            uab = _2r_2s(udq, pwm_phase);  
            uab = _limit_voltagecircle(uab);
            SVM(uab.alpha,uab.beta,&test_a,&test_b,&test_c);
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
float test_a,test_b,test_c;
static void motor_set_pwm(duty_t temp)
{
    test_a = temp._a;test_b = temp._b;test_c = temp._c;
    tim_set_pwm(test_a ,test_b,test_c);
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

    delta_theta = lowfilter_cale(&(mt_param.speedfilter),delta_theta);
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

static void mc_param_init(void)
{
    // sgp_curloop_d_pid = &sg_curloop_param.d_pid;
    // sgp_curloop_q_pid = &sg_curloop_param.q_pid;
    // sgp_speed_pid = &sg_speed_pid;
    // pid_init(sgp_curloop_d_pid,motordebug.pid_d_kp,motordebug.pid_d_ki,1.0f,D_MAX_VAL,D_MIN_VAL);
    // pid_init(sgp_curloop_q_pid,motordebug.pid_d_kp,motordebug.pid_d_ki,1.0f,D_MAX_VAL,D_MIN_VAL);
    // pid_init(sgp_speed_pid,motordebug.pid_d_kp,motordebug.pid_d_ki,1.0f,3.0,-3.0);

    // lowfilter_init(&sg_elefilter[0],80);
    // lowfilter_init(&sg_elefilter[1],80);
    // lowfilter_init(&sg_elefilter[2],80);


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


static void trigger_software_reset(void)
{
    HAL_NVIC_SystemReset();
}

