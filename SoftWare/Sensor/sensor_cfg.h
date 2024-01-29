#ifndef __SENSORT_CFG__H
#define __SENSORT_CFG__H

#ifndef SENSOR_CLOSE_DEBUG
    #include "_common.h"   //Debug打印
#endif
/*---------------------------角度传感器---------------------------------*/
    #define ANGLE_SENSOR_TLE5012B_EN        (0)    //TLE5012B使能
    #define ANGLE_SENSORT_MT6016_EN         (1)    //MT6016使能

    #if(ANGLE_SENSORT_MT6016_EN)
        #include "mt6816.h"
    #elif(ANGLE_SENSOR_TLE5012B_EN)
        #include "tle5012b.h"
    #endif
/*---------------------------温湿度传感器---------------------------------*/


/*----------------------------传感器列表--------------------------------*/
typedef enum{
    SENSOR_NONE = -1,
    SENSOR_01, 
    SENSOR_NUMBER
}ENUM_SENSOR;
#endif
