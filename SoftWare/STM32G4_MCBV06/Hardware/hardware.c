#include "hardware_cfg.h"
#include "sensor.h"
#include "tle5012b.h"
#include "hallencoder.h"
static sensor_t sg_sensor_tle5012 = {
        .pf_read = tle5012b_read,
        .pf_write = NULL,
        .pf_init = tle5012b_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};
static sensor_t sg_hallencoder = {
        .pf_read = hallencoder_readangle,
        .pf_write = NULL,
        .pf_init = hallencoder_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};
void hw_init(void)
{
    sensor_register(&sg_hallencoder,SENSOR_01);
}

void hw_sensor_process(void)
{
    sensor_process();
}
