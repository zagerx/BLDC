#include "hall_sensor.h"
#include "debuglog.h"
#include "gpio.h"

uint8_t test_cursect = 0;
float test_speed,test_angle,last_last_angle;
float hall_update(hall_sensor_t *hall)
{

    uint8_t cur_section = hall->getsection();
    uint32_t cur_tick = hall->gettick();
    test_cursect = cur_section;
    uint32_t delt_tick = hall->last_tick - cur_tick;
    last_last_angle = hall->hall_baseBuff[hall->last_section];
    switch (hall->last_section)
    {
    case 6:
        if (cur_section == 4)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->last_section = 4;
        }else if (cur_section == 2)
        {
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->dir = 1;
            hall->last_section = 2;
        }else{
            return;
        }
        
        break;

    case 4:
        if (cur_section == 5)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->last_section = 5;
        }else if (cur_section == 6)
        {
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->dir = 1;
            hall->last_section = 6;
        }else{
            return;
        }        
        break;
    case 5:
        if (cur_section == 1)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->last_section = 5;
        }else if (cur_section == 4)
        {
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->dir = 1;
            hall->last_section = 4;
        }else{
            return;
        }        
        break;
    case 1:
        if (cur_section == 3)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->last_section = 3;
        }else if (cur_section == 5)
        {
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->dir = 1;
            hall->last_section = 5;
        }else{
            return;
        }        
        break;
    case 3:
        if (cur_section == 2)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->last_section = 2;
        }else if (cur_section == 1)
        {
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->dir = 1;
            hall->last_section = 1;
        }else{
            return;
        }        
        break;
    case 2:
        if (cur_section == 6)
        {
            hall->dir = 0;//方向为正
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->last_section = 6;
        }else if (cur_section == 3)
        {
            hall->base_angle = hall->hall_baseBuff[cur_section];
            hall->dir = 1;
            hall->last_section = 3;
        }else{
            return;
        }        
        break;
    case 0:
        hall->last_section = cur_section;
        break;

    default:
        break;
    }

    /*计算上个扇区速度*/ 
    delt_tick = 1000.0f;
    test_speed = -0.01f;//(hall->hall_baseBuff[cur_section] - last_last_angle)/delt_tick;
    test_angle = hall->hall_baseBuff[cur_section];
}

/*
angle = speed*t

freq = 10kh
*/
#define CURLOOP_PER   0.0001f
float hall_cale(hall_sensor_t *hall)
{
    // test_angle += hall->speed*CURLOOP_PER;
    test_angle += test_speed*CURLOOP_PER;
}

//static float sg_hall_section[7] = {0.0f,3.6652f,5.7595f ,4.7123f,1.5707f,2.6179,-0.32f};
void hall_init(hall_sensor_t *hall,void *pf1,void *pf2)
{
    USER_DEBUG_NORMAL("hall_init\n");
    gpio_setencoder_power(); 
    hall->getsection = pf1;
    hall->gettick = pf2;

    hall->angle = 0.0f;
    hall->base_angle = 0.0f;
    hall->hall_baseBuff[0] = 0.0000f;hall->hall_baseBuff[1] = -0.32f;hall->hall_baseBuff[2] = 2.6179f;hall->hall_baseBuff[3] =  1.5707f;
    hall->hall_baseBuff[4] = 4.7123f;hall->hall_baseBuff[5] = 5.7595f;hall->hall_baseBuff[6] = 3.6652f;
}


