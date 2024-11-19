#ifndef __ABS_SENSOR__H
#define __ABS_SENSOR__H
#include "stdint.h"
#include "filter.h"
typedef struct abs_sensor
{
    int32_t raw_data;
    float mec_theta;
    float pre_theta; 
    float total_distance;   
    float ele_theta;
    float angle;
    float speed;
    lowfilter_t speedfilter;
    float  total_realmectheta;
    uint32_t (*get_rawdata)(void);
}abs_sensor_t;



#endif
