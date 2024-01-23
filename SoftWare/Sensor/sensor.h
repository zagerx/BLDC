#ifndef __SENSOR__H
#define __SENSOR__H

#include "sensor_cfg.h"

typedef struct sensor_data
{
    unsigned int raw; //原始数据
    float cov_data;   //转换后的数据    
    float filter_data;    //滤波后数据
}sensor_data_t;

typedef struct sensor
{
    /*------属性-----*/
    unsigned short cycle;//更新周期，单位ms 0代表不更新数据到缓存区
    /*------方法-----*/
    void* (*pf_read)(void);
    void (*pf_write)(void*,unsigned short len);
}sensor_t;

void sensor_process(void);
void* sensor_user_read(ENUM_SENSOR sensor_id);

#endif
