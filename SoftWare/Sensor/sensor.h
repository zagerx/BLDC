#ifndef __SENSOR__H
#define __SENSOR__H
#include "sensor_cfg.h"

typedef enum _sensor_stat{
    EN_SENSOR_INIT,
    EN_SENSOR_NORMAL,
    EN_SENSOR_STOP,
}sensor_stat;

typedef enum _sensor_datatype{
    EN_SENSORDATA_RAW,
    EN_SENSORDATA_COV,
    EN_SENSORDATA_FILTER,
}sensordata_type;

typedef struct sensor_data
{
    int32_t *raw_buf;     //原始数据
    int32_t *covdata_buf;       //转换后的数据    
    int32_t *filterdata_buf;    //滤波后数据
    int16_t buf_column;
}sensor_data_t;

typedef struct sensor
{
    /*------属性-----*/
    unsigned short cycle;//更新周期，单位ms 0代表不更新数据到缓存区
    sensor_stat status;  //传感器状态
    /*------方法-----*/
    void* (*pf_read)(void);
    void (*pf_write)(void*,unsigned short len);
    void (*pf_init)(void);
}sensor_t;

void sensor_process(void);
void* sensor_user_read(ENUM_SENSOR sensor_id,sensordata_type _type);
void sensor_register(sensor_t *this,ENUM_SENSOR index);
#endif
