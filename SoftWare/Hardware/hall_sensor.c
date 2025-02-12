#include "hall_sensor.h"
#include "debuglog.h"
#include "board.h"
#include "stdint.h"
#include "math.h"
#include "string.h"
#include <stdarg.h>
#include "mc_focmath.h"//提供角度归一化函数 TODO

/*==========================================================================================
 * @brief        更新基准角度&&更新速度&&方向
 * @FuncName     
 * @param        hall 
 * @param        cur_sect 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
static void _update_baseangle_speed_dir(hall_sensor_t *obj,uint8_t dir,uint8_t cur_sect)
{
    hall_sensor_t *hall =( hall_sensor_t *)(obj);
    sect_t *psect;
#ifdef HALL_ENABLE_CAIRLBE 
    // USER_DEBUG_NORMAL("%d----->%f\n", cur_sect, lowfilter_cale(&(hall->lfilter[cur_sect]), hall->self_angle));
#endif

    //更新基准角度&方向
    hall->dir = dir;
    if (hall->dir == -1)
    {
        psect = hall->negative_sect;
    }else{
        psect = hall->positive_sect;
    }
    hall->realcacle_angle = psect[cur_sect].angle;
    //更新速度
#if ((ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ))
    hall->realcacle_speed = psect[cur_sect].diff/(HALL_UPDATE_PERIOD * (hall->get_abzcount()));//TODO
#elif(ENCODER_TYPE == ENCODER_TYPE_HALL)
    hall->realcacle_speed = psect[cur_sect].diff/(HALL_UPDATE_PERIOD * hall->count);
#endif
    //更新计数器&上一次扇区
    hall->count = 0;
    hall->last_section = cur_sect;    
}
/*==========================================================================================
 * @brief        更新HALL扇区和方向
 * @FuncName     
 * @param        obj 
 * @return       uint8_t 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
uint8_t hall_update(void *obj)
{
    hall_sensor_t *hall = (hall_sensor_t *)obj;
    uint8_t cur_section = hall->getsection();
    switch (hall->last_section)
    {
    case 6:
        if (cur_section == 4){
            _update_baseangle_speed_dir(hall,1,cur_section);
        }else if (cur_section == 2){
            _update_baseangle_speed_dir(hall,-1,cur_section);
        }else if (cur_section == 6){
            hall->count++;
        }else{
            goto HALL_ERROR;
        }
        break;

    case 4:
        if (cur_section == 5){
            _update_baseangle_speed_dir(hall,1,cur_section);
        }else if (cur_section == 6){
            _update_baseangle_speed_dir(hall,-1,cur_section);
        }else if (cur_section == 4){
            hall->count++;
        }else{
            goto HALL_ERROR;
        }
        break;

    case 5:
        if(cur_section == 1)
        {
            _update_baseangle_speed_dir(hall,1,cur_section);
        }else if (cur_section == 4){
            _update_baseangle_speed_dir(hall,-1,cur_section);
        }else if (cur_section == 5){
            hall->count++;
        }else{
            goto HALL_ERROR;
        }
        break;

    case 1:
        if (cur_section == 3){
            _update_baseangle_speed_dir(hall,1,cur_section);
        }else if (cur_section == 5){
            _update_baseangle_speed_dir(hall,-1,cur_section);
        }else if (cur_section == 1){
            hall->count++;
        }else{
            goto HALL_ERROR;
        }
        break;
    case 3:
        if (cur_section == 2){
            _update_baseangle_speed_dir(hall,1,cur_section);
        }else if (cur_section == 1){
            _update_baseangle_speed_dir(hall,-1,cur_section);
        }else if (cur_section == 3){
            hall->count++;
        }else{
            goto HALL_ERROR;
        }
        break;
    case 2:
        if (cur_section == 6){
            _update_baseangle_speed_dir(hall,1,cur_section);
        }else if (cur_section == 3){
            _update_baseangle_speed_dir(hall,-1,cur_section);
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
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void hall_cale(void *obj)
{
    int32_t  delt_cnt;
    uint32_t cur_cnt;
    hall_sensor_t *hall;
    hall = (hall_sensor_t *)obj;

    #if ((ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ))//ABZ+HALL类传感器
        delt_cnt = hall->get_abzcount()-ABZ_ENCODER_LINES_HALF;
        hall->set_abzcount(ABZ_ENCODER_LINES_HALF);
        float diff = (delt_cnt)*ABZ_ENCODER_RESOLUTION;
        //cacle odom
        hall->odom += diff;
        //cacle angle
        hall->realcacle_angle += diff;
    #elif(ENCODER_TYPE == ENCODER_TYPE_HALL)//HALL类传感器
        hall->realcacle_angle += hall->realcacle_speed * HALL_UPDATE_PERIOD;
        hall->realcacle_angle = _normalize_angle(hall->realcacle_angle);
    #endif

#ifdef HALL_ENABLE_PLL//是否启用PLL功能
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
    hall->angle = hall->hat_angle;
    hall->speed = hall->hat_speed;
    return;
#endif

    //update angle&speed to sensor
    hall->angle = hall->realcacle_angle;
    hall->speed = hall->realcacle_speed;
}
/*==========================================================================================
 * @brief 
 * @FuncName     hall_get_initpos
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void hall_get_initpos(void *obj)
{
    hall_sensor_t *hall;
    hall = (hall_sensor_t*)obj;
    uint8_t cur_sect = hall->getsection();
    #if ((ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ))//ABZ+HALL类传感器
        hall->realcacle_angle = _normalize_angle(hall->negative_sect[cur_sect].angle + 0.523f);
    #elif(ENCODER_TYPE == ENCODER_TYPE_HALL)//HALL类传感器
        hall->realcacle_angle = _normalize_angle(hall->negative_sect[cur_sect].angle + 0.523f);
    #endif    
}
/*==========================================================================================
 * @brief 
 * @FuncName     hall_get_initpos
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void hall_set_calib_points(void *obj)
{
    hall_sensor_t *hall;
    hall = (hall_sensor_t*)obj;
    uint8_t cur_sect = hall->getsection();
    #if ((ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ))//ABZ+HALL类传感器
    #elif(ENCODER_TYPE == ENCODER_TYPE_HALL)//HALL类传感器
    #endif
}

 /*==========================================================================================
  * @brief 
  * @FuncName     
  * @param        obj 
  * @version      0.1
 --------------------------------------------------------------------------------------------*/
