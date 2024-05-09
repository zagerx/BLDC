#include "as5047.h"
#include "spi.h"
#include "gpio.h"


#include "stdbool.h"
#include "math.h"
typedef struct as5047_data
{
    int32_t *raw_buf;
    int32_t *covdata_buf;
    int32_t *filterdata_buf;
    int16_t buf_column;
}as5047_data_t;
static as5047_data_t sg_as5047data = {0};
int32_t rawdata,covdata,filterdata;
static int16_t column;

typedef struct 
{
	float  bandwidth;          //编码器带宽，和cpr成正比
	int32_t  phase_offset;     //编码器和转子电角度之间的相位差，run_offset_calibration()校准这个参数
	float  phase_offset_float; //编码器和转子电角度之间的相位差浮点部分，run_offset_calibration()校准这个参数
	int32_t  cpr;              //每圈编码器的脉冲数
	// float  index_offset;
	// bool  use_index;           //是否使用编码器索引信号
	// bool  pre_calibrated;      // If true, this means the offset stored in
	                           // configuration is valid and does not need
                             // be determined by run_offset_calibration.
                             // In this case the encoder will enter ready
                             // state as soon as the index is found.
	int32_t  direction;        //电机转动方向，run_offset_calibration()校准这个参数
	// bool  use_index_offset;
	bool  enable_phase_interpolation; // Use velocity to interpolate inside the count state
	float phase_;              //最终用于计算的当前电角度，范围-Pi~Pi。
	float phase_vel_;          //最终用于计算的当前电角速度，单位rad/s。
} ENCODER_CONFIG;

#define TIM_1_8_CLOCK_HZ 168000000
#define TIM_1_8_PERIOD_CLOCKS 3500
#define TIM_1_8_DEADTIME_CLOCKS 100  //100=595ns
#define TIM_APB1_CLOCK_HZ 84000000
#define TIM_APB1_PERIOD_CLOCKS 4096
#define TIM_APB1_DEADTIME_CLOCKS 50  //50=595ns
#define TIM_1_8_RCR 2
#define  ENCODER_mode        MODE_SPI_AS5047P    //编码器类型，共四种:MODE_INCREMENTAL,MODE_SPI_AS5047P,MODE_SPI_MT6701,MODE_SPI_MA730,MODE_SPI_TLE5012B
#define  ENCODER_cpr                   16384     //AS5047P=MT6701=16384,TLE5012B=32768, MODE_INCREMENTAL=4000,
#define  ENCODER_bandwidth             1000      //默认1000，hall电机的cpr比较小，可设置为100
#define M_PI  3.14159265358979323846f
#define  MOTOR_pole_pairs                          1    //电机极对数
#define CURRENT_MEAS_PERIOD ( (float)2*TIM_1_8_PERIOD_CLOCKS*(TIM_1_8_RCR+1) / (float)TIM_1_8_CLOCK_HZ )
//  const float current_meas_period = CURRENT_MEAS_PERIOD;
const float current_meas_period = 0.0001f;
ENCODER_CONFIG   encoder_config;

static float pll_kp_ = 0.0f;   // [count/s / count]
static float pll_ki_ = 0.0f;   // [(count/s^2) / count]

bool pos_estimate_valid_ = false;   //位置估算是否可用
bool vel_estimate_valid_ = false;   //速度估算是否可用

float pos_estimate_ = 0.0f; //当前估算的位置值，单位[turn]
float vel_estimate_ = 0.0f; //当前估算转速，单位[turn/s]

/*****************************************************************************/
/*****************************************************************************/
static inline float wrap_pm(float x, float y) {
#ifdef FPU_FPV4
	float intval = (float)round(x / y);  //round_int
#else
	float intval = nearbyintf(x / y);
#endif
	return x - intval * y;
}
static inline float fmodf_pos(float x, float y) {
	float res = wrap_pm(x, y);
	if (res < 0) res += y;
	return res;
}

static inline float wrap_pm_pi(float x) {
	return wrap_pm(x, 2 * M_PI);
}

static inline int mod(const int dividend, const int divisor)
{
	int r = dividend % divisor;
	if (r < 0) r += divisor;
	return r;
}


void encoder_set_error(uint32_t error) 
{
	vel_estimate_valid_ = false;
	pos_estimate_valid_ = false;
}
/*****************************************************************************/
void update_pll_gains(void)
{
	pll_kp_ = 2.0f * encoder_config.bandwidth;  // basic conversion to discrete time
	pll_ki_ = 0.25f * (pll_kp_ * pll_kp_); // Critically damped
}
/*****************************************************************************/


bool run_offset_calibration(void)
{
	uint32_t  i;
	const float start_lock_duration = 1.0f;	

	return true;
}
//初始化三种SPI接口的编码器的参数, 初始化I2C接口或者SPI接口
void MagneticSensor_Init(void)
{

	encoder_config.bandwidth = ENCODER_bandwidth;   //编码器带宽

	encoder_config.phase_offset = 0;        // Offset between encoder count and rotor electrical phase
	encoder_config.phase_offset_float = 0.0f; // Sub-count phase alignment offset
	encoder_config.enable_phase_interpolation = true; // Use velocity to interpolate inside the count state

	encoder_config.direction = 1;
	encoder_config.phase_offset = 23406;
	encoder_config.phase_offset_float = 0.56f;


	pll_kp_ = 2.0f * ENCODER_bandwidth;  // basic conversion to discrete time
	pll_ki_ = 0.25f * (pll_kp_ * pll_kp_); // Critically damped

}


