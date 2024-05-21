#include "./motorctrl_common.h"
#include "debuglog.h"
#include "mc_utils.h"
static dq_t circle_limit(dq_t dq);










duty_t SVM(float alpha, float beta) 
{
	float tA,tB,tC;
    int Sextant;

    if (beta >= 0.0f) {
        if (alpha >= 0.0f) {
            //quadrant I
            if (one_by_sqrt3 * beta > alpha)
                Sextant = 2; //sextant v2-v3
            else
                Sextant = 1; //sextant v1-v2

        } else {
            //quadrant II
            if (-one_by_sqrt3 * beta > alpha)
                Sextant = 3; //sextant v3-v4
            else
                Sextant = 2; //sextant v2-v3
        }
    } else {
        if (alpha >= 0.0f) {
            //quadrant IV
            if (-one_by_sqrt3 * beta > alpha)
                Sextant = 5; //sextant v5-v6
            else
                Sextant = 6; //sextant v6-v1
        } else {
            //quadrant III
            if (one_by_sqrt3 * beta > alpha)
                Sextant = 4; //sextant v4-v5
            else
                Sextant = 5; //sextant v5-v6
        }
    }

    switch (Sextant) {
        // sextant v1-v2
        case 1: {
            // Vector on-times
            float t1 = alpha - one_by_sqrt3 * beta;
            float t2 = two_by_sqrt3 * beta;

            // PWM timings
            tA = (1.0f - t1 - t2) * 0.5f;
            tB = tA + t1;
            tC = tB + t2;
        } break;

        // sextant v2-v3
        case 2: {
            // Vector on-times
            float t2 = alpha + one_by_sqrt3 * beta;
            float t3 = -alpha + one_by_sqrt3 * beta;

            // PWM timings
            tB = (1.0f - t2 - t3) * 0.5f;
            tA = tB + t3;
            tC = tA + t2;
        } break;

        // sextant v3-v4
        case 3: {
            // Vector on-times
            float t3 = two_by_sqrt3 * beta;
            float t4 = -alpha - one_by_sqrt3 * beta;

            // PWM timings
            tB = (1.0f - t3 - t4) * 0.5f;
            tC = tB + t3;
            tA = tC + t4;
        } break;

        // sextant v4-v5
        case 4: {
            // Vector on-times
            float t4 = -alpha + one_by_sqrt3 * beta;
            float t5 = -two_by_sqrt3 * beta;

            // PWM timings
            tC = (1.0f - t4 - t5) * 0.5f;
            tB = tC + t5;
            tA = tB + t4;
        } break;

        // sextant v5-v6
        case 5: {
            // Vector on-times
            float t5 = -alpha - one_by_sqrt3 * beta;
            float t6 = alpha - one_by_sqrt3 * beta;

            // PWM timings
            tC = (1.0f - t5 - t6) * 0.5f;
            tA = tC + t5;
            tB = tA + t6;
        } break;

        // sextant v6-v1
        case 6: {
            // Vector on-times
            float t6 = -two_by_sqrt3 * beta;
            float t1 = alpha + one_by_sqrt3 * beta;

            // PWM timings
            tA = (1.0f - t6 - t1) * 0.5f;
            tC = tA + t1;
            tB = tC + t6;
        } break;
    }

    // if any of the results becomes NaN, result_valid will evaluate to false
    int result_valid =
            tA >= 0.0f && tA <= 1.0f
         && tB >= 0.0f && tB <= 1.0f
         && tC >= 0.0f && tC <= 1.0f;
	duty_t duty;
	duty._a = tA;
	duty._b = tB;
	duty._c = tC;
    return duty ;
}




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

    static duty_t sg_duty;
    sg_duty._a = duty_a;
    sg_duty._b = duty_b;
    sg_duty._c = duty_c;

    return sg_duty;
}


 void _3s_2s(abc_t i_abc,alpbet_t *alp_bet)
{
    alpbet_t i_alphabeta;
    // alp_bet->alpha = 2.0f/3.0f*(i_abc.a - 1.0f/2.0f*i_abc.b - 1.0f/2.0f*i_abc.c);
    // alp_bet->beta =  2.0f/3.0f*(sqrt3_2*i_abc.b - sqrt3_2*i_abc.c);
    alp_bet->alpha = i_abc.a;
    alp_bet->beta = sqrt3/3.0f * (i_abc.b - i_abc.c);
}
 void _2s_2r(alpbet_t i_alphabeta,float theta,dq_t *dq)
{
    dq->d =  cosf(theta)*i_alphabeta.alpha + sinf(theta)*i_alphabeta.beta;
    dq->q = -sinf(theta)*i_alphabeta.alpha + cosf(theta)*i_alphabeta.beta;    
}
 alpbet_t _2r_2s(dq_t i_dq,float theta)
{
    alpbet_t i_alphabeta;
    i_alphabeta.alpha = i_dq.d * cosf(theta) -  i_dq.q * sinf(theta);
    i_alphabeta.beta = i_dq.d * sinf(theta) +  i_dq.q * cosf(theta);
    return i_alphabeta;
}


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



