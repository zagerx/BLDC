#ifndef __HALL_SENSOR__H
#define __HALL_SENSOR__H
#include "board.h"
#include "stdint.h"
#include "filter.h"
#pragma pack(push, 4)

typedef struct hall_sensor
{
    /* data */
    uint8_t last_section;    // 上次扇区
    int8_t dir;              // 当前方向
    uint32_t count;          // Hall类传感器的计数器
    float hall_baseBuff[7];  // 基本角度
    float cairlbe_angle;     // 校准角度
    uint8_t cairlbe_section; // 校准扇区
    uint16_t hallerr_count;
    float offset;

    uint8_t  (*getsection)(void);
    uint32_t (*gettick)(void);
#if (ENCODER_TYPE == ENCODER_TYPE_HALL_ABZ)
    uint32_t (*get_abzcount)(void);
    void (*set_abzcount)(uint32_t);
#endif // (0)
    void (*cacle)(void*);
    uint8_t (*update_base)(void*);
    void (*init)(void*);
    void (*deinit)(void*);
    void (*get_first_points)(void*);
    void (*set_calib_points)(void*);

    /*PLL*/
    float hat_angle;
    float hat_speed;
    float pll_sum;

    float realcacle_angle;
    float realcacle_speed;

    float speed;
    float angle;
    float self_angle;
/*OUTPUT*/

#if(MOTOR_WORK_MODE == MOTOR_DEBUG_SELF_MODE)//后续需要删除
    lowfilter_t lfilter[7];
#endif // DEBUG
    lowfilter_t speedfilter;
    lowfilter_t pll_speedfilter;
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
