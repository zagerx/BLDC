#include "hall_sensor.h"
#include "debuglog.h"
#include "gpio.h"

uint8_t test_cursect = 0;
float test_speed,test_angle,last_last_angle;


static void hall_update_dir(hall_sensor_t *hall,int8_t dir,uint8_t cur_sect)
{
    hall->dir = dir;
    /*计算扇区速度*/
    float delt_ = hall->hall_baseBuff[cur_sect] - hall->hall_baseBuff[hall->last_section];
    if (delt_ < 0.0f)
    {
        delt_ += 6.2831852f;//hall->hall_baseBuff[cur_sect] + 6.2831852f - hall->hall_baseBuff[hall->last_section];
    }
    // delt_ = fabsf(delt_);
    hall->speed =hall->dir * delt_ / (0.0001*hall->count);

    hall->angle = hall->hall_baseBuff[cur_sect];//更新扇区基准角度
    hall->last_section = cur_sect;
    hall->count = 0;
}
volatile uint8_t test_cur_base = 0;
float hall_update(hall_sensor_t *hall)
{
    hall->count++;
    uint8_t cur_section = hall->getsection();
    test_cur_base = cur_section;
    uint32_t cur_tick = hall->gettick();
    test_cursect = cur_section;
    uint32_t delt_tick = hall->last_tick - cur_tick;
    switch (hall->last_section)
    {
    case 6:
        if (cur_section == 4)
        {
            hall_update_dir(hall,1,cur_section); 
        }else if (cur_section == 2)
        {
            hall_update_dir(hall,-1,cur_section);
        }else{
            return 0.0f;
        }
        
        break;

    case 4:
        if (cur_section == 5)
        {
            hall_update_dir(hall,1,cur_section); 
        }else if (cur_section == 6)
        {
            hall_update_dir(hall,-1,cur_section);
        }else{
            return 0.0f;
        }        
        break;
    case 5:
        if (cur_section == 1)
        {
            hall_update_dir(hall,1,cur_section);
        }else if (cur_section == 4)
        {
            hall_update_dir(hall,-1,cur_section);
        }else{
            return 0.0f;
        }        
        break;
    case 1:
        if (cur_section == 3)
        {
            hall_update_dir(hall,1,cur_section); 
        }else if (cur_section == 5)
        {
            hall_update_dir(hall,-1,cur_section);
        }else{
            return 0.0f;
        }        
        break;
    case 3:
        if (cur_section == 2)
        {
            hall_update_dir(hall,1,cur_section); 
        }else if (cur_section == 1)
        {
            hall_update_dir(hall,-1,cur_section);
        }else{
            return 0.0f;
        }        
        break;
    case 2:
        if (cur_section == 6)
        {
            hall_update_dir(hall,1,cur_section); 

        }else if (cur_section == 3)
        {
            hall_update_dir(hall,-1,cur_section);
        }else{
            return 0.0f;
        }        
        break;
    case 0:
        hall->last_section = cur_section;
        break;
    default:
        break;
    }


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
    USER_DEBUG_NORMAL("hall_init\n");
    gpio_setencoder_power(); 
    hall->getsection = pf1;
    hall->gettick = pf2;

    hall->angle = 0.0f;
    hall->hall_baseBuff[0] = 0.0000f;
    {
        hall->hall_baseBuff[6] = 0.3050f;
        hall->hall_baseBuff[4] = 1.2544f;
        hall->hall_baseBuff[5] = 2.06332f;
        hall->hall_baseBuff[1] = 3.1735f;
        hall->hall_baseBuff[3] = 3.97523f;
        hall->hall_baseBuff[2] = 5.2240f;
    }
}


