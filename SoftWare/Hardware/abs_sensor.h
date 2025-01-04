#ifndef __ABS_SENSOR__H
#define __ABS_SENSOR__H
#include "stdint.h"
#include "filter.h"
typedef struct abs_sensor
{
    int32_t raw_data;
    float pre_theta; 
    float total_distance;   
    float angle;
    float speed;
    lowfilter_t speedfilter;
    float  total_realmectheta;
    float self_angle;
    uint32_t (*get_rawdata)(void);

    void (*cacle)(void*);
    uint8_t (*update_base)(void*);
    void (*init)(void*);
    void (*deinit)(void*);
    void (*get_first_points)(void*);
    void (*set_calib_points)(void*);
}abs_sensor_t;

void abs_sensor_register(void *obj,...);
void abs_sensor_init(void *obj);
void abs_sensor_deinit(void *obj);
void abs_sensor_get_firstpoint(void *obj);
void abs_sensor_set_cairbpoint(void *obj);
void abs_sensor_cacle(void *obj);
void abs_sensor_update(void *obj);

#endif
