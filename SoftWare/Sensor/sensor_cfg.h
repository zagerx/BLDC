#ifndef __SENSORT_CFG__H
#define __SENSORT_CFG__H

#ifndef SENSOR_CLOSE_DEBUG
    #include "pubilc.h"   //Debug打印
#endif
/*---------------------------角度传感器---------------------------------*/
    #define ANGLE_SENSOR_TLE5012B_EN        (0)    //TLE5012B使能
    #define ANGLE_SENSORT_MT6016_EN         (1)    //MT6016使能

    #define ANGLE_SENSOR_01                 (1)
    #define ANGLE_SENSOR_02                 (1)

    #if(ANGLE_SENSOR_01 && ANGLE_SENSORT_MT6016_EN)
        #include "mt6816.h"
    #elif(ANGLE_SENSOR_02 && ANGLE_SENSOR_TLE5012B_EN)
        #include "tle5012b.h"
    #endif
/*---------------------------温湿度传感器---------------------------------*/


/*----------------------------传感器列表--------------------------------*/
typedef enum{
    SENSOR_NONE = -1,
#if (ANGLE_SENSOR_01)
    SENSOR_01, 
#endif 
#if (ANGLE_SENSOR_02)
    SENSOR_02, 
#endif
    SENSOR_NUMBER
}ENUM_SENSOR;
#endif
