#include "./motorctrl_common.h"
#include "debuglog.h"

static dq_t circle_limit(dq_t dq);



duty_t _svpwm(float ualpha,float ubeta)
{
    unsigned char sector;    
    sector = 0;
    /*-------------------------------*/
    if(ubeta > 0.0F) {
        sector = 1;
    }
    if((sqrt3 * ualpha - ubeta) / 2.0F > 0.0F) {
        sector += 2;
    }
    if((-sqrt3 * ualpha - ubeta) / 2.0F > 0.0F) {
        sector += 4;
    }
    /*----------------------------------------------------*/
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

    /*------------------------------------------------------------*/
    if (m_vector + s_vector > T_PWM) 
    {
        m_vector /= (m_vector + s_vector);
        s_vector /= m_vector + s_vector;
    }

    /*---------------------------------------------------*/
    float Ta,Tb,Tc;
    Ta = (T_PWM - (m_vector + s_vector)) / 4.0F;  
    Tb = Ta + m_vector/2.0f;
    Tc = Tb + s_vector/2.0f;

    /*------------------------换相点-------------------------------------*/
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
    /*-------------------------占空比---------------------------*/
    float duty_a,duty_b,duty_c;
    duty_a =(T_PWM - Tcmp1*2.0f )/T_PWM;
    duty_b =(T_PWM - Tcmp2*2.0f )/T_PWM;
    duty_c =(T_PWM - Tcmp3*2.0f )/T_PWM;

    duty_t sg_duty;
    sg_duty._a = duty_a;
    sg_duty._b = duty_b;
    sg_duty._c = duty_c;

    return sg_duty;
}


/*---------------Clark�任------------------------*/
 void _3s_2s(abc_t i_abc,alpbet_t *alp_bet)
{
    alpbet_t i_alphabeta;
    alp_bet->alpha = 2.0f/3.0f*(i_abc.a - 1.0f/2.0f*i_abc.b - 1.0f/2.0f*i_abc.c);
    alp_bet->beta =  2.0f/3.0f*(sqrt3_2*i_abc.b - sqrt3_2*i_abc.c);
    // alp_bet->alpha = i_abc.a;
    // alp_bet->beta = sqrt3/3.0f * (i_abc.b - i_abc.c);
}
/*-------------------Park�任--------------------------------*/
 void _2s_2r(alpbet_t i_alphabeta,float theta,dq_t *dq)
{
    dq->d =  cosf(theta)*i_alphabeta.alpha + sinf(theta)*i_alphabeta.beta;
    dq->q = -sinf(theta)*i_alphabeta.alpha + cosf(theta)*i_alphabeta.beta;    
}
/*--------------��PARK�任------------------*/
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

float _normalize_angle(float angle)
{
  float a = fmod(angle, _2PI);
  return a >= 0 ? a : (a + 2.0f*PI);  
}

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



