/**
 * 
 * 
 * 
 * 
 * 
 */
#include "hall_sensor.h"
#include "debuglog.h"
#include "board.h"

static void hall_update_baseangle(hall_sensor_t *hall, int8_t dir, uint8_t cur_sect)
{
    float delt_,speed;
#ifdef MOTOR_OPENLOOP
    volatile static float test_temp[7];
    test_temp[cur_sect] = lowfilter_cale(&(hall->lfilter[cur_sect]), hall->self_angle);
    USER_DEBUG_NORMAL("%d----->%f\n", cur_sect, test_temp[cur_sect]);
#endif
    hall->dir = dir;
    /*计算扇区速度*/
    if (dir > 0)
    {
        delt_ = -(hall->hall_baseBuff[hall->last_section] - hall->hall_baseBuff[cur_sect]);

        if (delt_ < 0.0f)
        {
            delt_ += 6.2831852f;
        }
        speed = hall->dir * (delt_ / (HALL_UPDATE_PERIOD * hall->count));
        hall->speed = lowfilter_cale(&(hall->speedfilter),speed);
        hall->angle = hall->hall_baseBuff[cur_sect] + HALL_POSITIVE_OFFSET;
    }
    else
    {
        delt_ = hall->hall_baseBuff[cur_sect] - hall->hall_baseBuff[hall->last_section];

        if (delt_ > 0.0f)
        {
            delt_ -= 6.2831852f;
        }
        speed = hall->dir * (-1.0*delt_ / (HALL_UPDATE_PERIOD * hall->count));
        hall->speed = lowfilter_cale(&(hall->speedfilter),speed);        
        hall->angle = hall->hall_baseBuff[hall->last_section] + HALL_NEGATIVE_OFFSET;
    }
    hall->last_section = cur_sect;
    hall->count = 0;
}



void hall_update(void *pthis)
{
    hall_sensor_t *hall;
    hall = (hall_sensor_t *)pthis;
    hall->count++;
    uint8_t cur_section = hall->getsection();
    uint32_t cur_tick = hall->gettick();
    uint32_t delt_tick = hall->last_tick - cur_tick;
    switch (hall->last_section)
    {
    case 6:
        if (cur_section == 4)
        {
            hall_update_baseangle(hall, 1, cur_section);
        }else if (cur_section == 2){
            hall_update_baseangle(hall, -1, cur_section);
        }else{
        }
        break;

    case 4:
        if (cur_section == 5){
            hall_update_baseangle(hall, 1, cur_section);
        }else if (cur_section == 6){
            hall_update_baseangle(hall, -1, cur_section);
        }else{
        }
        break;
    case 5:
        if (cur_section == 1){
            hall_update_baseangle(hall, 1, cur_section);
        }else if (cur_section == 4)
        {
            hall_update_baseangle(hall, -1, cur_section);
        }else{
        }
        break;
    case 1:
        if (cur_section == 3){
            hall_update_baseangle(hall, 1, cur_section);
        }else if (cur_section == 5){
            hall_update_baseangle(hall, -1, cur_section);
        }else{
        }
        break;
    case 3:
        if (cur_section == 2){
            hall_update_baseangle(hall, 1, cur_section);
        }else if (cur_section == 1){
            hall_update_baseangle(hall, -1, cur_section);
        }else{
        }
        break;
    case 2:
        if (cur_section == 6){
            hall_update_baseangle(hall, 1, cur_section);
        }else if (cur_section == 3){
            hall_update_baseangle(hall, -1, cur_section);
        }else{
        }
        break;
    case 0:
        hall->last_section = cur_section;
        hall->angle = hall->hall_baseBuff[cur_section];
        break;
    default:
        break;
    }
}

/*
angle = speed*t

freq = 10kh
*/
void hall_cale(void *pthis)
{
    hall_sensor_t *hall;
    hall = (hall_sensor_t *)pthis;
    hall->angle += hall->speed * HALL_UPDATE_PERIOD;
    if (hall->angle > 6.2831852f)
    {
        hall->angle -= 6.2831852f;
    }
    if (hall->angle < 0.0f)
    {
        hall->angle += 6.2831852f;
    }
}





/*--------------------硬件相关---------------------------------*/
#include "gpio.h"
static uint8_t hall_get_sectionnumb(void)
{
    uint8_t u,v,w;
    u = HAL_GPIO_ReadPin(HALL_U1_GPIO_Port,HALL_U1_Pin);
    v = HAL_GPIO_ReadPin(HALL_V1_GPIO_Port,HALL_V1_Pin);
    w = HAL_GPIO_ReadPin(HALL_W1_GPIO_Port,HALL_W1_Pin);
    return u | (w<<1) | (v<<2);
}
static uint32_t hall_gettick()
{
    return 0;
}

void hall_init(void *this)
{
    hall_sensor_t *hall;
    hall = this;
    USER_DEBUG_NORMAL("hall_init\n");
    hall->getsection = hall_get_sectionnumb;
    hall->gettick = hall_gettick;
    hall->angle = 0.0f;
    hall->last_section = 0;

    hall->hall_baseBuff[0] = 0.0000f;
    {
        hall->hall_baseBuff[6] = SCETION_6_BASEANGLE;
        hall->hall_baseBuff[4] = SCETION_4_BASEANGLE;
        hall->hall_baseBuff[5] = SCETION_5_BASEANGLE;
        hall->hall_baseBuff[1] = SCETION_1_BASEANGLE;
        hall->hall_baseBuff[3] = SCETION_3_BASEANGLE;
        hall->hall_baseBuff[2] = SCETION_2_BASEANGLE;
    }

#ifdef MOTOR_OPENLOOP
    for (uint8_t i = 0; i < 7; i++)
    {
        lowfilter_init(&(hall->lfilter[i]), 5.0F);
    }
#endif // DEBUG
    lowfilter_init(&(hall->speedfilter), 5.0F);
}
