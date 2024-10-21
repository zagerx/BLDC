#ifndef __HALL_SENSOR__H
#define __HALL_SENSOR__H

#include "stdint.h"

typedef struct hall_sensor
{
    /* data */
    uint8_t cur_section;
    uint8_t last_section;
    uint8_t dir;
    float base_angle;
    float hall_baseBuff[7];
    uint8_t (*getsection)(uint8_t,uint8_t,uint8_t);
    uint32_t (*gettick)(void);
    uint32_t last_tick;
    float speed;
    float angle;
}hall_sensor_t;

float hall_update(hall_sensor_t *hall);
float hall_cale(hall_sensor_t *hall);
void hall_init(hall_sensor_t *hall,void *pf1,void *pf2);

#endif
