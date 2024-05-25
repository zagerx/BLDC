#include "hardware_cfg.h"
#include "as5047.h"
#include "adc.h"
#undef NULL
#define NULL 0
static sensor_t sg_sensor_mt6816 = {
        .pf_read = mt6816_read,
        .pf_write = NULL,
        .pf_init = mt6816_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};
static sensor_t sg_sensor_as5047 = {
        .pf_read = as5047_readangle,
        .pf_write = NULL,
        .pf_init = as5047_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};

static sensor_t sg_sensor_vbus = {
        .pf_read = adc_readvbus,
        .pf_write = NULL,
        .pf_init = adc_vbusinit,
        .cycle = 2,
        .status = EN_SENSOR_INIT
};
void hw_init(void)
{
    // sensor_register(&sg_sensor_mt6816,SENSOR_01);
    sensor_register(&sg_sensor_as5047,SENSOR_01);
    sensor_register(&sg_sensor_vbus,SENSOR_02);

}

void hw_sensor_process(void)
{
    sensor_process();
}
