#include "sensor.h"

#undef  NULL
#define NULL 0
#define SENSORE_PERCI            (1)

static float g_filtedata_arry[SENSOR_NUMBER + 1];        //传感器滤波的数据
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
                // USER_DEBUG_NORMAL("data %d %d\r\n",g_data_arry[sensor_id].raw_buf[2],\    
                //                                     (int)(g_data_arry[sensor_id].covdata_buf[2]));                
            }
            /*----是否滤波----*/
        }
        break;    
    default:
        break;
    }
}

/*-------------用户读取指定传感器-----------*/
void* sensor_user_read(ENUM_SENSOR sensor_id)
{
    static volatile sensor_data_t rawdata;
    if (sensor_id <= SENSOR_NONE || sensor_id > SENSOR_NUMBER)
    {
        return 0;
    }
    sensor_t *pcursensor;
    pcursensor = &g_sensor_arry[sensor_id];

    if (!pcursensor->cycle)
    {
        rawdata = *(sensor_data_t *)pcursensor->pf_read();
    }else{
        rawdata = g_data_arry[sensor_id];        
    }
    return &rawdata;
}

void* sensor_user_write(ENUM_SENSOR sensor_id,int8_t *pdata,uint16_t size)
{

}

void sensor_user_pause(ENUM_SENSOR sensor_id)
{

}


