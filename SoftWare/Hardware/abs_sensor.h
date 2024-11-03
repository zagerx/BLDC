#ifndef __ABS_SENSOR__H
#define __ABS_SENSOR__H

typedef struct abs_sensor
{
    int32_t raw_data;
    float mec_theta;
    float pre_theta;    
}abs_sensor_t;



#endif
