#include "hall_sensor.h"

float hall_update(hall_sensor_t *hall)
{
    uint8_t cur_section = hall->getsection();
    uint32_t cur_tick = hall->gettick();
    uint32_t delt_tick = hall->last_tick - cur_tick;

    switch (hall->last_section)
    {
    case 6:
        if (section == 4)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall_baseBuff[section];
            hall->last_section = 4;
        }else if (section == 2)
        {
            hall->base_angle = hall_baseBuff[section];
            hall->dir = 1;
            hall->last_section = 2;
        }else{
            //err
        }
        
        break;

    case 4:
        if (section == 5)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall_baseBuff[section];
            hall->last_section = 5;
        }else if (section == 6)
        {
            hall->base_angle = hall_baseBuff[section];
            hall->dir = 1;
            hall->last_section = 6;
        }else{
            //err
        }        
        break;
    case 5:
        if (section == 1)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall_baseBuff[section];
            hall->last_section = 5;
        }else if (section == 4)
        {
            hall->base_angle = hall_baseBuff[section];
            hall->dir = 1;
            hall->last_section = 4;
        }else{
            //err
        }        
        break;
    case 1:
        if (section == 3)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall_baseBuff[section];
            hall->last_section = 3;
        }else if (section == 5)
        {
            hall->base_angle = hall_baseBuff[section];
            hall->dir = 1;
            hall->last_section = 5;
        }else{
            //err
        }        
        break;
    case 3:
        if (section == 2)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall_baseBuff[section];
            hall->last_section = 2;
        }else if (section == 1)
        {
            hall->base_angle = hall_baseBuff[section];
            hall->dir = 1;
            hall->last_section = 1;
        }else{
            //err
        }        
        break;
    case 2:
        if (section == 6)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall_baseBuff[section];
            hall->last_section = 6;
        }else if (section == 3)
        {
            hall->base_angle = hall_baseBuff[section];
            hall->dir = 1;
            hall->last_section = 3;
        }else{
            //err
        }        
        break;
    case 0:
        hall->last_section = section;
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

void hall_init(hall_sensor_t *hall,void *pf1,void *pf2)
{
    hall->getsection = pf1;
    hall->gettick = pf2;

    hall->angle = 0.0f;
    hall->base_angle = 0.0f
}


