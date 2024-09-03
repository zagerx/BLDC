#ifndef __BLDCMOTOR_CFG__H
#define __BLDCMOTOR_CFG__H

#include "adc.h"
#include "tim.h"
#include "math.h"
#include "sensor.h"

#include "pid.h"
#include "filter.h"
#include "debuglog.h"
// #include "perf_counter.h"
#include "motorctrl_common.h"

#include "board.h"

#ifndef BOARD_MOTORCTRL_MODE
    #pragma message("ERR!!!------no param for motorctrl mode------please check!!!")
#endif




__attribute__((weak)) void motor_enable(void);
__attribute__((weak)) void motor_disable(void);
__attribute__((weak)) void motor_set_pwm(float _a,float _b,float _c);
__attribute__((weak)) void _bsp_protransmit(unsigned char* pdata,unsigned short len);



#endif
