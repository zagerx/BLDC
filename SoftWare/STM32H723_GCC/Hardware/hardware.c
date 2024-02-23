#include "hardware_cfg.h"

static sensor_t sg_sensor_ina226 = {
        .pf_read = ina226_read_data,
        .pf_write = NULL,
        .pf_init = ina226_init,
        .cycle = 500,
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
    sensor_register(&sg_sensor_ina226,SENSOR_02);
}

    






