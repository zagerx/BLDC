#include "hall_sensor.h"

float hall_update(hall_sensor_t *hall)
{
    uint8_t last_section = hall->last_section;
    uint8_t cur_section = hall->getsection();
    uint32_t cur_tick = hall->gettick();
    uint32_t delt_tick = hall->last_tick - cur_tick;

    switch (last_section)
    {
    case /* constant-expression */:
        /* code */
        break;
    
    default:
        break;
    }

    /*计算上个扇区速度*/ 
    hall->speed = (hall->hall_baseBuff[ hall->cur_section]-hall->hall_baseBuff[hall->last_section])/delt_tick;
    hall->angle = hall->hall_baseBuff[ hall->cur_section];
}

/*
angle = speed*t

freq = 10kh
*/
#define CURLOOP_PER   0.0001f
float hall_cale(hall_sensor_t *hall)
{
    hall->angle += hall->speed*CURLOOP_PER;
}

