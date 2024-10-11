#include "ina226.h"
#include "tac9539pwr.h"
#include "i2c_device.h"
#include "i2c_bus.h"
#include "i2c.h"
static i2c_bus_t i2c_bus1 = {
    .init = i2c_bus1_init,
    .read = i2c_bus1_read,
    .write = i2c_bus1_write,
};
static i2c_device_t ina226 = {
    .bus = &i2c_bus1,
    .init = dev_ina226_init,
};

static i2c_device_t dev_tca9538 = {
    .bus = &i2c_bus1,
    .init = tca9539pwr_init,
};
void pro_devinit(void)
{
    i2c_bus1.init();
    i2c_device_init(&ina226);
    i2c_device_init(&dev_tca9538);
}
