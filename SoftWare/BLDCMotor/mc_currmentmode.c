#include "mc_currmentmode.h"

#include "stdbool.h"
#include "utils.h"
#include "tim.h"

#define  CURRENT_SENSE_MIN_VOLT           0.3f
#define  CURRENT_SENSE_MAX_VOLT           3.0f
#define  SHUNT_RESISTANCE                 0.001f    //采样电阻，如果是0.5mΩ=0.0005f,1mΩ=0.001f
#define  PHASE_CURRENT_GAIN               20.0f     //电流采样运放倍数，20倍
#define  CURRENT_ADC_LOWER_BOUND          (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MIN_VOLT / 3.3f)
#define  CURRENT_ADC_UPPER_BOUND          (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MAX_VOLT / 3.3f)
#define  TIM_PERIOD                       3500
#define  CURRMENT_MEAS_PERIOD             0.000125f
extern float vel_estimate_    ;          //当前估算转速，单位[turn/s]
extern float encoder_elespeed ;
extern float encoder_eletheta ;
extern mc_param_t sgmc_param ;
extern float  vbus_voltage;


static void _convert_current(abc_t *current,uint16_t ADC_A,uint16_t ADC_B,uint16_t ADC_C);
static abc_t convert_current(uint16_t adc_A,uint16_t adc_B,uint16_t adc_C);
static  float phase_current_from_adcval(uint32_t ADCValue);
static bool FOC_current(float Id_des, float Iq_des,abc_t iabc);


void currment_loop(uint16_t a,uint16_t b,uint16_t c,float theta,float pre_theta)
{
    ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_COV))[0];
    abc_t phasecurrment = {0};
    // phasecurrment = convert_current(a,b,c);
    _convert_current(&phasecurrment,a,b,c);
    FOC_current(motordebug.id_targe, motordebug.iq_targe,phasecurrment);

}

void currment_loop_paraminit(pid_cb_t *pid)
{
	/*pid 参数*/
}

void currment_loop_paramdeinit(pid_cb_t *pid)
{
	/*pid 反向初始化参数*/
}


static bool FOC_current(float Id_des, float Iq_des,abc_t iabc)   //I_phase是park变换时的θ，pwm_phase是park逆变换时的θ
{
	float I_phase = encoder_eletheta;
	float pwm_phase = encoder_eletheta + 1.5f * CURRMENT_MEAS_PERIOD * encoder_elespeed;  //0.5.6是在反park变换时计算，效果一样

    alpbet_t motor_Ialphabeta;
	_3s_2s(iabc,&motor_Ialphabeta);
	dq_t idq = {0};
	_2s_2r(motor_Ialphabeta,I_phase,&idq);

	float Ierr_d = Id_des - idq.d;
	float Ierr_q = Iq_des - idq.q;

	motordebug.id_targe = Id_des;
	motordebug.id_real  = idq.d;
	motordebug.iq_targe = Iq_des;
	motordebug.iq_real  = idq.q;

	// TODO look into feed forward terms (esp omega, since PI pole maps to RL tau)
	// Apply PI control
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






static abc_t convert_current(uint16_t adc_A,uint16_t adc_B,uint16_t adc_C)
{
    abc_t phasecurrent;
	if (adc_B < CURRENT_ADC_LOWER_BOUND || adc_B > CURRENT_ADC_UPPER_BOUND ||\
		adc_C < CURRENT_ADC_LOWER_BOUND || adc_C > CURRENT_ADC_UPPER_BOUND)
	{
		phasecurrent.b = 0.0f;
		phasecurrent.c = 0.0f;
	}else{
		phasecurrent.b  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_A - (1 << 11)) * (1/PHASE_CURRENT_GAIN)) * (1/SHUNT_RESISTANCE);          //shunt_conductance_ = 1/0.001采样电阻;
		phasecurrent.c  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_C - (1 << 11)) * (1/PHASE_CURRENT_GAIN)) * (1/SHUNT_RESISTANCE);
	}
	phasecurrent.a = -phasecurrent.b - phasecurrent.c;    // phasecurrent.a = -phasecurrent.b-phasecurrent.c;

    phasecurrent.b  += 0.2f;
    phasecurrent.c  -= 0.2f;
    phasecurrent.a  -= 0.0f;
    return phasecurrent;
}

static void _convert_current(abc_t *current,uint16_t ADC_A,uint16_t ADC_B,uint16_t ADC_C)
{
	current->b = phase_current_from_adcval(ADC_B);
	current->c = phase_current_from_adcval(ADC_C);
	current->a = -current->b - current->c;

	current->a = current->a - 0.0f;//DC_calib_.phA;
	current->b = current->b - (-0.2f);//DC_calib_.phB;
	current->c = current->c - 0.2f;//DC_calib_.phC;	
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