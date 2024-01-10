#ifndef __SENSORT_CFG__H
#define __SENSORT_CFG__H

#ifndef SENSOR_CLOSE_DEBUG
    #include "pubilc.h"   //Debug打印
#endif
/*---------------------------角度传感器---------------------------------*/
    #define ANGLE_SENSOR_NONE               (0)
    #define ANGLE_SENSOR_MT6818             (1)
    #define ANGLE_SENSOR_TLE5012B           (2)
    #define ANGLE_SENSOR                    ANGLE_SENSOR_MT6818
    #if(ANGLE_SENSOR == ANGLE_SENSOR_MT6818)
        #include "mt6816.h"
    #elif(ANGLE_SENSOR == ANGLE_SENSOR_TLE5012B)
        #include "tle5012b.h"
    #endif
/*---------------------------温湿度传感器---------------------------------*/





/*----------------------------传感器列表--------------------------------*/
typedef enum{
    SENSOR_NONE = -1,
#if(ANGLE_SENSOR == ANGLE_SENSOR_MT6818)
    SENSOR_MT6818,
#elif(ANGLE_SENSOR == ANGLE_SENSOR_TLE5012B)
    SENSOR_TLE5012B,
#endif
    SENSOR_NUMBER
}ENUM_SENSOR;
#endif
