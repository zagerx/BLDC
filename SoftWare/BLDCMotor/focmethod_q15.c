#include "_common.h"
#include "stdint.h"
#include "focmethod.h"
 

#define SQRT3_Q15 _IQ15(1.732050808f)
#define T_PWM_Q15 _IQ15(0.01f)
#define T_UDC_Q15 _IQ15(0.01f/24.0f)
int32_t test;
int32_t test_alpha,test_beta;
uint8_t test_section_q15;
int32_t test_m,test_s;
duty_t _svpwm_q15(int32_t ualpha,int32_t ubeta)
{
    uint8_t sector;
    sector = 0;
    if (ubeta > 0)
    {
        sector = 1;
    }
    if ((((((SQRT3_Q15*ualpha)>>15) - ubeta)>>1)) > 0)
    {
        sector += 2;
    }
    if ((((((-SQRT3_Q15*ualpha)>>15) - ubeta)>>1)) > 0)
    {
        sector += 4;
    }

    int32_t s_vector,m_vector;
   switch (sector) {
        case 1:
            m_vector = (_IQ15(-1.5) * ualpha + (SQRT3_Q15>>1) * ubeta) * T_UDC_Q15;
            s_vector = (_IQ15(1.5) * ualpha + (SQRT3_Q15>>1) * ubeta) * T_UDC_Q15;
        break;

        case 2:
            m_vector = (_IQ15(1.5) * ualpha + (SQRT3_Q15>>1) * ubeta) * T_UDC_Q15;
            s_vector = -(SQRT3_Q15 * ubeta * T_UDC_Q15);
        break;

        case 3:
            m_vector = -((-_IQ15(1.5) * ualpha + (SQRT3_Q15>>1) * ubeta) * T_UDC_Q15);
            s_vector = SQRT3_Q15 * ubeta * T_UDC_Q15;
        break;

        case 4:
            m_vector = -(SQRT3_Q15 * ubeta * T_UDC_Q15);
            s_vector = (-_IQ15(1.5) * ualpha + (SQRT3_Q15>>1) * ubeta) * T_UDC_Q15;
        break;

        case 5:
            m_vector = SQRT3_Q15 * ubeta * T_UDC_Q15;
            s_vector = -((_IQ15(1.5) * ualpha + (SQRT3_Q15>>1) * ubeta) * T_UDC_Q15);
        break;

        default:
            m_vector = -((_IQ15(1.5) * ualpha + (SQRT3_Q15>>1) * ubeta) * T_UDC_Q15);
            s_vector = -((-_IQ15(1.5) * ualpha + (SQRT3_Q15>>1) * ubeta) * T_UDC_Q15);
        break;
    }
    test_m = m_vector;
    test_s = s_vector;
    if (m_vector + s_vector > T_PWM_Q15) 
    {
        m_vector /= (m_vector + s_vector);
        s_vector /= m_vector + s_vector;
    }

    int32_t Ta,Tb,Tc;
    Ta = ((T_PWM_Q15 - (m_vector + s_vector)>> 2));  
    Tb = Ta + (m_vector>>1);
    Tc = Tb + (s_vector>>1);

    int32_t Tcmp1 ;
    int32_t Tcmp2 ;
    int32_t Tcmp3 ;
    switch (sector) {
        case 1:Tcmp1 = Tb;Tcmp2 = Ta;Tcmp3 = Tc;break;
        case 2:Tcmp1 = Ta;Tcmp2 = Tc;Tcmp3 = Tb;break;
        case 3:Tcmp1 = Ta;Tcmp2 = Tb;Tcmp3 = Tc;break;
        case 4:Tcmp1 = Tc;Tcmp2 = Tb;Tcmp3 = Ta;break;
        case 5:Tcmp1 = Tc;Tcmp2 = Ta;Tcmp3 = Tb;break;
        case 6:Tcmp1 = Tb;Tcmp2 = Tc;Tcmp3 = Ta;break;
    }
    ipc_write_data(PUBLIC_DATA_IQ,Tcmp1/1000);
    // float duty_a,duty_b,duty_c;
    // duty_a =(float)(T_PWM_Q15 - Tcmp1<<1 )/(float)T_PWM_Q15;
    // duty_b =(float)(T_PWM_Q15 - Tcmp2<<1 )/(float)T_PWM_Q15;
    // duty_c =(float)(T_PWM_Q15 - Tcmp3<<1 )/(float)T_PWM_Q15;
    duty_t sg_duty;
    // sg_duty._a = duty_a;
    // sg_duty._b = duty_b;
    // sg_duty._c = duty_c;
    return sg_duty;
}