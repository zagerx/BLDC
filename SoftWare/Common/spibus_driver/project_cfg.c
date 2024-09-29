#include "bus_driver.h"
#include "spi.h"

bus_dv_t spi0 = {
    0,
    spi1_init,
    spi1_close,
};

void allbus_init_resg(void)
{
    bus_resgiter(&spi0);
    bus_init();
}

#include "sensor.h"
#undef NULL
#define NULL 0




#include "as5047.h"
static sensor_t sg_sensor_as5047 = {
        .pf_read = as5047_readangle,
        .pf_write = NULL,
        .pf_init = as5047_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};