/****************************************************************************/
bool encoder_update(int32_t raw)
{
	int32_t delta_enc = 0;
	int32_t pos_abs_latched; //LATCH
	static int32_t shadow_count_ = 0;   //编码器累计计数。
    int32_t count_in_cpr_ = 0;   //编码器当前计数值。
    float interpolation_ = 0.0f; //编码器当前插补值。
	static float pos_estimate_counts_ = 0.0f;  //当前估算的位置值，单位[count]   
	static float pos_cpr_counts_ = 0.0f;       //当前约束在cpr范围内的位置值，单位[count]
	static float vel_estimate_counts_ = 0.0f;  //当前估算转速，单位[count/s]

	pos_abs_latched = raw;

	delta_enc = pos_abs_latched - count_in_cpr_; //LATCH
	delta_enc = mod(delta_enc, encoder_config.cpr);
	if(delta_enc > encoder_config.cpr/2)
	{
        delta_enc -= encoder_config.cpr;
    }
    shadow_count_ += delta_enc;
    count_in_cpr_ = pos_abs_latched;
	
	// Memory for pos_circular
	float pos_cpr_counts_last = pos_cpr_counts_;
	
	// run pll (for now pll is in units of encoder counts)
	// Predict current pos
	pos_estimate_counts_ += current_meas_period * vel_estimate_counts_;
	pos_cpr_counts_      += current_meas_period * vel_estimate_counts_;
	
	// discrete phase detector
	float delta_pos_counts = (float)(shadow_count_ - (int32_t)pos_estimate_counts_);
	float delta_pos_cpr_counts = (float)(count_in_cpr_ - (int32_t)pos_cpr_counts_);
	delta_pos_cpr_counts = wrap_pm(delta_pos_cpr_counts, (float)(ENCODER_cpr));
	// pll feedback
	pos_estimate_counts_ += current_meas_period * pll_kp_ * delta_pos_counts;
	pos_cpr_counts_ += current_meas_period * pll_kp_ * delta_pos_cpr_counts;
	pos_cpr_counts_ = fmodf_pos(pos_cpr_counts_, (float)(ENCODER_cpr));
	vel_estimate_counts_ += current_meas_period * pll_ki_ * delta_pos_cpr_counts;
	uint8_t snap_to_zero_vel = false;
	if (fabsf(vel_estimate_counts_) < 0.5f * current_meas_period * pll_ki_)
	{
		vel_estimate_counts_ = 0.0f;  //align delta-sigma on zero to prevent jitter
		snap_to_zero_vel = true;
	}
	
	// Outputs from Encoder for Controller
	pos_estimate_ = pos_estimate_counts_ / (float)ENCODER_cpr;
	vel_estimate_ = vel_estimate_counts_ / (float)ENCODER_cpr;

	//// run encoder count interpolation
	int32_t corrected_enc = count_in_cpr_ - encoder_config.phase_offset;
	// if we are stopped, make sure we don't randomly drift
	if(snap_to_zero_vel || !encoder_config.enable_phase_interpolation)
	{
		interpolation_ = 0.5f;
	}
	else if(delta_enc > 0){
		interpolation_ = 0.0f;
	}else if(delta_enc < 0){
		interpolation_ = 1.0f;
	}
	else {
		// Interpolate (predict) between encoder counts using vel_estimate,
		interpolation_ += current_meas_period * vel_estimate_counts_;
		// don't allow interpolation indicated position outside of [enc, enc+1)
		if (interpolation_ > 1.0f) interpolation_ = 1.0f;
		if (interpolation_ < 0.0f) interpolation_ = 0.0f;
	}
	float interpolated_enc = corrected_enc + interpolation_;
	
	//TODO avoid recomputing elec_rad_per_enc every time
	float elec_rad_per_enc = MOTOR_pole_pairs  * 2 * M_PI * (1.0f / (float)(ENCODER_cpr));
	float ph = elec_rad_per_enc * (interpolated_enc - encoder_config.phase_offset_float);

	encoder_config.phase_ = wrap_pm_pi(ph) * encoder_config.direction;
	encoder_config.phase_vel_ = (2*M_PI) * vel_estimate_ * MOTOR_pole_pairs  * encoder_config.direction;

	return 1;
}
/****************************************************************************/
//SPI发送读取函数
static uint8_t ams_parity(uint16_t v)
{
	v ^= v >> 8;
	v ^= v >> 4;
	v ^= v >> 2;
	v ^= v >> 1;
	return v & 1;
}
uint16_t SPI_ReadWrite_OneByte(uint16_t _txdata)
{

    uint16_t pos,rawVal;
	HAL_GPIO_WritePin(SPI3_SOFTCS_GPIO_Port,SPI3_SOFTCS_Pin,GPIO_PIN_RESET);	//cs拉低

    HAL_SPI_TransmitReceive(&hspi3,(uint8_t *)&_txdata,(uint8_t *)&rawVal,1,1000);
	HAL_GPIO_WritePin(SPI3_SOFTCS_GPIO_Port,SPI3_SOFTCS_Pin,GPIO_PIN_SET);

	if(ams_parity(rawVal)) //|| ((rawVal >> 14) & 1)
    {
        return 0xFFFF;
    }else{
        return pos = (rawVal & 0x3fff);
    }

}
void* as5047_read(void)
{
    uint16_t add = 0;
	uint16_t data;
    data=SPI_ReadWrite_OneByte(0xFFFF); //发送一条空指令，读取上一次指令返回的数据。
    encoder_update(data);

    rawdata = data;
    // covdata = data * 402;
    covdata = encoder_config.phase_ * (1<<20);
    filterdata = encoder_config.phase_vel_ * (1<<15);
	return (void *)&sg_as5047data;
}

void as5047_init(void)
{
    sg_as5047data.raw_buf = &rawdata;
    sg_as5047data.covdata_buf = &covdata;
    sg_as5047data.filterdata_buf = &filterdata;
    MagneticSensor_Init();
}

