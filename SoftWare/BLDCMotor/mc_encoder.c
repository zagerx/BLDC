#include "mc_encoder.h"
#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include "mc_utils.h"


#ifndef ENCODER_TYPE_HALL    
	#include "board.h"
	#include "sensor.h"
static void Absolute_encoder(mc_encoder_t *encoder);
static void Absolute_encoder(mc_encoder_t *encoder)
{
	uint32_t data = *((uint32_t*)sensor_user_read(SENSOR_01));
	encoder->raw_data = data;
    float mec_theta = data * ENCODER_CPR - MEC_ANGLE_OFFSET;
    float ele_theta = mec_theta * MOTOR_PAIRS;
    encoder->ele_theta = wrap_pm_pi(ele_theta);
	encoder->mec_theta = mec_theta;
    motordebug.ele_angle = encoder->ele_theta;
	
	/*更新速度*/
    // 将mec_theta归一化到[0, 2π)区间  
    float normalized_mec_theta = fmodf(mec_theta, 2.0f * M_PI);  
    if (normalized_mec_theta < 0.0f) {  
        normalized_mec_theta += 2.0f * M_PI; // 处理负值  
    }
    // 计算角度变化  
    float delt_theta = normalized_mec_theta - encoder->pre_theta;  
    // 处理跨越周期边界的情况  
    if (delt_theta > M_PI) {  
        delt_theta -= 2.0f * M_PI; // 逆时针大跳转  
    } else if (delt_theta < -M_PI) {  
        delt_theta += 2.0f * M_PI; // 顺时针大跳转  
    }
    // 更新上一次的角度值  
    encoder->pre_theta = normalized_mec_theta;  
    // 计算角速度（这里假设时间间隔为2ms，因此乘以500来得到每秒的角速度）  
    float omega = delt_theta / (CURRMENT_PERIOD / 2.0f);//TODO /2部分 

	//
	encoder->total_realmectheta += delt_theta;
	motordebug.pos_real = encoder->total_realmectheta;
    // 计算转速  
    float n_rap = 9.5492965f * omega;  

	float filter_n_rap;
    filter_n_rap = lowfilter_cale(&(encoder->speedfilter),n_rap);

    // 更新转速  
    encoder->speed = filter_n_rap;
	motordebug.speed_real = filter_n_rap;
}

#else
	#include "hall_sensor.h"
static void* hall_encoder(mc_encoder_t *mc_encoder);
static void* hall_encoder(mc_encoder_t *mc_encoder)
{
#ifdef 	ENCODER_TYPE_HALL
	hall_update(&(mc_encoder->hallsensor));
	hall_cale(&(mc_encoder->hallsensor));
	mc_encoder->ele_theta = mc_encoder->hallsensor.angle;
	mc_encoder->speed = mc_encoder->hallsensor.speed;
#endif	
}
#endif



//TODO
void mc_encoder_read(mc_encoder_t *encoder)
{
#ifndef ENCODER_TYPE_HALL    
	Absolute_encoder(encoder);
#else
	hall_encoder(encoder);
#endif
}






float mc_read_virvalencoder(float ialpha,float ibeta)
{
	return 0.0f;
}









//-----------------------------------------------------------------//
#if 0 
#include "mc_utils.h"

#define  ENCODER_DIR             1
#define  ENCODER_OFFSET          23406
#define  ENCODER_OFFSET_FLOAT    0.56f
#define  ENCODER_PERIOD          0.000125f
#define  ENCODER_MOTOR_PAIR      7.0f
#define  ENCODER_CPR             16384
float vel_estimate_ = 0.0f;          //当前估算转速，单位[turn/s]
static float pll_kp_ = 2000.0f;      // [count/s / count]
static float pll_ki_ = 1000000.0f;   // [(count/s^2) / count]


static void mc_encoder_readTEST(int32_t data,float *theta,float *speed);
static void mc_encoder_readTEST(int32_t data,float *theta,float *speed)
{
	int32_t delta_enc = 0;
	int32_t pos_abs_latched;                    //LATCH
	static int32_t shadow_count_ = 0;           //编码器累计计数。
	int32_t count_in_cpr_ = 0;                  //编码器当前计数值。
	static float pos_cpr_counts_ = 0.0f;        //当前约束在cpr范围内的位置值，单位[count]
	static float vel_estimate_counts_ = 0.0f;   //当前估算转速，单位[count/s]

	pos_abs_latched = *data;
	delta_enc = pos_abs_latched - count_in_cpr_; //LATCH
	delta_enc = mod(delta_enc, ENCODER_CPR);
	if(delta_enc > ENCODER_CPR/2)
	{
		delta_enc -= ENCODER_CPR;
	}
	shadow_count_ += delta_enc;
	count_in_cpr_ = pos_abs_latched;
	pos_cpr_counts_      += ENCODER_PERIOD * vel_estimate_counts_;
	// discrete phase detector
	float delta_pos_cpr_counts = (float)(count_in_cpr_ - (int32_t)pos_cpr_counts_);
	delta_pos_cpr_counts = wrap_pm(delta_pos_cpr_counts, (float)(ENCODER_CPR));
	// pll feedback
	pos_cpr_counts_ += ENCODER_PERIOD * pll_kp_ * delta_pos_cpr_counts;
	pos_cpr_counts_ = fmodf_pos(pos_cpr_counts_, (float)(ENCODER_CPR));
	vel_estimate_counts_ += ENCODER_PERIOD * pll_ki_ * delta_pos_cpr_counts;
	if (fabsf(vel_estimate_counts_) < 0.5f * ENCODER_PERIOD * pll_ki_)
	{
		vel_estimate_counts_ = 0.0f;  
	}
	vel_estimate_ = vel_estimate_counts_ / (float)ENCODER_CPR;
	int32_t corrected_enc = count_in_cpr_ - ENCODER_OFFSET;	
	float elec_rad_per_enc = ENCODER_MOTOR_PAIR * 2 * M_PI * (1.0f / (float)(ENCODER_CPR));
	float ph = elec_rad_per_enc * (corrected_enc - ENCODER_OFFSET_FLOAT);

	// *theta = wrap_pm_pi(ph) * ENCODER_DIR;
	*speed = (2*M_PI) * vel_estimate_ * ENCODER_MOTOR_PAIR * ENCODER_DIR;
	return;
}
#endif