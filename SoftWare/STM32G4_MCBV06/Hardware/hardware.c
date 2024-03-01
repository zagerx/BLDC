#include "hardware_cfg.h"
#include "sensor.h"
#include "tle5012b.h"
static sensor_t sg_sensor_tle5012 = {
        .pf_read = tle5012b_read,
        .pf_write = NULL,
        .pf_init = tle5012b_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};

void hw_init(void)
{
    sensor_register(&sg_sensor_tle5012,SENSOR_01);
}

void hw_sensor_process(void)
{
    sensor_process();
}
