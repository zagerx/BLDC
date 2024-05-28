#include "board.h"
/************************传感器*****************************/
#include "sensor.h"
#include "as5047.h"
#undef NULL
#define NULL 0

static sensor_t sg_sensor_as5047 = {
        .pf_read = as5047_readangle,
        .pf_write = NULL,
        .pf_init = as5047_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};

void board_init(void)
{
    sensor_register(&sg_sensor_as5047,SENSOR_01);
}

void board_deinit(void)
{

}
