#ifndef __HALL_SENSOR__H
#define __HALL_SENSOR__H
#include "board.h"
#include "stdint.h"
#include "filter.h"
#pragma pack(push,4)

typedef struct hall_sensor
{
    /* data */
    uint8_t hall_runflag;
    uint8_t last_section;
    int8_t dir;
    uint32_t count;
    float hall_baseBuff[7];
    uint8_t (*getsection)(void);
    uint32_t (*gettick)(void);

    uint32_t last_tick;
    float speed;
    float angle;
    float realcacle_angle;
    float realcacle_speed;
    float self_angle;

#if (ENCODER_TYPE==ENCODER_TYPE_HALL_ABZ)
    uint32_t (*get_abzcount)(void);
    void     (*set_abzcount)(uint32_t);
    uint32_t cur_abzcout;
    uint32_t last_abzcout;
#endif // (0)

/*PLL*/
    float hat_angle;
    float hat_speed;
    float pll_sum;
/*OUTPUT*/    
#ifdef MOTOR_OPENLOOP 
    lowfilter_t lfilter[7];
#endif // DEBUG  
    lowfilter_t speedfilter;
    lowfilter_t pll_speedfilter;
}hall_sensor_t;
#pragma pack(pop)

uint8_t hall_update(void *pthis);
void hall_cale(void  *pthis);
void hall_init(void *this);

#endif
