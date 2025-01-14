#include "abs_sensor.h"
#include "board.h"
#include "math.h"
#include "mc_utils.h"
#include "debuglog.h"
#include <stdarg.h>
#include <string.h>
/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        obj 
 * @param        ... 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void abs_sensor_register(void *obj,...)
{
    va_list args;
    va_start(args, obj);
    abs_sensor_t *abs_sensor = (abs_sensor_t *)obj;

    // 初始化 abs_sensor 结构体
    memset(abs_sensor, 0, sizeof(abs_sensor_t));
    abs_sensor->get_rawdata = va_arg(args,uint32_t (*)(void));

    abs_sensor->cacle = va_arg(args,void (*)(void*));
    abs_sensor->update_base = va_arg(args,uint8_t (*)(void*));
    abs_sensor->init = va_arg(args,void (*)(void*));
    abs_sensor->deinit = va_arg(args,void (*)(void*));
    abs_sensor->get_first_points = va_arg(args,void (*)(void*));
    abs_sensor->set_calib_points = va_arg(args,void (*)(void*));

    va_end(args);
}
/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void abs_sensor_init(void *obj)
{
    abs_sensor_t *abs_sensor = (abs_sensor_t *)obj;
    lowfilter_init(&(abs_sensor->speedfilter),5.0f);
}
/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void abs_sensor_deinit(void *obj)
{
    abs_sensor_t *abs_sensor = (abs_sensor_t *)obj;

}
/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void abs_sensor_get_firstpoint(void *obj)
{
    abs_sensor_t *abs_sensor = (abs_sensor_t *)obj;
}
/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void abs_sensor_set_cairbpoint(void *obj)
{
    abs_sensor_t *abs_sensor = (abs_sensor_t *)obj;
}

/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void abs_sensor_cacle(void *obj)
{
    abs_sensor_t *abs_sensor = (abs_sensor_t *)obj;
    uint32_t data = abs_sensor->get_rawdata();

    abs_sensor->raw_data = data;
    float mec_theta = data * ENCODER_CPR - MEC_ANGLE_OFFSET;
    
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
    abs_sensor->angle = mec_theta;
    abs_sensor->speed = filter_n_rap;
}
/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void abs_sensor_update(void *obj)
{
}
