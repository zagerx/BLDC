#ifndef __BLDCMOTOR_CFG__H
#define __BLDCMOTOR_CFG__H

#include "adc.h"
#include "tim.h"
#include "math.h"
#include "sensor.h"

#include "pid.h"
#include "filter.h"
#include "IQmathLib.h"
#include "debuglog.h"
#include "board.h"

#ifndef BOARD_MOTORCTRL_MODE
    #pragma message("ERR!!!------no param for motorctrl mode------please check!!!")
#endif

#endif
