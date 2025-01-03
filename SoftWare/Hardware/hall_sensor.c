#include "hall_sensor.h"
#include "debuglog.h"
#include "board.h"
#include "stdint.h"
#include "math.h"
#include "string.h"
#include <stdarg.h>
/*==========================================================================================
 * @brief        更新基准角度&&更新速度
 * @FuncName     
 * @param        hall 
 * @param        cur_sect 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
static void _update_base_dir_angle(hall_sensor_t *hall,uint8_t cur_sect)
{
    float delta, real_calc_speed;
 #if( MOTOR_OPENLOOP && !MOTOR_OPENLOOP_ENCODER)//CPU负担过重，只能在自开环条件下打开
    float test_temp[7];
    float temp_self = hall->self_angle;
    test_temp[cur_sect] = lowfilter_cale(&(hall->lfilter[cur_sect]), temp_self);
    static float temp_curbase,temp_base_buf;
    static uint8_t temp_cursect;
    temp_curbase = test_temp[cur_sect];
    temp_base_buf = hall->hall_baseBuff[cur_sect];
    temp_cursect = cur_sect;
    USER_DEBUG_NORMAL("%d----->%f\n", cur_sect, test_temp[cur_sect]);
#endif

#if ((ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ))
    #ifdef MCB_V06
        // 使用霍尔进行校准
        if (cur_sect == 6)
        {
            hall->realcacle_angle = hall->hall_baseBuff[cur_sect] + (hall->dir > 0 ? HALL_POSITIVE_OFFSET : HALL_NEGATIVE_OFFSET);
        }
    #else
        if (cur_sect == 6)
        {
            hall->realcacle_angle = hall->hall_baseBuff[cur_sect] + (hall->dir > 0 ? HALL_POSITIVE_OFFSET : HALL_NEGATIVE_OFFSET);
        }
    #endif
#elif(ENCODER_TYPE == ENCODER_TYPE_HALL)
    //Cacle speed
    delta = hall->hall_baseBuff[cur_sect] - hall->hall_baseBuff[hall->last_section];
    if (hall->dir > 0) {
        if (delta < 0.0f) {
            delta += 6.283185f;
        }
    } else {
        if (delta > 0.0f) {
            delta -= 6.283185f;
        }
    }
    real_calc_speed = 1 * (delta / (HALL_UPDATE_PERIOD * hall->count));
    hall->realcacle_speed = lowfilter_cale(&(hall->speedfilter), real_calc_speed); 
#endif
    hall->count = 0;
    hall->last_section = cur_sect;
}
/*==========================================================================================
 * @brief        更新HALL扇区和方向
 * @FuncName     
 * @param        pthis 
 * @return       uint8_t 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
uint8_t hall_update(void *pthis)
{
    hall_sensor_t *hall;
    hall = (hall_sensor_t *)pthis;
    uint8_t cur_section = hall->getsection();
    switch (hall->last_section)
    {
    case 6:
        if (cur_section == 4){
            hall->dir = 1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 2){
            hall->dir = -1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 6){
            hall->count++;
        }else{
            goto HALL_ERROR;
        }
        break;

    case 4:
        if (cur_section == 5){
            hall->dir = 1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 6){
            hall->dir = -1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 4){
            hall->count++;
        }else{
            goto HALL_ERROR;
        }
        break;

    case 5:
        if(cur_section == 1)
        {
            hall->dir = 1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 4){
            hall->dir = -1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 5){
            hall->count++;
        }else{
            goto HALL_ERROR;
        }
        break;

    case 1:
        if (cur_section == 3){
            hall->dir = 1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 5){
            hall->dir = -1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 1){
            hall->count++;
        }else{
            goto HALL_ERROR;
        }
        break;
    case 3:
        if (cur_section == 2){
            hall->dir = 1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 1){
            hall->dir = -1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 3){
            hall->count++;
        }else{
            goto HALL_ERROR;
        }
        break;
    case 2:
        if (cur_section == 6){
            hall->dir = 1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 3){
            hall->dir = -1;
            _update_base_dir_angle(hall,cur_section);
        }else if (cur_section == 2){
            hall->count++;
        }else{
            goto HALL_ERROR;
        }
        break;
    default:
        hall->last_section = cur_section;
        break;
    }
    return 0;

HALL_ERROR:
    USER_DEBUG_NORMAL("HALL err\n");  
    hall->hallerr_count++;  
}
/*==========================================================================================
 * @brief        计算Hall角度
 * @FuncName     
 * @param        pthis 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void hall_cale(void *pthis)
{
    static volatile int32_t  delt_cnt;
    uint32_t cur_cnt;
    hall_sensor_t *hall;
    hall = (hall_sensor_t *)pthis;

    #if ((ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ))//ABZ+HALL类传感器
        delt_cnt = hall->get_abzcount()-ABZ_ENCODER_LINES_HALF;
        hall->set_abzcount(ABZ_ENCODER_LINES_HALF);
        float diff = (delt_cnt)*ABZ_ENCODER_RESOLUTION;
        hall->realcacle_angle += diff;
        hall->realcacle_angle = fmodf(hall->realcacle_angle,6.283185f);
        hall->realcacle_angle += (hall->realcacle_angle < 0.0f) ? 6.283185f : 0.0f;
        hall->angle = hall->realcacle_angle;

        //cacle speed  TODO
        static uint8_t cnt = 0;
        static float speed_sum = 0.0f;
        speed_sum += diff;
        if (cnt++>=10)
        {
            hall->realcacle_speed =  (speed_sum/0.001f);
            cnt = 0;
            speed_sum = 0.0f;
        }
        hall->speed = hall->realcacle_speed;
       
    #elif(ENCODER_TYPE == ENCODER_TYPE_HALL)//HALL类传感器
        hall->realcacle_angle += hall->realcacle_speed * HALL_UPDATE_PERIOD;
        if (hall->realcacle_angle > 6.2831852f)
        {
            hall->realcacle_angle -= 6.2831852f;
        }
        if (hall->realcacle_angle < 0.0f)
        {
            hall->realcacle_angle += 6.2831852f;
        }

        //PLL
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
        hall->hat_speed = omega;
        
        //update angle&speed to sensor
        hall->angle = hall->realcacle_angle;
        hall->speed = hall->realcacle_speed;
    #endif
}

void hall_get_initpos(void *pthis)
{
    hall_sensor_t *hall;
    hall = (hall_sensor_t*)pthis;
    uint8_t cur_sect = hall->getsection();
    hall->last_section = cur_sect;
    hall->realcacle_angle = hall->hall_baseBuff[cur_sect] + 1.0471f;
    hall->offset = hall->hall_baseBuff[cur_sect];
    USER_DEBUG_NORMAL("hall_get_initpos %d,%f\n",cur_sect,hall->offset);
}
void hall_set_calib_points(void *pthis)
{
    hall_sensor_t *hall;
    hall = (hall_sensor_t*)pthis;
    uint8_t cur_sect = hall->getsection();
    #if ((ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ))//ABZ+HALL类传感器
        hall->cairlbe_angle = 0.0f;
    #elif(ENCODER_TYPE == ENCODER_TYPE_HALL)//HALL类传感器
        hall->cairlbe_angle = hall->hall_baseBuff[cur_sect] + 1.0471f;
    #endif
    hall->cairlbe_section = cur_sect;
    USER_DEBUG_NORMAL("hall_set_calib_points  %d  %f\n",cur_sect,hall->cairlbe_angle);
}

/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        hall 
 * @param        ... 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void hall_register(hall_sensor_t *hall,...) 
{
    va_list args;
    va_start(args, hall);
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
    hall->cacle = va_arg(args,void (*)(void*));
    hall->update_base = va_arg(args,uint8_t (*)(void*));
    hall->init = va_arg(args,void (*)(void*));
    hall->deinit = va_arg(args,void (*)(void*));
    hall->get_first_points = va_arg(args,void (*)(void*));
    hall->set_calib_points = va_arg(args,void (*)(void*));

    va_end(args);
}
 /*==========================================================================================
  * @brief 
  * @FuncName     
  * @param        this 
  * @version      0.1
 --------------------------------------------------------------------------------------------*/
