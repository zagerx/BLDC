#include "hall_sensor.h"
#include "debuglog.h"
#include "board.h"
#include "stdint.h"
static void hall_update_baseangle(hall_sensor_t *hall, int8_t dir, uint8_t cur_sect)
{
    float delt_,realcacle_speed;
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
        realcacle_speed = hall->dir * (delt_ / (HALL_UPDATE_PERIOD * hall->count));
        hall->realcacle_speed = lowfilter_cale(&(hall->speedfilter),realcacle_speed);
        hall->realcacle_angle = hall->hall_baseBuff[cur_sect] + HALL_POSITIVE_OFFSET;
    }
    else
    {
        delt_ = hall->hall_baseBuff[cur_sect] - hall->hall_baseBuff[hall->last_section];

        if (delt_ > 0.0f)
        {
            delt_ -= 6.2831852f;
        }
        realcacle_speed = hall->dir * (-1.0*delt_ / (HALL_UPDATE_PERIOD * hall->count));
        hall->realcacle_speed = realcacle_speed;//lowfilter_cale(&(hall->speedfilter),realcacle_speed);        
        hall->realcacle_angle = hall->hall_baseBuff[hall->last_section] + HALL_NEGATIVE_OFFSET;
    }
    hall->last_section = cur_sect;
    hall->count = 0;
}



uint8_t hall_update(void *pthis)
{
    hall_sensor_t *hall;
    hall = (hall_sensor_t *)pthis;
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
            hall->count++;
        }
        break;

    case 4:
        if (cur_section == 5){
            hall_update_baseangle(hall, 1, cur_section);
        }else if (cur_section == 6){
            hall_update_baseangle(hall, -1, cur_section);
        }else{
            hall->count++;
        }
        break;
    case 5:
        if (cur_section == 1){
            hall_update_baseangle(hall, 1, cur_section);
        }else if (cur_section == 4)
        {
            hall_update_baseangle(hall, -1, cur_section);
        }else{
            hall->count++;
        }
        break;
    case 1:
        if (cur_section == 3){
            hall_update_baseangle(hall, 1, cur_section);
        }else if (cur_section == 5){
            hall_update_baseangle(hall, -1, cur_section);
        }else{
            hall->count++;
        }
        
        break;
    case 3:
        if (cur_section == 2){
            hall_update_baseangle(hall, 1, cur_section);
        }else if (cur_section == 1){
            hall_update_baseangle(hall, -1, cur_section);
        }else{
            hall->count++;
        }
        break;
    case 2:
        if (cur_section == 6){
            hall_update_baseangle(hall, 1, cur_section);
        }else if (cur_section == 3){
            hall_update_baseangle(hall, -1, cur_section);
        }else{
            hall->count++;
        }
        break;
    case 0:
        hall->last_section = cur_section;
        hall->realcacle_angle = hall->hall_baseBuff[cur_section];
        break;
    default:
        break;
    }
    return 0;
}

#include "math.h"
void hall_cale(void *pthis)
{
    int32_t  delt_cnt;
    uint32_t cur_cnt;
    hall_sensor_t *hall;
    hall = (hall_sensor_t *)pthis;

    #if ((ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ))
        if (hall->dir == 1)
        {
            cur_cnt = hall->get_abzcount();
            delt_cnt = cur_cnt - hall->last_abzcout;        
            if (delt_cnt<0)
            {
                delt_cnt += ABZ_ENCODER_LINES;
            }
            hall->realcacle_angle += delt_cnt*(ABZ_ENCODER_RESOLUTION);
        }else if(hall->dir == -1)
        {
            cur_cnt = hall->get_abzcount();
            delt_cnt = hall->last_abzcout - cur_cnt;        
            if (delt_cnt<0)
            {
                delt_cnt += ABZ_ENCODER_LINES;
            }
            hall->realcacle_angle -= delt_cnt*(ABZ_ENCODER_RESOLUTION);    
        }
        hall->last_abzcout = cur_cnt;
    #elif(ENCODER_TYPE == ENCODER_TYPE_HALL)
        hall->realcacle_angle += hall->realcacle_speed * HALL_UPDATE_PERIOD;
    #endif
    if (hall->realcacle_angle > 6.2831852f)
    {
        hall->realcacle_angle -= 6.2831852f;
    }
    if (hall->realcacle_angle < 0.0f)
    {
        hall->realcacle_angle += 6.2831852f;
    }
    float delt_theta;
    delt_theta = sinf(hall->realcacle_angle)*cosf(hall->hat_angle) - cosf(hall->realcacle_angle)*sinf(hall->hat_angle);
    hall->pll_sum += delt_theta;
    float omega;
    omega =  PLL_KP*delt_theta + PLL_KI*hall->pll_sum;
    hall->hat_angle += (omega*OMEGTOTHETA);
    if (hall->hat_angle > 6.2831852f)
    {
        hall->hat_angle -= 6.2831852f;
    }
    if (hall->hat_angle < 0.0f)
    {
        hall->hat_angle += 6.2831852f;
    }
    // hall->angle = hall->realcacle_angle;    
    hall->angle = hall->hat_angle;
    omega = lowfilter_cale(&(hall->pll_speedfilter),omega);
    hall->hat_speed = omega;
    hall->speed = hall->realcacle_speed;
}





/*--------------------硬件相关---------------------------------*/
#include "gpio.h"
#include "tim.h"
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
#include "string.h"
void hall_init(void *this)
{
    hall_sensor_t *hall;
    hall = this;
    USER_DEBUG_NORMAL("hall_init\n");
    memset(this,0,sizeof(hall_sensor_t));    
    hall->getsection = hall_get_sectionnumb;
    hall->gettick = hall_gettick;
    
    #if (ENCODER_TYPE == ENCODER_TYPE_HALL_ABZ)
        hall->get_abzcount = tim_abzencoder_getcount;
        hall->set_abzcount = tim_abzencoder_setcount;    
    #endif

    // hall->last_section = 0;
    hall->last_section = hall->getsection();
    hall->realcacle_angle = hall->hall_baseBuff[hall->last_section];
    // hall->realcacle_angle = 0.0f;
    hall->realcacle_speed = 0.0f;
    hall->count = 0.0f;
    hall->speed = 0.0f;
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
    lowfilter_init(&(hall->speedfilter), 30.0F);
    lowfilter_init(&(hall->pll_speedfilter),95.0f);
}
