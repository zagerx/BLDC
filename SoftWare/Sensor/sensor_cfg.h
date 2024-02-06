#ifndef __SENSORT_CFG__H
#define __SENSORT_CFG__H

#ifndef SENSOR_CLOSE_DEBUG
    #include "_common.h"   //Debug打印
#endif
/*---------------------------角度传感器---------------------------------*/
#if(ANGLE_SENSOR)
    #include "hardware.h"
#endif
/*---------------------------温湿度传感器---------------------------------*/


/*----------------------------传感器列表--------------------------------*/
typedef enum{
    SENSOR_NONE = -1,
    SENSOR_01, //角度传感器
    SENSOR_NUMBER
}ENUM_SENSOR;
#endif
