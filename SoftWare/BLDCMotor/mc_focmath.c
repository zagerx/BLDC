/*==========================================================================================
 * @file mc_focmath.c
 * @author zager
 * @version      0.1
 * @date 2025-01-04
 * 
 * @copyright Copyright (c) 2025
 * 
 * @brief 
--------------------------------------------------------------------------------------------*/
#include "./motorctrl_common.h"
#include "debuglog.h"
#include "mc_utils.h"

#if 1
/*==========================================================================================
 * @brief        svpwm函数
 * @FuncName     SVM
 * @param        alpha 
 * @param        beta 
 * @return       duty_t 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void SVM(float alpha, float beta,float *duty) 
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
	duty[0] = tA;
	duty[1] = tB;
	duty[2] = tC;
}

#else

duty_t SVM(float ualpha,float ubeta)
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
#endif

/*==========================================================================================
 * @brief        Clark变换
 * @FuncName     _3s_2s
 * @param        abc 
 * @param        alpbet 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void _3s_2s(float* abc,float *alpbet)
{
    // alp_bet[0] = 2.0f/3.0f*(i_abc[0] - 1.0f/2.0f*i_abc[1] - 1.0f/2.0f*i_abc[2]);
    // alp_bet[1] =  2.0f/3.0f*(sqrt3_2*i_abc[1] - sqrt3_2*i_abc[2]);
    alpbet[0] = abc[0];
    alpbet[1] = sqrt3/3.0f * (abc[1] - abc[2]);
}
/*==========================================================================================
 * @brief        Park变换
 * @FuncName     _2s_2r
 * @param        alpbet 
 * @param        theta 
 * @param        dq 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void _2s_2r(float* alpbet,float theta,float *dq)
{
    dq[0] =  cosf(theta)*alpbet[0] + sinf(theta)*alpbet[1];
    dq[1] = -sinf(theta)*alpbet[0] + cosf(theta)*alpbet[1];    
}
/*==========================================================================================
 * @brief        反Park变换
 * @FuncName     _2r_2s
 * @param        dq 
 * @param        theta 
 * @param        alpbet 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void _2r_2s(float* dq,float theta,float* alpbet)
{
    alpbet[0] = dq[0] * cosf(theta) -  dq[1] * sinf(theta);
    alpbet[1] = dq[0] * sinf(theta) +  dq[1] * cosf(theta);
}
/*==========================================================================================
 * @brief        数据归一化
 * @FuncName     _normalize_angle
 * @param        angle 
 * @return       float 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
float _normalize_angle(float angle)
{
  float a = fmod(angle, _2PI);
  return a >= 0 ? a : (a + 2.0f*PI);  
}
/*==========================================================================================
 * @brief        限制矢量圆
 * @FuncName     _limit_vector_circle
 * @param        dq 
 * @param        limit_dq 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void _limit_vector_circle(float *dq,float *limit_dq)
{
    /*TODO */
    float mod_to_V = (2.0f / 3.0f) * VOLATAGE_BUS;
    float V_to_mod = 1.0f / mod_to_V;
    float mod_d = V_to_mod * dq[0];
    float mod_q = V_to_mod * dq[1];
    float mod_scalefactor = 0.80f * 0.86602540378f * 1.0f / sqrtf(mod_d * mod_d + mod_q * mod_q);//#define SQRT_3__2    0.86602540378f
    if (mod_scalefactor < 1.0f)
    {
        mod_d *= mod_scalefactor;
        mod_q *= mod_scalefactor;
    }
    limit_dq[0] = mod_d;
    limit_dq[1] = mod_q;

    limit_dq[0] = dq[0];
    limit_dq[1] = dq[1];
}