void hall_init(void *obj)
{
    hall_sensor_t *hall;
    hall = (hall_sensor_t*)obj;

    hall->realcacle_speed = 0.0f;
    hall->count = 0.0f;
    hall->hallerr_count = 0;
    hall->realcacle_angle = 0.0f;

    {//================================初始化基准角度=======================================//
        #if ((ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ))//ABZ+HALL类传感器
            hall->set_abzcount(ABZ_ENCODER_LINES_HALF);
        #elif(ENCODER_TYPE == ENCODER_TYPE_HALL)//HALL类传感器
        #endif
        hall->positive_sect[6].angle = SCETION_6_BASEANGLE + HALL_POSITIVE_OFFSET;
        hall->positive_sect[4].angle = SCETION_4_BASEANGLE + HALL_POSITIVE_OFFSET;
        hall->positive_sect[5].angle = SCETION_5_BASEANGLE + HALL_POSITIVE_OFFSET;
        hall->positive_sect[1].angle = SCETION_1_BASEANGLE + HALL_POSITIVE_OFFSET;
        hall->positive_sect[3].angle = SCETION_3_BASEANGLE + HALL_POSITIVE_OFFSET;
        hall->positive_sect[2].angle = SCETION_2_BASEANGLE + HALL_POSITIVE_OFFSET;
        hall->positive_sect[6].diff = _normalize_angle(hall->positive_sect[6].angle - hall->positive_sect[2].angle);
        hall->positive_sect[4].diff = _normalize_angle(hall->positive_sect[4].angle - hall->positive_sect[6].angle);
        hall->positive_sect[5].diff = _normalize_angle(hall->positive_sect[5].angle - hall->positive_sect[4].angle);
        hall->positive_sect[1].diff = _normalize_angle(hall->positive_sect[1].angle - hall->positive_sect[5].angle);
        hall->positive_sect[3].diff = _normalize_angle(hall->positive_sect[3].angle - hall->positive_sect[1].angle);
        hall->positive_sect[2].diff = _normalize_angle(hall->positive_sect[2].angle - hall->positive_sect[3].angle);

        hall->negative_sect[6].angle = hall->positive_sect[4].angle + HALL_NEGATIVE_OFFSET;
        hall->negative_sect[4].angle = hall->positive_sect[5].angle + HALL_NEGATIVE_OFFSET;
        hall->negative_sect[5].angle = hall->positive_sect[1].angle + HALL_NEGATIVE_OFFSET;
        hall->negative_sect[1].angle = hall->positive_sect[3].angle + HALL_NEGATIVE_OFFSET;
        hall->negative_sect[3].angle = hall->positive_sect[2].angle + HALL_NEGATIVE_OFFSET;
        hall->negative_sect[2].angle = hall->positive_sect[6].angle + HALL_NEGATIVE_OFFSET;
        hall->negative_sect[6].diff = -hall->positive_sect[5].diff;
        hall->negative_sect[4].diff = -hall->positive_sect[1].diff;
        hall->negative_sect[5].diff = -hall->positive_sect[3].diff;
        hall->negative_sect[1].diff = -hall->positive_sect[2].diff; 
        hall->negative_sect[3].diff = -hall->positive_sect[6].diff;
        hall->negative_sect[2].diff = -hall->positive_sect[4].diff;        
    }

    {//================================初始化滤波器=======================================//
        #ifdef HALL_ENABLE_CAIRLBE
            for (uint8_t i = 0; i < 7; i++)
            {
                lowfilter_init(&(hall->lfilter[i]), 5.0F);
            }
        #endif // DEBUG
            lowfilter_init(&(hall->speedfilter), 3.0F);
            lowfilter_init(&(hall->pll_speedfilter),95.0f);
    }

}
/*==========================================================================================
 * @brief 
 * @FuncName     
 * @param        obj 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void hall_deinit(void *obj)
{
    hall_sensor_t *sensor;
    sensor = (hall_sensor_t *)obj;
    sensor->last_section = 0;
    sensor->dir = 0;
    sensor->count = 0;
    sensor->hallerr_count = 0;
    sensor->realcacle_angle = 0.0f;
    sensor->realcacle_speed = 0.0f;

#ifdef HALL_ENABLE_PLL
    sensor->hat_angle = 0.0f;
    sensor->hat_speed = 0.0f;
    sensor->pll_sum = 0.0f;
#endif

#ifdef HALL_ENABLE_CAIRLBE
    for (int i = 0; i < 7; i++) {
        memset(&(sensor->lfilter[i]), 0, sizeof(lowfilter_t)); 
    }
#endif
    memset(&(sensor->speedfilter), 0, sizeof(lowfilter_t));
    memset(&(sensor->pll_speedfilter), 0, sizeof(lowfilter_t));
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
    va_end(args);
    hall->cacle = hall_cale;
    hall->update_base = hall_update;
    hall->init = hall_init;
    hall->deinit = hall_deinit;
    hall->get_first_points = hall_get_initpos;
    hall->set_calib_points = hall_set_calib_points;
}
