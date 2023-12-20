#include "motortctrl.h"
/*----------------输入配置---------------------*/
#include "tim.h"
#include "mt6816.h"
#include "adc.h"
#include "ipc.h"
// #include "Motor_Control.h"
#include "method.h"
#include "focmethod.h"
#include "motorhardware.h"
#include "debuglog_.h"
#define MOTORCTRL_PERCI            (1)

enum{
    MOTOR_INIT,
    MOTOR_ALIGN,
    MOTOR_IDLE,
    MOTOR_START, 
    MOTOR_RUNING,    
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
/*-------------由simulink提供---------*/
// extern pid_t eleloop_param;
extern alpbet_t _2r_2s(dq_t i_dq,float theta);
static lowfilter_t sg_elefilter[3];

static void motor_enable(void);
static void motor_disable(void);
static void motor_set_pwm(duty_t duty);
static unsigned short max_val_01(unsigned short a,unsigned short b,unsigned short c);
static char motor_findzerpoint(float *cur_theta,float tar_theta,float beta)
{
#if 0    
    if((tar_theta)<=*cur_theta && *cur_theta<(tar_theta+0.05f) && \
    (beta > -0.1f && beta < 0.1f))
    {
        return 0;//
    }
    return 1;//未找到
#else
   if((tar_theta)<=*cur_theta && *cur_theta<(tar_theta+0.05f))
    {
        return 0;//
    }
    return 1;//未找到
#endif    
}

void motortctrl_process(void)
{
    float theta = 100.0f;
    switch (g_Motor1.state)
    {
    case MOTOR_INIT:
        lowfilter_init(&sg_elefilter[0],80);
        lowfilter_init(&sg_elefilter[1],80);
        lowfilter_init(&sg_elefilter[2],80);

        foc_paraminit();
        for (unsigned char  i = 0; i < 10; i++)
        {
            /* code */
            theta = mt6816_readangle();
        }
        // Motor_Control_initialize();
        /*------设置IQ的目标电流------*/
        // ipc_write_data(PUBLIC_DATA_IQ_TARGET,0.8f);
        /*--------转子预定位-----------*/
        g_Motor1.state = MOTOR_OFFSET;   
        break;
    case MODE_ANALOG:          
    {
        static float temp1 = 0.0F;
        alpbet_t temp2 = {1,0};
        dq_t udq = {0,1.0f};
        motor_enable_noirq();
        /* code */
        temp1 += 0.01f;
        temp2 = _2r_2s(udq,temp1*2.0f);
        ipc_write_data(PUBLIC_DATA_IALPHA,temp2.alpha);
        ipc_write_data(PUBLIC_DATA_IBETA,temp2.beta);     
        motor_set_pwm(_svpwm(temp2.alpha,temp2.beta));   
        HAL_Delay(1);
        theta = mt6816_readangle() * 2.0f;  
        ipc_write_data(PUBLIC_DATA_TEMP0,theta); 

        if(motor_findzerpoint(&theta,0,0))
        {
            break;
        }
        motor_set_pwm(_svpwm(0.0f,0.0f)); 
        g_Motor1.state = MOTOR_IDLE;
        break;        
    }
    case MOTOR_OFFSET:
        {
            alpbet_t temp2 = {1.0f,0.0f};
            motor_enable_noirq();
            /* code */
            // temp2 = _2r_2s(udq,0.0f);  
            motor_set_pwm(_svpwm(temp2.alpha,temp2.beta));
            HAL_Delay(2000);
            sg_MecThetaOffset = mt6816_readangle();
            g_Motor1.state = MOTOR_TEST;
            for (unsigned char i = 0; i < 5*2; i++)
            {
                /* code */
                HAL_GPIO_TogglePin(LED_01_GPIO_Port,LED_01_Pin);
                HAL_Delay(200);
            }
            USER_DEBUG_NORMAL("\r\n offset %f\r\n",sg_MecThetaOffset * 57.3f);
            break;
        }
    case MOTOR_TEST:
        {
            duty_t a = {1,0,0};
            while (1)
            {
                // dq_t udq = {0.0f,1.0f};
                // alpbet_t alp_beta;
                // float nor_eletheta;
                // float test_theta = 0.0f;
                // float real_theta = 0.0f;
                // real_theta = mt6816_readangle();
                // test_theta = (real_theta - sg_MecThetaOffset)*2.0f;            
                // nor_eletheta = _normalize_angle(test_theta);
                // alp_beta = _2r_2s(udq,nor_eletheta);
                // motor_set_pwm(_svpwm(alp_beta.alpha,alp_beta.beta)); 
                // HAL_Delay(10);

                // real_theta = mt6816_readangle();
                // test_theta = (real_theta - sg_MecThetaOffset)*2.0f;            
                // nor_eletheta = _normalize_angle(test_theta);
                // alp_beta = _2r_2s(udq,nor_eletheta);
                // motor_set_pwm(_svpwm(alp_beta.alpha,alp_beta.beta)); 
                // HAL_Delay(10);
                // real_theta = mt6816_readangle();
                // test_theta = (real_theta - sg_MecThetaOffset)*2.0f;            
                // nor_eletheta = _normalize_angle(test_theta);
                // alp_beta = _2r_2s(udq,nor_eletheta);
                // motor_set_pwm(_svpwm(alp_beta.alpha,alp_beta.beta)); 
                // HAL_Delay(10);
            }
            g_Motor1.state = MOTOR_IDLE;
        }
        break;        
    case MOTOR_IDLE:
        theta = mt6816_readangle();
        ipc_write_data(PUBLIC_DATA_TEMP0,theta);
        if(!IPC_GET_EVENT(gEventGroup,KEY01_SHORT_PRESS))
        {
            break;
        }
        IPC_CLEAR_EVENT(gEventGroup,KEY01_SHORT_PRESS);
        motor_enable();
        g_Motor1.state = MOTOR_RUNING;             
    case MOTOR_RUNING:
        if(!IPC_GET_EVENT(gEventGroup,KEY01_SHORT_PRESS))
        {
            break;
        }
        IPC_CLEAR_EVENT(gEventGroup,KEY01_SHORT_PRESS); 
        g_Motor1.state = MOTOR_STOP;         
    case MOTOR_STOP:
        motor_disable();
        g_Motor1.state = MOTOR_INIT;
        break;
    default:
        break;
    }
}


/*----------------------------------------ADC中断----------------------------------------------------
** 每100us执行一次 pwm频率10KHz
*/
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    float Ia,Ib,Ic;
    float a_Iafilter100HZ = 0.0f;
    float a_Ibfilter100HZ = 0.0f;
    float a_Icfilter100HZ = 0.0f;    
    duty_t duty;
    /*------------------------电流转换-------------------------------------------*/
    float a1,a2,a3;
    a1 = (float)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1) - AD_OFFSET;
    a2 = (float)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2) - AD_OFFSET;
    a3 = (float)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_3) - AD_OFFSET;
    Ia = (float)a1*(float)(3.3f/4096.0f/RA_S/BETA_);
    Ib = (float)a2*(float)(3.3f/4096.0f/RB_S/BETA_);
    Ic = (float)a3*(float)(3.3f/4096.0f/RC_S/BETA_);
    // Ic = 0.0f - Ia - Ib; 
