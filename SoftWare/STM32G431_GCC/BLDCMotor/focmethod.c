#include "focmethod.h"
#include "math.h"
// #include "xxxx.h"
#include "ipc.h"
#include "motorhardware.h"

 alpbet_t _2r_2s(dq_t i_dq,float theta);
static void _2s_2r(alpbet_t i_alphabeta,float theta,dq_t *dq);
static void _3s_2s(abc_t i_abc,alpbet_t *alp_bet);
static dq_t circle_limit(dq_t dq);

static curloop_t sg_curloop_param;
static pid_cb_t *sgp_curloop_d_pid;
static pid_cb_t *sgp_curloop_q_pid;
static pid_cb_t sg_Q_cur;
void foc_paraminit(void)
{
    sgp_curloop_d_pid = &sg_curloop_param.d_pid;
    sgp_curloop_q_pid = &sg_curloop_param.q_pid;
    pid_init(sgp_curloop_d_pid,0.2f,0.0001f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(sgp_curloop_q_pid,0.050f,0.01f,1.0f,Q_MAX_VAL,Q_MIN_VAL);
}

/*-----------------------------------------
theta:电角度
*/
duty_t foc_curloopcale(abc_t i_abc,float theta)
{    
    dq_t i_dq;
    dq_t udq_limt;
    alpbet_t i_alphbeta;
    float real_id,real_iq;
#if 1
    // i_abc.a = cosf(theta);
    // i_abc.b = cosf(theta - _2PI/3.0f);
    // i_abc.c = cosf(theta + _2PI/3.0f);
    _3s_2s(i_abc,&i_alphbeta);
    _2s_2r(i_alphbeta,_normalize_angle(theta - PI/2.0f),&i_dq);
    // udq_limt = circle_limit(i_dq);
    // real_id = udq_limt.d;
    // real_iq = udq_limt.q;
    real_id = i_dq.d;
    real_iq = i_dq.q;
    /*------------电流环PI---------------*/
    
    float target_uq = 8.0f;  
    dq_t u_dq;  
    u_dq.d = 0.0f;
    u_dq.q = -1.0f;
    // u_dq.d = pid_contrl(sgp_curloop_d_pid,0.0f,real_id);
    // u_dq.q = pid_contrl(sgp_curloop_q_pid,target_uq,real_iq);
    /*------------IPC DATA--------------*/
    ipc_write_data(PUBLIC_DATA_IA,i_abc.a);
    ipc_write_data(PUBLIC_DATA_IB,i_abc.b);
    ipc_write_data(PUBLIC_DATA_IC,i_abc.c);
    ipc_write_data(PUBLIC_DATA_IALPHA,i_alphbeta.alpha);
    ipc_write_data(PUBLIC_DATA_IBETA,i_alphbeta.beta);
    ipc_write_data(PUBLIC_DATA_ID,real_id);
    ipc_write_data(PUBLIC_DATA_IQ,real_iq);
    // ipc_write_data(PUBLIC_DATA_TEMP0,u_dq.q);
    ipc_write_data(PUBLIC_DATA_TEMP0,theta);
#else
    dq_t u_dq;  
    u_dq.d = 0.0f;
    u_dq.q = 1.0f;    
#endif


    alpbet_t u_alphabeta;
    u_alphabeta = _2r_2s(u_dq,(theta));// + PI/2.0f); 
    duty_t duty;
    duty = _svpwm(u_alphabeta.alpha,u_alphabeta.beta);
    return duty;
}
#if 1
/*----*/
duty_t _svpwm(float ualpha,float ubeta)
{    
    unsigned char sector;    
    sector = 0;
    /*--------------判断扇区-----------------*/
    if(ubeta > 0.0F) {
        sector = 1;
    }
    if((sqrt3 * ualpha - ubeta) / 2.0F > 0.0F) {
        sector += 2;
    }  
    if((-sqrt3 * ualpha - ubeta) / 2.0F > 0.0F) {
        sector += 4;
    }

    /*--------------------计算主副矢量--------------------------------*/
    float s_vector = 0.0f,m_vector = 0.0f;
    switch (sector) {
        case 1:
            m_vector = (-1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC;
            s_vector = (1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC;
        break;

        case 2:
            m_vector = (1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC;
            s_vector = -(sqrt3 * ubeta * T_UDC);
        break;

        case 3:
            m_vector = -((-1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC);
            s_vector = sqrt3 * ubeta * T_UDC;
        break;

        case 4:
            m_vector = -(sqrt3 * ubeta * T_UDC);
            s_vector = (-1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC;
        break;

        case 5:
            m_vector = sqrt3 * ubeta * T_UDC;
            s_vector = -((1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC);
        break;

        default:
            m_vector = -((1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC);
            s_vector = -((-1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC);
        break;
    }
    /*---------------------------限制矢量圆---------------------------------*/
    if (m_vector + s_vector > T_PWM) 
    {
        m_vector /= (m_vector + s_vector);
        s_vector /= m_vector + s_vector;
    }

    /*--------------------计算三相桥臂作用时长-------------------------------*/
    float Ta,Tb,Tc;
    Ta = (T_PWM - (m_vector + s_vector)) / 4.0F;  
    Tb = Ta + m_vector/2.0f;
    Tc = Tb + s_vector/2.0f;

    /*-------------------------分配引信------------------------------------*/
    float Tcmp1 = 0.0f;
    float Tcmp2 = 0.0f;
    float Tcmp3 = 0.0f;
    switch (sector) {
        case 1:Tcmp1 = Tb;Tcmp2 = Ta;Tcmp3 = Tc;break;
        case 2:Tcmp1 = Ta;Tcmp2 = Tc;Tcmp3 = Tb;break;
        case 3:Tcmp1 = Ta;Tcmp2 = Tb;Tcmp3 = Tc;break;
        case 4:Tcmp1 = Tc;Tcmp2 = Tb;Tcmp3 = Ta;break;
        case 5:Tcmp1 = Tc;Tcmp2 = Ta;Tcmp3 = Tb;break;
        case 6:Tcmp1 = Tb;Tcmp2 = Tc;Tcmp3 = Ta;break;
    }

    /*-----------------------计算占空比-----------------------------*/
    float duty_a,duty_b,duty_c;
    duty_a =(T_PWM - Tcmp1*2.0f )*F_PWM;
    duty_b =(T_PWM - Tcmp2*2.0f )*F_PWM;
    duty_c =(T_PWM - Tcmp3*2.0f )*F_PWM;

    duty_t sg_duty;
    sg_duty._a = duty_a;
    sg_duty._b = duty_b;
    sg_duty._c = duty_c;
    return sg_duty;
}
#else
duty_t _svpwm(float ud,float uq,float theta)
{
    static duty_t sg_duty;
    unsigned char sector;
    float ualpha;
    float ubeta;
    float Ta,Tb,Tc;
    sector = 0;
    float s_vector = 0.0f,m_vector = 0.0f;
    dq_t u_dq;
    alpbet_t u_alphabeta;
    u_dq.d = ud;
    u_dq.q = uq;
    u_alphabeta = _2r_2s(u_dq,theta); 
    ubeta = u_alphabeta.beta;
    ualpha = u_alphabeta.alpha;
    /*--------------判断扇区-----------------*/
    if(ubeta > 0.0F) {
        sector = 1;
    }
    if((sqrt3 * ualpha - ubeta) / 2.0F > 0.0F) {
        sector += 2;
    }  
    if((-sqrt3 * ualpha - ubeta) / 2.0F > 0.0F) {
        sector += 4;
    }

    /*--------------------计算主副矢量--------------------------------*/
    switch (sector) {
        case 1:
            m_vector = (-1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC;
            s_vector = (1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC;
        break;

        case 2:
            m_vector = (1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC;
            s_vector = -(sqrt3 * ubeta * T_UDC);
        break;

        case 3:
            m_vector = -((-1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC);
            s_vector = sqrt3 * ubeta * T_UDC;
        break;

        case 4:
            m_vector = -(sqrt3 * ubeta * T_UDC);
            s_vector = (-1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC;
        break;

        case 5:
            m_vector = sqrt3 * ubeta * T_UDC;
            s_vector = -((1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC);
        break;

        default:
            m_vector = -((1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC);
            s_vector = -((-1.5F * ualpha + sqrt3/2.0f * ubeta) * T_UDC);
        break;
    }
    /*---------------------------限制矢量圆---------------------------------*/
    if (m_vector + s_vector > T_PWM) 
    {
        m_vector /= (m_vector + s_vector);
        s_vector /= m_vector + s_vector;
    }

    /*--------------------计算三相桥臂作用时长-------------------------------*/
    Ta = (T_PWM - (m_vector + s_vector)) / 4.0F;  
    Tb = Ta + m_vector/2.0f;
    Tc = Tb + s_vector/2.0f;

    /*-------------------------分配引信------------------------------------*/
    float Tcmp1 = 0.0f;
    float Tcmp2 = 0.0f;
    float Tcmp3 = 0.0f;
    switch (sector) {
        case 1:Tcmp1 = Tb;Tcmp2 = Ta;Tcmp3 = Tc;break;
        case 2:Tcmp1 = Ta;Tcmp2 = Tc;Tcmp3 = Tb;break;
        case 3:Tcmp1 = Ta;Tcmp2 = Tb;Tcmp3 = Tc;break;
        case 4:Tcmp1 = Tc;Tcmp2 = Tb;Tcmp3 = Ta;break;
        case 5:Tcmp1 = Tc;Tcmp2 = Ta;Tcmp3 = Tb;break;
        case 6:Tcmp1 = Tb;Tcmp2 = Tc;Tcmp3 = Ta;break;
    }

    /*-----------------------计算占空比-----------------------------*/
    float duty_a,duty_b,duty_c;
    duty_a =(T_PWM - Tcmp1*2 )*F_PWM;
    duty_b =(T_PWM - Tcmp2*2 )*F_PWM;
    duty_c =(T_PWM - Tcmp3*2 )*F_PWM;
    sg_duty.duty_a = duty_a;
    sg_duty.duty_b = duty_b;
    sg_duty.duty_c = duty_c;
    return sg_duty;
}
#endif

/*---------------Clark变换------------------------*/
static void _3s_2s(abc_t i_abc,alpbet_t *alp_bet)
{
    // alpbet_t i_alphabeta;
    // alp_bet->alpha = 2.0f/3.0f*(i_abc.a - 1.0f/2.0f*i_abc.b - 1.0f/2.0f*i_abc.c);
    // alp_bet->beta =  2.0f/3.0f*(sqrt3_2*i_abc.b - sqrt3_2*i_abc.c);
    alp_bet->alpha = i_abc.a;
    alp_bet->beta = sqrt3/3.0f * (i_abc.b - i_abc.c);
}
/*-------------------Park变换--------------------------------*/
static void _2s_2r(alpbet_t i_alphabeta,float theta,dq_t *dq)
{
    dq->d =  cosf(theta)*i_alphabeta.alpha + sinf(theta)*i_alphabeta.beta;
    dq->q = -sinf(theta)*i_alphabeta.alpha + cosf(theta)*i_alphabeta.beta;    
}
/*--------------反PARK变换------------------*/
 alpbet_t _2r_2s(dq_t i_dq,float theta)
{
    alpbet_t i_alphabeta;
    i_alphabeta.alpha = i_dq.d * cosf(theta) -  i_dq.q * sinf(theta);
    i_alphabeta.beta = i_dq.d * sinf(theta) +  i_dq.q * cosf(theta);
    return i_alphabeta;
}

/*-----------------------------------------*/
#if 0
static dq_t _3s_2r(abc_t i_abc, float theta)
{
    dq_t i_dq;
    float alpha,beta,rotor_theta;
    float ia, ib;
    ia = i_abc.a;
    ib = i_abc.b;
    rotor_theta =  theta;
    alpha = ia;
    beta = (2.0f*ib + ia)*0.577350269F;
    i_dq.d = alpha*cosf(rotor_theta) + beta*sinf(rotor_theta);
    i_dq.q = beta*cosf(rotor_theta) - alpha*sinf(rotor_theta);
    i_dq.theta = rotor_theta;
    return i_dq;
}
#endif
/*-------------------角度归一化-------------------------------
三目运算符。格式：condition ? expr1 : expr2 
其中，condition 是要求值的条件表达式，如果条件成立，则返回 expr1 的值，否则返回 expr2 的值。
可以将三目运算符视为 if-else 语句的简化形式。
fmod 函数的余数的符号与除数相同。因此，当 angle 的值为负数时，余数的符号将与 _2PI 的符号相反。
也就是说，如果 angle 的值小于 0 且 _2PI 的值为正数，则 fmod(angle, _2PI) 的余数将为负数。
例如，当 angle 的值为 -PI/2，_2PI 的值为 2PI 时，fmod(angle, _2PI) 将返回一个负数。
在这种情况下，可以通过将负数的余数加上 _2PI 来将角度归一化到 [0, 2PI] 的范围内，以确保角度的值始终为正数。
*/
float _normalize_angle(float angle)
{
  float a = fmod(angle, 2.0f*PI);   //取余运算可以用于归一化，列出特殊值例子算便知
  return a >= 0 ? a : (a + 2.0f*PI);  
}

/*---------------------DQ归一化------------------------------------*/
dq_t circle_limit(dq_t dq)
{
    dq_t new_vdq;
    float square_limit;
    float vd_square_limit;
    float square_q;
    float square_d;
    float square_sum;
    float square_temp;
    float new_q,new_d;

    new_vdq = dq;
    square_d = dq.d * dq.d;
    square_q = dq.q * dq.q;
    square_limit = CIRCLE_MAX_VAL * CIRCLE_MAX_VAL;
    vd_square_limit = D_MAX_VAL*D_MAX_VAL;

    square_sum = square_d + square_q;
    if (square_sum > square_limit)
    {
      if (square_d <= vd_square_limit)
      {
        square_temp = square_limit - square_d;
        new_q = sqrtf(square_temp);
        if (dq.q < 0)
        {
          new_q = -new_q;
        }
        new_d = dq.d;
      }
      else
      {
        new_d = D_MAX_VAL;
        if (dq.d < 0)
        {
          new_d = -new_d;
        }
        square_temp = square_limit - vd_square_limit;
        new_q = sqrtf(square_temp);
        if (dq.q < 0)
        {
          new_q = - new_q;
        }
      }
      new_vdq.q = new_q;
      new_vdq.d = new_d;
    }

    return new_vdq;
}


/*----------------机械找零---------------------*/

void mechie_zero(float theta)
{
    dq_t udq;
    alpbet_t ualpbeta;
    // do
    // {
    //     // ualpbeta = _2r_2s(udq,theta);
    //     _svpwm(1,0);

    // } while (theta >= 0.02f);
    _svpwm(2,0);
}

