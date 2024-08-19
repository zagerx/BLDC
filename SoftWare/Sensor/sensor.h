#ifndef __SENSOR__H
#define __SENSOR__H
#include "stdint.h"

#ifdef ENABLE_SENSORMODULE
    #include "sensor_enum.h"
#else
    #error "sensor_enum.h is not included because ENABLE_SENSORMODULE is not defined. Compilation will stop."  
#endif


typedef enum _sensor_stat{
    EN_SENSOR_INIT,
    EN_SENSOR_NORMAL,
    EN_SENSOR_STOP,
}sens_type_sta_t;


typedef struct sensor
{
    /*------数据-----*/
    void *pdata;
    /*------属性-----*/
    unsigned short cycle;//更新周期，单位ms 0代表不更新数据到缓存区
    sens_type_sta_t status;  //传感器状态
    /*------方法-----*/
    void* (*pf_read)(void);
    void (*pf_write)(void*,unsigned short len);
    void (*pf_init)(void);
}sensor_t;

void sensor_process(void);
void* sensor_user_read(sens_type_idx_t sensor_id);
void sensor_register(sensor_t *this,sens_type_idx_t index);
#endif
