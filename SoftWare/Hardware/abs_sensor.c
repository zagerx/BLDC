#include "abs_sensor.h"
#include "board.h"
#include "math.h"
#include "mc_utils.h"
void Absolute_encoder_read(void *this)
{
    abs_sensor_t *abs_sensor = this;
    uint32_t data = abs_sensor->get_rawdata;

    abs_sensor->raw_data = data;
    float mec_theta = data * ENCODER_CPR - MEC_ANGLE_OFFSET;
    float ele_theta = mec_theta * MOTOR_PAIRS;
    abs_sensor->ele_theta = wrap_pm_pi(ele_theta);
    abs_sensor->mec_theta = mec_theta;
    
    /*更新速度*/
    // 将mec_theta归一化到[0, 2π)区间  
    float normalized_mec_theta = fmodf(mec_theta, 2.0f * M_PI);  
    if (normalized_mec_theta < 0.0f) {  
        normalized_mec_theta += 2.0f * M_PI; // 处理负值  
    }
    // 计算角度变化  
    float delt_theta = normalized_mec_theta - abs_sensor->pre_theta;  
    // 处理跨越周期边界的情况  
    if (delt_theta > M_PI) {  
        delt_theta -= 2.0f * M_PI; // 逆时针大跳转  
    } else if (delt_theta < -M_PI) {  
        delt_theta += 2.0f * M_PI; // 顺时针大跳转  
    }
    // 更新上一次的角度值  
    abs_sensor->pre_theta = normalized_mec_theta;  
    // 计算角速度（这里假设时间间隔为2ms，因此乘以500来得到每秒的角速度）  
    float omega = delt_theta / (CURRMENT_PERIOD / 2.0f);//TODO /2部分 

    //
    abs_sensor->total_realmectheta += delt_theta;
    // 计算转速  
    float n_rap = 9.5492965f * omega;  

    float filter_n_rap;
    filter_n_rap = lowfilter_cale(&(abs_sensor->speedfilter),n_rap);

    // 更新转速  
    abs_sensor->speed = filter_n_rap;
}

void abs_sensor_update(void* sensor)
{
    abs_sensor_t *pthis = (abs_sensor_t*)sensor;
    //读取原始数据
    
}
