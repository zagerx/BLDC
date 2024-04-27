#include "hardware_cfg.h"

#undef NULL
#define NULL 0
static sensor_t sg_sensor_mt6816 = {
        .pf_read = as5047_read,
        .pf_write = NULL,
        .pf_init = as5047_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};

void hw_init(void)
{
    sensor_register(&sg_sensor_mt6816,SENSOR_01);
}

void hw_sensor_process(void)
{
    sensor_process();
}
