#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include "mc_utils.h"
#include "motorctrl_common.h"
float vel_estimate_ = 0.0f;          //当前估算转速，单位[turn/s]
static float pll_kp_ = 2000.0f;      // [count/s / count]
static float pll_ki_ = 1000000.0f;   // [(count/s^2) / count]
#define  ENCODER_DIR             1
#define  ENCODER_OFFSET          23406
#define  ENCODER_OFFSET_FLOAT    0.56f
#define  ENCODER_PERIOD          0.0001f
#define  ENCODER_MOTOR_PAIR      7.0f
#define  ENCODER_CPR             16384


static void mc_encoder_readTEST(int32_t data,float *theta,float *speed);

static int32_t test_data;
void mc_encoder_readspeedangle(int32_t *data,float *theta,float *speed)
{

	int32_t delta_enc = 0;
	int32_t pos_abs_latched;                    //LATCH
	static int32_t shadow_count_ = 0;           //编码器累计计数。
	int32_t count_in_cpr_ = 0;                  //编码器当前计数值。
	static float pos_cpr_counts_ = 0.0f;        //当前约束在cpr范围内的位置值，单位[count]
	static float vel_estimate_counts_ = 0.0f;   //当前估算转速，单位[count/s]

	mc_encoder_readTEST(*data,theta,0);
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
	uint8_t snap_to_zero_vel = false;
	if (fabsf(vel_estimate_counts_) < 0.5f * ENCODER_PERIOD * pll_ki_)
	{
		vel_estimate_counts_ = 0.0f;  
		snap_to_zero_vel = true;
	}
	vel_estimate_ = vel_estimate_counts_ / (float)ENCODER_CPR;
	int32_t corrected_enc = count_in_cpr_ - ENCODER_OFFSET;	
	float elec_rad_per_enc = ENCODER_MOTOR_PAIR * 2 * M_PI * (1.0f / (float)(ENCODER_CPR));
	float ph = elec_rad_per_enc * (corrected_enc - ENCODER_OFFSET_FLOAT);

	// *theta = wrap_pm_pi(ph) * ENCODER_DIR;
	*speed = (2*M_PI) * vel_estimate_ * ENCODER_MOTOR_PAIR * ENCODER_DIR;

	// motordebug.self_ele_theta = *theta;

	return;
}



static  float wrap_pm_pi_user(float x) {  
    // 使用fmodf函数来获取x除以2π的余数，这将把x包裹到[-2π, 2π)范围内  
    float wrapped = fmodf(x, 2.0f * 3.14159265358979323846f);  
  
    // 如果结果小于-π，则加上2π将其移动到[0, π)范围内  
    // 如果结果大于等于π，则减去2π将其移动到[-π, 0)范围内  
    // 实际上，由于我们只关心[-π, π)范围，所以只需检查大于等于π的情况  
    if (wrapped >= 3.14159265358979323846f) {  
        wrapped -= 2.0f * 3.14159265358979323846f;  
    }  
  
    // 注意：这里假设输入的x已经是弧度制。如果x是度数，则需要先转换为弧度（x * (π/180)）  
  
    return wrapped;  
}  
float test_data1,test_data2;
static void mc_encoder_readTEST(int32_t data,float *theta,float *speed)
{
	// float theta = 0.0f;
	test_data = data;
    float mec_theta = data * 0.00038349f - 0.0056f;
	test_data1 = mec_theta;
    float ele_theta = mec_theta * 7.0f;
	test_data2 = ele_theta;
    *theta = wrap_pm_pi(ele_theta);
    motordebug.ele_angle = *theta;


}

float mc_read_virvalencoder(float ialpha,float ibeta)
{
	return 0.0f;
}