#if 1
    /*------------------------电流滤波--------------------------------------------------*/
    a_Iafilter100HZ = lowfilter_cale(&sg_elefilter[0],Ia);
    a_Ibfilter100HZ = lowfilter_cale(&sg_elefilter[1],Ib);
    a_Icfilter100HZ = lowfilter_cale(&sg_elefilter[2],Ic);
    abc_t i_abc;
    i_abc.a = a_Iafilter100HZ;
    i_abc.b = a_Ibfilter100HZ;
    i_abc.c = a_Icfilter100HZ;//更新到simulink
#else
    rtU.abc[0] = a_Iafilter100HZ;
    rtU.abc[1] = a_Ibfilter100HZ;
    rtU.abc[2] = a_Icfilter100HZ;//更新到simulink
    rtU.abc[0] = Ia;
    rtU.abc[1] = Ib;
    rtU.abc[2] = Ic;//更新到simulink
#endif
    

    /*-------------------------获取机械角度和电角度--------------------------------------------*/
    float mech_theta = 0.0f;
    float elec_theta = 0.0;
    mech_theta = mt6816_readangle() - sg_MecThetaOffset;    
    elec_theta = _normalize_angle(mech_theta) * MOTOR_PAIR;
    // elec_theta = mech_theta;
    // rtU.test_angle = elec_theta;//更新到simulink
    /*-------------------------设置转速-----------------------------------------------------*/
    // rtU.speed_ref = 1200;

#if 0
    /*-------------------电流环/速度环/观测器计算-----------------------------------*/
    Motor_Control_step();
    duty.duty_a = rtY.Ta;
    duty.duty_b = rtY.Tb;
    duty.duty_c = rtY.Tc;   
#else
    #if 0
    /*----当步进角度为 0.01f时，q轴设置为2.4f较为合理-----*/
        static float temp_theta = 0.0f;
        temp_theta += 0.01f;
        if (temp_theta >= _2PI)
        {
            /* code */
            temp_theta -= _2PI;
        }
        duty = foc_curloopcale(i_abc,(temp_theta));
    #else
        duty = foc_curloopcale(i_abc,(elec_theta) - PI/2.0f);
    #endif    
#endif
    /*-------------------------------更新到PWM模块-----------------------------------------*/
    motor_set_pwm(duty);
}
static void motor_set_pwm(duty_t duty)
{
    float a,b,c;
    a = ((1.0f-(float)duty._a)*_ARR);
    b = ((1.0f-(float)duty._b)*_ARR);
    c = ((1.0f-(float)duty._c)*_ARR);
    unsigned short max;
    max = max_val_01((uint16_t)a,(uint16_t)b,(uint16_t)c);	
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,(uint16_t)a);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,(uint16_t)b);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,(uint16_t)c);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,(uint16_t)(max + 100));	
}
static unsigned short max_val_01(unsigned short a,unsigned short b,unsigned short c)
{
	short max;
	if(a>b)
	{
		max = a;
	}else{
		max = b;
	}
	if(c>max)
	{
		max = c;
	}
	return max;
}
void motor_enable_noirq(void)
{
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,(uint16_t)0);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,(uint16_t)0);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,(uint16_t)0);    
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);     
}
static void motor_enable(void)
{
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3); 

    /*----------启动ADC采样--------------*/
    HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
    HAL_ADCEx_InjectedStart_IT(&hadc1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);  
}
static void motor_disable(void)
{
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,(0));
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,(0));
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,(0));
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,(0));	

    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_3); 
    HAL_ADCEx_InjectedStop_IT(&hadc1);
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_4);      
}
