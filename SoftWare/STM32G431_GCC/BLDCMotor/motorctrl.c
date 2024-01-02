#include "./motorctrl.h"

/*----------------ģ���ڲ����໥����---------------------*/
#include "focmethod.h"

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
/*-------------��simulink�ṩ---------*/
// extern pid_t eleloop_param;
extern alpbet_t _2r_2s(dq_t i_dq,float theta);
static lowfilter_t sg_elefilter[3];

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
            theta = (*(sensor_data_t*)sensor_user_real_read(SENSOR_MT6818)).cov_data;
        }
        // Motor_Control_initialize();
        /*------����IQ��Ŀ�����------*/
        // ipc_write_data(PUBLIC_DATA_IQ_TARGET,0.8f);
        /*--------ת��Ԥ��λ-----------*/
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

        duty_t duty_01;
        duty_01 = _svpwm(temp2.alpha,temp2.beta);
        motor_set_pwm(duty_01._a,duty_01._b,duty_01._c);   
        HAL_Delay(1);
        theta = (*(sensor_data_t*)sensor_user_real_read(SENSOR_MT6818)).cov_data * 2.0f;  
        ipc_write_data(PUBLIC_DATA_TEMP0,theta); 

        // if(motor_findzerpoint(&theta,0,0))
        // {
        //     break;
        // }
        duty_01 = _svpwm(temp2.alpha,temp2.beta);
        motor_set_pwm(duty_01._a,duty_01._b,duty_01._c); 
        g_Motor1.state = MOTOR_IDLE;
        break;        
    }
    case MOTOR_OFFSET:
        {
            alpbet_t temp2 = {1.0f,0.0f};
            motor_enable_noirq();
            /* code */
            // temp2 = _2r_2s(udq,0.0f);  
            duty_t duty_01;
            duty_01 = _svpwm(temp2.alpha,temp2.beta);
            motor_set_pwm(duty_01._a,duty_01._b,duty_01._c); 
            HAL_Delay(2000);
            sg_MecThetaOffset = (*(sensor_data_t*)sensor_user_real_read(SENSOR_MT6818)).cov_data;
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
            g_Motor1.state = MOTOR_IDLE;
        }
        break;        
    case MOTOR_IDLE:
        theta = (*(sensor_data_t*)sensor_user_real_read(SENSOR_MT6818)).cov_data;
        ipc_write_data(PUBLIC_DATA_TEMP0,theta);
        USER_DEBUG_NORMAL("theta %f\r\n",theta);
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



void motorctrl_foccalc(unsigned int *abc_vale,float _elec_theta)
{
    float Ia,Ib,Ic;
    float a_Iafilter100HZ = 0.0f;
    float a_Ibfilter100HZ = 0.0f;
    float a_Icfilter100HZ = 0.0f;    
    duty_t duty;
    /*------------------------����ת��-------------------------------------------*/
    float a1,a2,a3;
    a1 = (float)abc_vale[0] - AD_OFFSET;
    a2 = (float)abc_vale[1] - AD_OFFSET;
    a3 = (float)abc_vale[2] - AD_OFFSET;
    Ia = (float)a1*(float)(3.3f/4096.0f/RA_S/BETA_);
    Ib = (float)a2*(float)(3.3f/4096.0f/RB_S/BETA_);
    Ic = (float)a3*(float)(3.3f/4096.0f/RC_S/BETA_);
    // Ic = 0.0f - Ia - Ib; 
#if 1
    /*------------------------�����˲�--------------------------------------------------*/
    a_Iafilter100HZ = lowfilter_cale(&sg_elefilter[0],Ia);
    a_Ibfilter100HZ = lowfilter_cale(&sg_elefilter[1],Ib);
    a_Icfilter100HZ = lowfilter_cale(&sg_elefilter[2],Ic);
    abc_t i_abc;
    i_abc.a = a_Iafilter100HZ;
    i_abc.b = a_Ibfilter100HZ;
    i_abc.c = a_Icfilter100HZ;//���µ�simulink
#else
    rtU.abc[0] = a_Iafilter100HZ;
    rtU.abc[1] = a_Ibfilter100HZ;
    rtU.abc[2] = a_Icfilter100HZ;//���µ�simulink
    rtU.abc[0] = Ia;
    rtU.abc[1] = Ib;
    rtU.abc[2] = Ic;//���µ�simulink
#endif

    /*-------------------------��ȡ��е�ǶȺ͵�Ƕ�--------------------------------------------*/
    float mech_theta = 0.0f;
    float elec_theta = 0.0;
    mech_theta = (*(sensor_data_t*)sensor_user_real_read(SENSOR_MT6818)).cov_data - sg_MecThetaOffset;    
    elec_theta = _normalize_angle(mech_theta) * MOTOR_PAIR;
    // elec_theta = mech_theta;
    // rtU.test_angle = elec_theta;//���µ�simulink
    /*-------------------------����ת��-----------------------------------------------------*/
    // rtU.speed_ref = 1200;

#if 0
    /*-------------------������/�ٶȻ�/�۲�������-----------------------------------*/
    Motor_Control_step();
    duty.duty_a = rtY.Ta;
    duty.duty_b = rtY.Tb;
    duty.duty_c = rtY.Tc;   
#else
    #if 0
    /*----�������Ƕ�Ϊ 0.01fʱ��q������Ϊ2.4f��Ϊ����-----*/
        static float temp_theta = 0.0f;
        temp_theta += 0.01f;
        if (temp_theta >= _2PI)
        {
            /* code */
            temp_theta -= _2PI;
        }
        duty = foc_curloopcale(i_abc,(temp_theta));
    #else
        duty = foc_curloopcale(i_abc,(elec_theta));
    #endif    
#endif
    /*-------------------------------���µ�PWMģ��-----------------------------------------*/
    motor_set_pwm(duty._a,duty._b,duty._c);
}