void hall_init(void *this)
{
    USER_DEBUG_NORMAL("hall init\n");
    hall_sensor_t *hall;
    hall = (hall_sensor_t*)this;
    #if (ENCODER_TYPE == ENCODER_TYPE_HALL_ABZ)
    {
      hall->set_abzcount(ABZ_ENCODER_LINES_HALF);
    }
    #endif
    hall->realcacle_speed = 0.0f;
    hall->count = 0.0f;
    hall->hallerr_count = 0;
    hall->speed = 0.0f;
    hall->cairlbe_angle = 0.0f;
    hall->offset = 0.0f;
    hall->realcacle_angle = 0.0f;
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
/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        this 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void hall_deinit(void *this)
{
    hall_sensor_t *sensor;
    sensor = (hall_sensor_t *)this;
    sensor->last_section = 0;
    sensor->dir = 0;
    sensor->count = 0;
    sensor->hallerr_count = 0;
    for (int i = 0; i < 7; i++) {
        sensor->hall_baseBuff[i] = 0.0f;
    }
    sensor->speed = 0.0f;
    sensor->angle = 0.0f;
    sensor->realcacle_angle = 0.0f;
    sensor->realcacle_speed = 0.0f;
    sensor->self_angle = 0.0f;
  
    sensor->hat_angle = 0.0f;
    sensor->hat_speed = 0.0f;
    sensor->pll_sum = 0.0f;
 
#ifdef MOTOR_OPENLOOP
    for (int i = 0; i < 7; i++) {
        memset(&(sensor->lfilter[i]), 0, sizeof(lowfilter_t)); 
    }
#endif
    memset(&(sensor->speedfilter), 0, sizeof(lowfilter_t));
    memset(&(sensor->pll_speedfilter), 0, sizeof(lowfilter_t));
}