#include "sensor.h"
#include "sensor_cfg.h"

#undef  NULL
#define NULL 0
#define SENSORE_PERCI            (1)

static sensor_data_t g_data_arry[SENSOR_NUMBER + 1];     //传感器数据

static sensor_t g_sensor_arry[SENSOR_NUMBER + 1];

void sensor_register(sensor_t *this,ENUM_SENSOR index)
{
    if (index > SENSOR_NUMBER || index <= SENSOR_NONE)
    {
        return;//注册失败
    }
    g_sensor_arry[index] = *this;
}

enum{
    SENSOR_INIT,
    SENSOR_IDLE,
    SENSOR_OPEN,
    SENSOR_UPDATE,
    SENSOR_CLOSE,
};
typedef struct
{
    unsigned int tim;
    unsigned char state;
}sensor_arry_t;

sensor_arry_t g_sensor = {
    .tim = 0,
    .state = SENSOR_INIT,
};
/*-------------初始化、更新传感器组----------------*/
void sensor_process(void)
{
    switch (g_sensor.state)
    {
    case SENSOR_INIT:
        for (ENUM_SENSOR sensor_id = 0; sensor_id < SENSOR_NUMBER; sensor_id++) //遍历整个传感器数组
        {
            if (!g_sensor_arry[sensor_id].pf_init)
            {
                continue;
            }
            g_sensor_arry[sensor_id].pf_init();
        }
        g_sensor.state = SENSOR_UPDATE;
        break;
    case SENSOR_UPDATE:
        g_sensor.tim++;
        for (ENUM_SENSOR sensor_id = 0; sensor_id < SENSOR_NUMBER; sensor_id++) //遍历整个传感器数组
        {
            if (!g_sensor_arry[sensor_id].pf_read || !g_sensor_arry[sensor_id].cycle)
            {
                continue;
            }
            /*----更新周期是否满足----*/
            if (!(g_sensor.tim % g_sensor_arry[sensor_id].cycle))
            {
                g_data_arry[sensor_id] = *(sensor_data_t *)g_sensor_arry[sensor_id].pf_read();
            }
        }
        break;    
    default:
        break;
    }
}

/*-------------用户读取指定传感器-----------


    test_angle = ((int32_t*)sensor_user_read(SENSOR_02,EN_SENSORDATA_COV))[0];


*-----------------------------------------*/
void* sensor_user_read(ENUM_SENSOR sensor_id,sensordata_type _type)
{
    static volatile sensor_data_t rawdata;
    if (sensor_id <= SENSOR_NONE || sensor_id > SENSOR_NUMBER)
    {
        return (void*)0;
    }
    sensor_t *pcursensor;
    pcursensor = &g_sensor_arry[sensor_id];

    if (!pcursensor->cycle)
    {
        rawdata = *(sensor_data_t *)pcursensor->pf_read();
    }else{
        rawdata = g_data_arry[sensor_id];        
    }

    if (_type == EN_SENSORDATA_COV)
    {
        return (void*)(rawdata.covdata_buf);
    }

    if (_type == EN_SENSORDATA_RAW)
    {
        return (void*)(rawdata.raw_buf);
    }

    if (_type == EN_SENSORDATA_FILTER)
    {
        return (void*)(rawdata.filterdata_buf);
    }
    return 0;
}

void* sensor_user_write(ENUM_SENSOR sensor_id,int8_t *pdata,uint16_t size)
{
    return (void*)0;
}

void sensor_user_pause(ENUM_SENSOR sensor_id)
{

}


