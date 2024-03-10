#include "stdint.h"
#include "motorctrl_common.h"

#define SQRT3_Q15            (56756)
#define T_PWM_Q15            (32768)
#define T_UDC_Q15            (1365) 

duty_t _svpwm_Q(int32_t ualpha,int32_t ubeta)
{
    uint8_t sector;
    sector = 0;
    if (ubeta > 0)
    {
        sector = 1;
    }
    if ((((SQRT3_Q15*ualpha)>>15) - ubeta)>>1 > 0)
    {
        sector += 2;
    }
    if ((((-SQRT3_Q15*ualpha)>>15) - ubeta)>>1 > 0)
    {
        sector += 4;
    }

    int32_t s_vector = 0,m_vector = 0;
    int32_t X = 0,Y = 0,Z = 0;
    X = _IQ15mpy(_IQ15mpy(SQRT3_Q15,ubeta),T_UDC_Q15);
    Y = _IQ15mpy(_IQ15mpy(_IQ15(1.5),ualpha) + _IQ15mpy((SQRT3_Q15>>1),ubeta),T_UDC_Q15);
    Z = _IQ15mpy(_IQ15mpy(_IQ15(-1.5),ualpha) + _IQ15mpy((SQRT3_Q15>>1),ubeta),T_UDC_Q15);
   switch (sector) {
        case 1: m_vector = Z;s_vector = Y ;break;
        case 2:m_vector = Y ;s_vector = -X;break;
        case 3:m_vector = -Z;s_vector = X ;break;
        case 4:m_vector = -X;s_vector = Z ;break;
        case 5:m_vector = X ;s_vector = -Y;break;
        case 6:m_vector = -Y;s_vector = -Z;break;
        default:
        break;
    }
    if (m_vector + s_vector > T_PWM_Q15) 
    {
        m_vector /= (m_vector + s_vector);
        s_vector /= m_vector + s_vector;
    }
    int32_t Ta,Tb,Tc;
    Ta = (T_PWM_Q15-m_vector-s_vector) >> 2;  
    Tb = Ta + (m_vector>>1);
    Tc = Tb + (s_vector>>1);

    int32_t Tcmp1,Tcmp2,Tcmp3;
    switch (sector) {
        case 1:Tcmp1 = Tb;Tcmp2 = Ta;Tcmp3 = Tc;break;
        case 2:Tcmp1 = Ta;Tcmp2 = Tc;Tcmp3 = Tb;break;
        case 3:Tcmp1 = Ta;Tcmp2 = Tb;Tcmp3 = Tc;break;
        case 4:Tcmp1 = Tc;Tcmp2 = Tb;Tcmp3 = Ta;break;
        case 5:Tcmp1 = Tc;Tcmp2 = Ta;Tcmp3 = Tb;break;
        case 6:Tcmp1 = Tb;Tcmp2 = Tc;Tcmp3 = Ta;break;
        default:break;
    }
    duty_t sg_duty;
    sg_duty._a = (float)(T_PWM_Q15 - (Tcmp1<<1) )/(float)T_PWM_Q15;
    sg_duty._b = (float)(T_PWM_Q15 - (Tcmp2<<1) )/(float)T_PWM_Q15;
    sg_duty._c = (float)(T_PWM_Q15 - (Tcmp3<<1) )/(float)T_PWM_Q15;
    return sg_duty;
}

void _3s_2s_Q(abc_t i_abc,alpbet_t *alp_bet)
{
    alp_bet->Q_alpha = i_abc.Q_a;
    alp_bet->Q_beta = _IQ15mpy(_IQ15div(SQRT3_Q15,_IQ15(3.0f)),(i_abc.Q_b - i_abc.Q_c));
}

alpbet_t _2r_2s_Q(dq_t i_dq,int32_t theta)
{
    alpbet_t i_alphabeta;
    int32_t cos_theta,sin_theta;
    int32_t theta_q15;
    theta_q15 = theta;
    sin_theta = _IQ15sin(theta_q15);
    cos_theta = _IQ15cos(theta_q15);

    i_alphabeta.Q_alpha = _IQ15mpy(i_dq.Q_d , cos_theta) -  _IQ15mpy(i_dq.Q_q,sin_theta);
    i_alphabeta.Q_beta = _IQ15mpy(i_dq.Q_d , sin_theta) +  _IQ15mpy(i_dq.Q_q,cos_theta);
    return i_alphabeta;
}

void _2s_2r_Q(alpbet_t i_alphabeta,int32_t theta,dq_t *dq)
{
    int32_t cos_theta,sin_theta;
    int32_t theta_q15;
    theta_q15 = (theta);
    sin_theta = _IQ15sin(theta_q15);
    cos_theta = _IQ15cos(theta_q15);
    dq->Q_d =  _IQ15mpy(cos_theta,i_alphabeta.Q_alpha) + _IQ15mpy(sin_theta,i_alphabeta.Q_beta);
    dq->Q_q = _IQ15mpy(-sin_theta,i_alphabeta.Q_alpha) + _IQ15mpy(cos_theta,i_alphabeta.Q_beta);    
}

