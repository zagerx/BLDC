#ifndef __HALL_SENSOR__H
#define __HALL_SENSOR__H
#include "board.h"
#include "stdint.h"
#include "filter.h"
#pragma pack(push, 4)
typedef struct _sect
{
    /* data */
    // uint8_t cur_sect;
    uint8_t last_sect;
    uint8_t next_sect;
    float angle;
    float diff;
}sect_t;

typedef struct hall_sensor
{
    /* data */
    uint8_t last_section;    // 上次扇区
    int8_t dir;              // 当前方向
    uint32_t count;          // Hall类传感器的计数器
    float cairlbe_angle;     // 校准角度
    uint8_t cairlbe_section; // 校准扇区
    uint16_t hallerr_count;
    sect_t positive_sect[7];
    sect_t negative_sect[7];
#ifdef HALL_ENABLE_CAIRLBE
    float self_angle;
    lowfilter_t lfilter[7];
#endif
#ifdef HALL_ENABLE_PLL
    float hat_angle;
    float hat_speed;
    float pll_sum;
#endif

    float realcacle_angle;
    float realcacle_speed;

    float speed;
    float angle;
    float odom;
    /*OUTPUT*/
    lowfilter_t speedfilter;
    lowfilter_t pll_speedfilter;


    uint8_t  (*getsection)(void);
    uint32_t (*gettick)(void);
#if (ENCODER_TYPE == ENCODER_TYPE_HALL_ABZ)
    uint32_t (*get_abzcount)(void);
    void (*set_abzcount)(uint32_t);
#endif
    void (*cacle)(void*);
    uint8_t (*update_base)(void*);
    void (*init)(void*);
    void (*deinit)(void*);
    void (*get_first_points)(void*);
    void (*set_calib_points)(void*);
} hall_sensor_t;
#pragma pack(pop)

uint8_t hall_update(void *pthis);
void hall_cale(void *pthis);
void hall_get_initpos(void *pthis);
void hall_set_calib_points(void *pthis);

void hall_init(void *this);
void hall_register(hall_sensor_t *hall, ...);
void hall_deinit(void *this);

#endif
