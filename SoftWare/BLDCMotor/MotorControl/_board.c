
// #include "MyProject.h"


#include "stm32f4xx.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "utils.h"
#include "arm_cos_f32.h"
#include "board.h"
#include "sensor.h"
#include "motorctrl_common.h"
#include "tim.h"
static mt_param_t sgmc_param = {0};
extern float vel_estimate_    ;          //当前估算转速，单位[turn/s]
extern float encoder_elespeed ;
extern float encoder_eletheta ;
#define  CURRENT_SENSE_MIN_VOLT  0.3f
#define  CURRENT_SENSE_MAX_VOLT  3.0f
#define  CURRENT_ADC_LOWER_BOUND  (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MIN_VOLT / 3.3f)
#define  CURRENT_ADC_UPPER_BOUND  (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MAX_VOLT / 3.3f)

static float  vbus_voltage=24.0f;

// abc_t
static float  Ialpha_beta[2];
static alpbet_t  motor_Ialphabeta;
// static dq_t  motor_Idq;

static float  pi_gains_[2];
static float  v_current_control_integral_d = 0.0f; // [V]
static float  v_current_control_integral_q = 0.0f; // [V]
static float effective_current_lim_ = 10.0f; // [A]
static float max_allowed_current_ = 0.0f;    // [A] set in setup()


float input_torque_ = 0.05f;  // [Nm]

void arm(void);
void disarm(void);

#define enable_torque_mode_vel_limit     (1)
#define TORQUE_COMSTANT                  0.04F
#define  VELOCITY_P                      0.02f              //速度P参数
#define  VELOCITY_I                      0.2f               //速度I参数
#define  VELOCITY_limit                  50                 //最大速度限制，力矩模式超过限速力矩会下降
#define CURRMENT_MEAS_PERIOD             0.000125f
#define VBUS_S_DIVIDER_RATIO             18.73f    //电源分压电阻2.2k+39k
#define SHUNT_RESISTANCE                 0.001f    //采样电阻，如果是0.5mΩ=0.0005f,1mΩ=0.001f
#define PHASE_CURRENT_GAIN               20.0f     //电流采样运放倍数，20倍
#define TIM_PERIOD                       3500
static float limitVel(float vel_limit, float vel_estimate, float vel_gain, float torque)
{
	float Tmax = (vel_limit - vel_estimate) * vel_gain;
	float Tmin = (-vel_limit - vel_estimate) * vel_gain;
	return clamp(torque, Tmin, Tmax);
}
static float motor_max_available_torque(void)
{
	return 30.0f * TORQUE_COMSTANT;
}
static float torqueMode_limittorque(void)
{

	float *vel_estimate;
	float torque = 0.0f;
		
	vel_estimate = &vel_estimate_;
	
	if (!has_value(vel_estimate))
		return 0;
	torque = input_torque_; 
	// Never command a setpoint beyond its limit
	const float Tlim = motor_max_available_torque();   //max_torque = effective_current_lim_ * config_.torque_constant = 60*0.04f = 2.4f;
	torque = clamp(torque, -Tlim, Tlim);

  	float vel_gain = VELOCITY_P;
	// Velocity limiting in current mode
	{
		torque = limitVel(VELOCITY_limit, *vel_estimate, vel_gain, torque);
	}

	// Torque limiting
	if (torque > Tlim)
	{
		torque = Tlim;
	}
	if (torque < -Tlim)
	{
		torque = -Tlim;
	}
	return torque;
}

static void foc_reset(void)
{
	v_current_control_integral_d = 0.0f;  //积分清零，否则闭环时电机误动作
	v_current_control_integral_q = 0.0f;
	pid_reset(&(sgmc_param.daxis_pi));
	pid_reset(&(sgmc_param.qaxis_pi));
}

 void motor_para_init(void)
{
	pi_gains_[0] = 0.0273f;
	pi_gains_[1] = 166.2507f;
	effective_current_lim_ = 30.0f;
	max_allowed_current_ = 60.75f;
	
    pid_init(&(sgmc_param.daxis_pi),0.0273f,166.2507f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(sgmc_param.qaxis_pi),0.0273f,166.2507f,1.0f,D_MAX_VAL,D_MIN_VAL);
}

 void arm(void)
{
	foc_reset();

}




static float phase_current_from_adcval(uint32_t ADCValue)
{
	if (ADCValue < CURRENT_ADC_LOWER_BOUND || ADCValue > CURRENT_ADC_UPPER_BOUND)
	{
		return 0;
	}
	
	int adcval_bal = (int)ADCValue - (1 << 11);
	float amp_out_volt = (3.3f / (float)(1 << 12)) * (float)adcval_bal;
	float shunt_volt = amp_out_volt * (1/PHASE_CURRENT_GAIN);   //phase_current_rev_gain_ = 1/20倍放大
	float current = shunt_volt * (1/SHUNT_RESISTANCE);          //shunt_conductance_ = 1/0.001采样电阻;
	return current;
}
static void vbus_sense_adc_cb(uint32_t adc_value) 
{
	float voltage_scale = 3.3f * VBUS_S_DIVIDER_RATIO / 4096;
	vbus_voltage = adc_value * voltage_scale;
	vbus_voltage = 24.0F;
}



