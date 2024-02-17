#include "hardware_cfg.h"

static sensor_t sg_sensor_ina226 = {
        .pf_read = ina226_read_data,
        .pf_write = NULL,
        .pf_init = ina226_init,
        .cycle = 1000,
        .status = EN_SENSOR_INIT
};

void hw_init(void)
{
    sensor_register(&sg_sensor_ina226,SENSOR_01);
}

void hw_sensor_process(void)
{
    sensor_process();
}
