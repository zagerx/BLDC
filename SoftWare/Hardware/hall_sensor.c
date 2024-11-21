#include "hall_sensor.h"
#include "debuglog.h"
#include "board.h"
#include "stdint.h"
#include "math.h"

static void hall_update_baseangle(hall_sensor_t *hall, int8_t dir, uint8_t cur_sect) {
    float delta, real_calc_speed;
    const float TWO_PI = 6.2831852f;
 #if( MOTOR_OPENLOOP && !MOTOR_OPENLOOP_ENCODER)//CPU负担过重，只能在自开环条件下打开
    volatile static float test_temp[7];
    test_temp[cur_sect] = lowfilter_cale(&(hall->lfilter[cur_sect]), hall->self_angle);
    USER_DEBUG_NORMAL("%d----->%f\n", cur_sect, test_temp[cur_sect]);
#endif
    hall->dir = dir;
    delta = hall->hall_baseBuff[cur_sect] - hall->hall_baseBuff[hall->last_section];

    // Adjust delta based on direction and ensure it is within the range [-PI, PI]
    if (dir > 0) {
        if (delta < 0.0f) {
            delta += TWO_PI;
        }
    } else {
        if (delta > 0.0f) {
            delta -= TWO_PI;
        }
    }
 
    // Calculate the real speed, applying the direction and filtering
    real_calc_speed = 1 * (delta / (HALL_UPDATE_PERIOD * hall->count));
    hall->realcacle_speed = lowfilter_cale(&(hall->speedfilter), real_calc_speed);
 
    // Update the angle with the appropriate offset
    hall->realcacle_angle = hall->hall_baseBuff[cur_sect] + (dir > 0 ? HALL_POSITIVE_OFFSET : HALL_NEGATIVE_OFFSET);
 
    // Update the last section and reset the count
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

void hall_get_initpos(void *pthis)
{
    hall_sensor_t *hall;
    hall = (hall_sensor_t*)pthis;
    uint8_t cur_sect = hall->getsection();
    hall->last_section = cur_sect;
    hall->realcacle_angle = hall->hall_baseBuff[cur_sect] + 1.0471f;
}



/*--------------------硬件相关---------------------------------*/
#include "string.h"
#include <stdarg.h>

// void hall_register(void *this)
// {
//     hall_sensor_t *hall;
//     hall = (hall_sensor_t *)this;
//     USER_DEBUG_NORMAL("hall register\n");
//     memset(hall,0,sizeof(hall_sensor_t));    
//     hall->getsection = hall_get_sectionnumb;
//     hall->gettick = hall_gettick;
//     #if (ENCODER_TYPE == ENCODER_TYPE_HALL_ABZ)
//         hall->get_abzcount = tim_abzencoder_getcount;
//         hall->set_abzcount = tim_abzencoder_setcount;    
//     #endif
// }
void hall_register(hall_sensor_t *hall,...) 
{
    va_list args;
    va_start(args, hall); // 初始化 args 以读取 hall 之后的参数

    // 初始化 hall 结构体
    memset(hall, 0, sizeof(hall_sensor_t));
    hall->getsection = va_arg(args,uint8_t (*)(void));
    hall->gettick = va_arg(args,uint32_t (*)(void));
 
    // 检查是否设置 ABZ 回调函数
    #if (ENCODER_TYPE == ENCODER_TYPE_HALL_ABZ)
    {
        hall->get_abzcount = va_arg(args,uint32_t (*)(void));
        hall->set_abzcount = va_arg(args,void (*)(uint32_t));
    }
    #endif
    va_end(args);
}
 
void hall_init(void *this)
{
    USER_DEBUG_NORMAL("hall init\n");
    hall_sensor_t *hall;
    hall = this;
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
    lowfilter_init(&(hall->speedfilter), 3.0F);
    lowfilter_init(&(hall->pll_speedfilter),95.0f);
}
void hall_deinit(void *this)
{
    hall_sensor_t *sensor;
    sensor = (hall_sensor_t *)this;
    // 清零数值变量
    sensor->last_section = 0;
    sensor->dir = 0;
    sensor->count = 0;
    for (int i = 0; i < 7; i++) {
        sensor->hall_baseBuff[i] = 0.0f;
    }
    sensor->last_tick = 0;
    sensor->speed = 0.0f;
    sensor->angle = 0.0f;
    sensor->realcacle_angle = 0.0f;
    sensor->realcacle_speed = 0.0f;
    sensor->self_angle = 0.0f;
 
#if (ENCODER_TYPE == ENCODER_TYPE_HALL_ABZ)
    sensor->cur_abzcout = 0;
    sensor->last_abzcout = 0;
#endif
 
    sensor->hat_angle = 0.0f;
    sensor->hat_speed = 0.0f;
    sensor->pll_sum = 0.0f;
 
#ifdef MOTOR_OPENLOOP
    for (int i = 0; i < 7; i++) {
        // 假设 lowfilter_t 有一个合适的清零方法，这里只是示例
        // 如果 lowfilter_t 是简单的数值结构体，你也可以直接清零它的每个成员
        memset(&(sensor->lfilter[i]), 0, sizeof(lowfilter_t)); // 如果 lowfilter_t 可以这样清零
    }
#endif
    // 清零 speedfilter 和 pll_speedfilter，假设它们也是可以直接清零的结构体
    memset(&(sensor->speedfilter), 0, sizeof(lowfilter_t));
    memset(&(sensor->pll_speedfilter), 0, sizeof(lowfilter_t));
    // 或者，如果 lowfilter_t 有特定的清零函数，则调用该函数
    // lowfilter_zero(&sensor->speedfilter);
    // lowfilter_zero(&sensor->pll_speedfilter);
}