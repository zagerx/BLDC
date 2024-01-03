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

alpbet_t a_b[12] = {
    [0] = {0.0f,1.0f},
    [1] = {-0.5,0.866025404f},
    [2] = {-0.866025404f,0.5f},

    [3] = {-1.0f,0.0f},
    [4] = {-0.866025404f,-0.5f},
    [5] = {-0.5f,-0.866025404f},

    [6] = {0.0f,-1.0},
    [7] = {0.5f,-0.866025404f},
    [8] = {0.866025404f,-0.5f},

    [9] = {1.0f,0.0f},
    [10] = {0.866025404f,0.5f},
    [11] = {0.5f,0.866025404f},
};

extern alpbet_t _2r_2s(dq_t i_dq,float theta);
static lowfilter_t sg_elefilter[3];
void motortctrl_process(void)
{
    float theta = 100.0f;
    sensor_data_t angle_data;
    sensor_data_t *pangle_data;
    switch (g_Motor1.state)
    {
    case MOTOR_INIT:
        for (unsigned char  i = 0; i < 10; i++)
        {
            theta = (*(sensor_data_t*)sensor_user_read(SENSOR_MT6818)).cov_data;            
        }
        USER_DEBUG_NORMAL("theta %f\r\n",theta);
        motor_enable_noirq();
        g_Motor1.state = MOTOR_RUNING;           
        break;

    case MOTOT_ANGIL:
        {
            /*-----------设置alpha/beta坐标系------------------*/

            /*-----------获取初始位置的偏移值------------------*/

        }
        break;

    case MOTOR_RUNING2:
        {
            
        }
        break;
#if 0
    case MOTOR_DIR2:
        {
            alpbet_t ualpbet;
            duty_t duty_01;
            for (unsigned char i = 0; i < sizeof(a_b)/sizeof(alpbet_t); i++)
            {
                /* code */
                ualpbet = a_b[i];
                duty_01 = _svpwm(ualpbet.alpha,ualpbet.beta);
                motor_set_pwm(duty_01._a,duty_01._b,duty_01._c);
                HAL_Delay(20);                
            }
        }
        break;

    case MOTOR_DIR1:
        {
            alpbet_t temp2 = {1.0f,0.0f};
            dq_t udq = {0.0f,1.0f};
            /*---判断传感器方向---*/
            static float theta = 0.0f;
            while (theta <= _2PI)
            {
                /* code */
                theta += 0.002f;
                temp2 = _2r_2s(udq,theta);  
                duty_t duty_01;
                duty_01 = _svpwm(temp2.alpha,temp2.beta);
                motor_set_pwm(duty_01._a,duty_01._b,duty_01._c);
                USER_DEBUG_NORMAL("sensor theta %f\r\n",(*(sensor_data_t*)sensor_user_read(SENSOR_MT6818)).cov_data * 57.3f);
                HAL_Delay(1);
            }
            for (unsigned char i = 0; i < 5*2; i++)
            {
                /* code */
                HAL_GPIO_TogglePin(LED_01_GPIO_Port,LED_01_Pin);
                HAL_Delay(200);
            }
        }
        break;

    case MOTOR_RUNING:
        {
            /*----------获取当前转子位置--------------*/
            float cur_theta,elec_theta;
            cur_theta = (*(sensor_data_t*)sensor_user_read(SENSOR_MT6818)).cov_data;
            elec_theta = cur_theta * MOTOR_PAIR;
            alpbet_t ualp_beta;
            dq_t udq = {0.0f,1.0f};
            ualp_beta = _2r_2s(udq,elec_theta - PI/2.0F);  
            duty_t duty_01;
            duty_01 = _svpwm(ualp_beta.alpha,ualp_beta.beta);
            motor_set_pwm(duty_01._a,duty_01._b,duty_01._c);
        }
        break;
#endif
    case MOTOR_STOP:
        motor_disable();
        g_Motor1.state = MOTOR_INIT;
        break;
    default:
        break;
    }
}


/*------------周期性被调用----------------*/
void motorctrl_foccalc(unsigned int *abc_vale,float _elec_theta)
{
    float Ia,Ib,Ic;
    float a_Iafilter100HZ = 0.0f;
    float a_Ibfilter100HZ = 0.0f;
    float a_Icfilter100HZ = 0.0f;    
    duty_t duty;
    float a1,a2,a3;
    a1 = (float)abc_vale[0] - AD_OFFSET;
    a2 = (float)abc_vale[1] - AD_OFFSET;
    a3 = (float)abc_vale[2] - AD_OFFSET;
    Ia = (float)a1*(float)(3.3f/4096.0f/RA_S/BETA_);
    Ib = (float)a2*(float)(3.3f/4096.0f/RB_S/BETA_);
    Ic = (float)a3*(float)(3.3f/4096.0f/RC_S/BETA_);
#if 1
    a_Iafilter100HZ = lowfilter_cale(&sg_elefilter[0],Ia);
    a_Ibfilter100HZ = lowfilter_cale(&sg_elefilter[1],Ib);
    a_Icfilter100HZ = lowfilter_cale(&sg_elefilter[2],Ic);
    abc_t i_abc;
    i_abc.a = a_Iafilter100HZ;
    i_abc.b = a_Ibfilter100HZ;
    i_abc.c = a_Icfilter100HZ;
#endif

    float mech_theta = 0.0f;
    float elec_theta = 0.0;
    mech_theta = (*(sensor_data_t*)sensor_user_read(SENSOR_MT6818)).cov_data - sg_MecThetaOffset;    

#if 0
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
    motor_set_pwm(duty._a,duty._b,duty._c);
}


