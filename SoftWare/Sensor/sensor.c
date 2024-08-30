#include "sensor.h"
#include "taskmodule.h"
#undef  NULL
#define NULL 0
#define SENSORE_PERCI            (1)

enum{
    INIT,
    IDLE,
    OPEN,
    UPDATE,
    CLOSE,
};
typedef struct
{
    unsigned int tim;
    unsigned char state;
}sens_fsm_t;

static sensor_t gsensor_arry[SENSOR_NUMBER + 1];
static sens_fsm_t gsens_fsm = {
    .tim = 0,
    .state = INIT,
};

void sensor_register(sensor_t *this,sens_idx_t index)
{
    if (index > SENSOR_NUMBER || index <= SENSOR_NONE)
    {
        return;
    }
    gsensor_arry[index] = *this;
}

void sensor_process(void)
{
    switch (gsens_fsm.state)
    {
    case INIT:
        for (sens_idx_t sensor_id = 0; sensor_id < SENSOR_NUMBER; sensor_id++) //遍历整个传感器数组
        {
            if (!gsensor_arry[sensor_id].pf_init)
            {
                continue;
            }
            gsensor_arry[sensor_id].pf_init();
        }
        gsens_fsm.state = UPDATE;
        break;
    case UPDATE:
        gsens_fsm.tim++;
        for (sens_idx_t sensor_id = 0; sensor_id < SENSOR_NUMBER; sensor_id++) //遍历整个传感器数组
        {
            if (!gsensor_arry[sensor_id].pf_read || !gsensor_arry[sensor_id].cycle)
            {
                continue;
            }
            if (!(gsens_fsm.tim % gsensor_arry[sensor_id].cycle))
            {
                gsensor_arry[sensor_id].pdata = gsensor_arry[sensor_id].pf_read();
            }
        }
        break;    
    default:
        break;
    }
}

void* sensor_user_read(sens_idx_t sensor_id)
{
    if (sensor_id <= SENSOR_NONE || sensor_id > SENSOR_NUMBER)
    {
        return (void*)0;
    }
    sensor_t *pcursensor;
    pcursensor = &gsensor_arry[sensor_id];

    if (!pcursensor->cycle)
    {
         return pcursensor->pf_read();
    }else{
        return pcursensor->pdata;
    }
}

void* sensor_user_write(sens_idx_t sensor_id,int8_t *pdata,uint16_t size)
{
    return (void*)0;
}

void sensor_user_pause(sens_idx_t sensor_id)
{

}

board_task(sensor_process)