static dq_t torqueMode_limitIq(float torque)
{
	dq_t idq = {0};
	torque = (1) * torque;
	
	// Load setpoints from previous iteration.
	float id = 0;
	float iq = 0;
	
	float ilim = 30.0f;
	
	id = clamp(id, -ilim*0.99f, ilim*0.99f); // 1% space reserved for Iq to avoid numerical issues
	iq = torque / TORQUE_COMSTANT;
	
	// 2-norm clamping where Id takes priority
	float iq_lim_sqr = SQ(ilim) - SQ(id);
	float Iq_lim = (iq_lim_sqr <= 0.0f) ? 0.0f : sqrt(iq_lim_sqr);
	iq = clamp(iq, -Iq_lim, Iq_lim);
	
	idq.d = id;
	idq.q = iq;
	return idq;
}


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


static bool FOC_current(float Id_des, float Iq_des,abc_t iabc)   //I_phase是park变换时的θ，pwm_phase是park逆变换时的θ
{
	float I_phase = encoder_eletheta;
	float pwm_phase = encoder_eletheta + 1.5f * CURRMENT_MEAS_PERIOD * encoder_elespeed;  //0.5.6是在反park变换时计算，效果一样

	_3s_2s(iabc,&motor_Ialphabeta);
	dq_t idq = {0};
	_2s_2r(motor_Ialphabeta,I_phase,&idq);

	float Ierr_d = Id_des - idq.d;
	float Ierr_q = Iq_des - idq.q;
	motordebug.id_targe = Id_des;
	motordebug.id_real  = idq.d;
	motordebug.iq_targe = Iq_des;
	motordebug.iq_real  = idq.q;


	// // TODO look into feed forward terms (esp omega, since PI pole maps to RL tau)
	// // Apply PI control
	// float Vd = v_current_control_integral_d + Ierr_d * pi_gains_[0];
	// float Vq = v_current_control_integral_q + Ierr_q * pi_gains_[0];

	// float mod_to_V = (2.0f / 3.0f) * vbus_voltage;
	// float V_to_mod = 1.0f / mod_to_V;
	// float mod_d = V_to_mod * Vd;
	// float mod_q = V_to_mod * Vq;
	// float mod_scalefactor = 0.80f * sqrt3_by_2 * 1.0f / sqrtf(mod_d * mod_d + mod_q * mod_q);
	// if (mod_scalefactor < 1.0f)
	// {
	// 	mod_d *= mod_scalefactor;
	// 	mod_q *= mod_scalefactor;
	// 	v_current_control_integral_d *= 0.99f;
	// 	v_current_control_integral_q *= 0.99f;
	// }
	// else
	// {
	// 	v_current_control_integral_d += Ierr_d * (pi_gains_[1] * CURRMENT_MEAS_PERIOD);
	// 	v_current_control_integral_q += Ierr_q * (pi_gains_[1] * CURRMENT_MEAS_PERIOD);
	// }

	float Vd = pid_contrl(&(sgmc_param.daxis_pi),Id_des,idq.d);
	float Vq = pid_contrl(&(sgmc_param.qaxis_pi),Iq_des,idq.q);

	float mod_to_V = (2.0f / 3.0f) * vbus_voltage;
	float V_to_mod = 1.0f / mod_to_V;
	float mod_d = V_to_mod * Vd;
	float mod_q = V_to_mod * Vq;
	float mod_scalefactor = 0.80f * sqrt3_by_2 * 1.0f / sqrtf(mod_d * mod_d + mod_q * mod_q);
	if (mod_scalefactor < 1.0f)
	{
		mod_d *= mod_scalefactor;
		mod_q *= mod_scalefactor;
	}

	// Inverse park transform
	idq.d = mod_d;
	idq.q = mod_q;
	alpbet_t temp_ab = {0};
	temp_ab = _2r_2s(idq,pwm_phase);

	// Apply SVM
	duty_t duty = {0};
	duty = SVM(temp_ab.alpha,temp_ab.beta);

	tim_set_pwm(duty._a,duty._b,duty._c);
	return true;
}

void _convert_current(abc_t *current,uint16_t ADC_A,uint16_t ADC_B,uint16_t ADC_C)
{  
	current->b = phase_current_from_adcval(ADC_B);
	current->c = phase_current_from_adcval(ADC_C);
	current->a = -current->b - current->c;

	current->a = current->a - 0.0f;//DC_calib_.phA;
	current->b = current->b - (-0.2f);//DC_calib_.phB;
	current->c = current->c - 0.2f;//DC_calib_.phC;	
}


void __loopcurrment(uint16_t a,uint16_t b,uint16_t c)
{
	((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_COV))[0];

	dq_t idq;
	idq = torqueMode_limitIq(torqueMode_limittorque());//更新iq限制值
	abc_t  motor_Iabc;
	_convert_current(&motor_Iabc,0,b,c);
	FOC_current(idq.d, idq.q,motor_Iabc);  //highcurrent电机用电流模式	
}




