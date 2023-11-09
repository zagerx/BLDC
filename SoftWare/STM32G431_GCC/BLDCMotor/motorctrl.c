#include "motortctrl.h"
/*----------------��������---------------------*/
#include "tim.h"
#include "mt6816.h"
#include "adc.h"
#include "ipc.h"
#include "Motor_Control.h"
#include "method.h"

extern pid_t eleloop_param;


#define MOTORCTRL_PERCI            (1)
#define AD_OFFSET                  2116
#define t_PWM                      0.0001f
#define R_S                        0.01F
#define BETA_                      5.7F
#define PI                         3.14159260f
#define _2PI                       6.3031852
float Ia,Ib,Ic;
float a_real_theta;
float a_Iafilter100HZ = 0.0f;
float a_Ibfilter100HZ = 0.0f;
float a_Icfilter100HZ = 0.0f;
lowfilter_t g_elefilter[3];

static void motor_enable(void);
static void motor_disable(void);
static void motor_set_iq(float iq);
static unsigned short max_val_01(unsigned short a,unsigned short b,unsigned short c);

typedef struct
{
    /* data */
    unsigned char state;
}motorctrl_t;
motorctrl_t g_Motor1;
void motortctrl_process(void)
{
    enum{
        MOTOR_INIT,
        MOTOR_IDLE,
        MOTOR_START, 
        MOTOR_RUNING,    
        MOTOT_FALT,
        MOTOR_STOP,
    };
    switch (g_Motor1.state)
    {
    case MOTOR_INIT:
        lowfilter_init(&g_elefilter[0],80);
        lowfilter_init(&g_elefilter[1],80);
        lowfilter_init(&g_elefilter[2],80);
        Motor_Control_initialize();

    case MOTOR_IDLE:
        if(!IPC_GET_EVENT(gEventGroup,KEY01_SHORT_PRESS))
        {
            break;
        }
        IPC_CLEAR_EVENT(gEventGroup,KEY01_SHORT_PRESS);
        motor_enable();
        g_Motor1.state = MOTOR_RUNING;
    case MOTOR_RUNING:
        if (IPC_GET_EVENT(gEventGroup,IPC_SET_IQ))
        {
            IPC_CLEAR_EVENT(gEventGroup,IPC_SET_IQ);
            motor_set_iq(ipc_read_data(PUBLIC_DATA_IQ_TARGET));
        }
        
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
static void motor_enable(void)
{
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3); 

    /*----------����ADC����--------------*/
    HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
    HAL_ADCEx_InjectedStart_IT(&hadc1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);  
}

/*----------------------------------------ADC�ж�----------------------------------------------------
** ÿ100usִ��һ�� pwmƵ��10KHz
*/
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    /*------------------------����ת��-------------------------------------------*/
    float a1,a2,a3;
    a1 = (float)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1) - AD_OFFSET;
    a2 = (float)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2) - AD_OFFSET;
    a3 = (float)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_3) - AD_OFFSET;
    Ia=(float)a1*(float)(3.3f/4096.0f/R_S/BETA_);//0.01413446
    Ib=(float)a2*(float)(3.3f/4096.0f/R_S/BETA_);
    Ic=(float)a3*(float)(3.3f/4096.0f/R_S/BETA_);	
    /*------------------------�����˲�--------------------------------------------------*/
    a_Iafilter100HZ = lowfilter_cale(&g_elefilter[0],Ia) - 0.2f;
    a_Ibfilter100HZ = lowfilter_cale(&g_elefilter[1],Ib) - 0.2f;
    a_Icfilter100HZ = lowfilter_cale(&g_elefilter[2],Ic) - 0.2f;
#if 0
    rtU.abc[0] = a_Iafilter100HZ;
    rtU.abc[1] = a_Ibfilter100HZ;
    rtU.abc[2] = a_Icfilter100HZ;//���µ�simulink
#else
    rtU.abc[0] = Ia;
    rtU.abc[1] = Ib;
    rtU.abc[2] = Ic;//���µ�simulink
#endif
    /*-------------------------��ȡ��е�ǶȺ͵�Ƕ�--------------------------------------------*/
    float mech_theta = 0.0f;
    float elec_theta = 0.0;
    mech_theta =_2PI - mt6816_readangle();//read_angle();//��е�Ƕ�
    elec_theta = mech_theta * 2;
    if (elec_theta >= _2PI)
    {
    /* code */
        elec_theta -= _2PI; 
    }
    a_real_theta  = elec_theta;
    rtU.test_angle = a_real_theta;//���µ�simulink

    /*-------------------------����ת��-----------------------------------------------------*/
    rtU.speed_ref = 1200;
    /*---------------------------������/�ٶȻ�/�۲�������-------------------------------------------*/
    Motor_Control_step();
    /*-------------------------------���µ�PWMģ��-----------------------------------------*/
    float a,b,c;
    a = ((1.0f-(float)rtY.Ta)*_ARR);
    b = ((1.0f-(float)rtY.Tb)*_ARR);
    c = ((1.0f-(float)rtY.Tc)*_ARR);  
    unsigned short max;
    max = max_val_01((uint16_t)a,(uint16_t)b,(uint16_t)c);	
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,(uint16_t)c);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,(uint16_t)b);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,(uint16_t)a);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,(uint16_t)(max + 100));	
}
static void motor_set_iq(float iq)
{
    eleloop_param.target = iq;
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
