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
    float realcacle_angle;
    float realcacle_speed;
/*PLL*/
    float hat_angle;
    float hat_speed;
    float pll_sum;
/*OUTPUT*/    
#ifdef MOTOR_OPENLOOP 
    float self_angle;
    lowfilter_t lfilter[7];
#endif // DEBUG  
    lowfilter_t speedfilter;
}hall_sensor_t;
#pragma pack(pop)

void hall_update(void *pthis);
void hall_cale(void  *pthis);
void hall_init(void *this);

#endif
