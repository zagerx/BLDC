#include "mc_torquemode.h"
#include "mc_utils.h"



static float _limit_torque(float tar);
static float motor_max_available_torque(void);
static float limitVel(float vel_limit, float vel_estimate, float vel_gain, float torque);

extern float vel_estimate_;          //当前估算转速，单位[turn/s]

#define TORQUE_COMSTANT                  0.04F
#define  VELOCITY_P                      0.02f              //速度P参数
#define  VELOCITY_I                      0.2f    
#define  VELOCITY_LIMIT                  50                 //最大速度限制，力矩模式超过限速力矩会下降

dq_t mc_update_torque(float tar)
{
    float torque = 0.0f;
	torque = (1) * _limit_torque(tar);

	float id = 0;
	float iq = 0;
	float ilim = 30.0f;
	id = clamp(id, -ilim*0.99f, ilim*0.99f);                 // 1% space reserved for Iq to avoid numerical issues
	iq = torque / TORQUE_COMSTANT;
	float iq_lim_sqr = SQ(ilim) - SQ(id);
	float Iq_lim = (iq_lim_sqr <= 0.0f) ? 0.0f : sqrt(iq_lim_sqr);
	iq = clamp(iq, -Iq_lim, Iq_lim);

	dq_t idq = {0};
	idq.d = id;
	idq.q = iq;
	return idq;
}

static float _limit_torque(float tar)
{

	float *vel_estimate;
	float torque = 0.0f;
		
	vel_estimate = &vel_estimate_;
	
	torque = tar; 
	const float Tlim = motor_max_available_torque();   //max_torque = effective_current_lim_ * config_.torque_constant = 60*0.04f = 2.4f;
	torque = clamp(torque, -Tlim, Tlim);

  	float vel_gain = VELOCITY_P;
	{
		torque = limitVel(VELOCITY_LIMIT, *vel_estimate, vel_gain, torque);
	}

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

static float motor_max_available_torque(void)
{
	return 30.0f * TORQUE_COMSTANT;
}

static float limitVel(float vel_limit, float vel_estimate, float vel_gain, float torque)
{
	float Tmax = (vel_limit - vel_estimate) * vel_gain;
	float Tmin = (-vel_limit - vel_estimate) * vel_gain;
	return clamp(torque, Tmin, Tmax);
}
