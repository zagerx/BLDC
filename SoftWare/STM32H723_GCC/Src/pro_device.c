#include "ian226.h"
#include "i2c_device.h"
#include "i2c_bus.h"
#include "i2c.h"
static i2c_bus_t i2c_bus1 = {
    .init = i2c_bus1_init,
    .read = i2c_bus1_read,
    .write = i2c2_write,
};
static i2c_device_t ina226 = {
    .bus = 
};

void pro_devinit(void)
{
    i2c_device_res(&ina226,&i2c_bus1);
    i2c_device_read("ina226",pdata,datalen);
}
