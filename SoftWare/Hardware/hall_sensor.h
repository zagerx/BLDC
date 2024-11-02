#ifndef __HALL_SENSOR__H
#define __HALL_SENSOR__H

#include "stdint.h"
#include "filter.h"
#pragma pack(push,4)

typedef struct hall_sensor
{
    /* data */
    uint8_t last_section;
    int8_t dir;
    uint32_t count;
    float hall_baseBuff[7];
    uint8_t (*getsection)(void);
    uint32_t (*gettick)(void);
    uint32_t last_tick;
    float speed;
    float angle;
#ifdef MOTOR_OPENLOOP 
    float self_angle;
    lowfilter_t lfilter[7];
#endif // DEBUG  
    lowfilter_t speedfilter;
}hall_sensor_t;
#pragma pack(pop)

float hall_update(hall_sensor_t *hall);
float hall_cale(hall_sensor_t *hall);
void hall_init(hall_sensor_t *hall,void *pf1,void *pf2);

#endif
