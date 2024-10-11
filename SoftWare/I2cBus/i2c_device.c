#include "i2c_device.h"
#include "i2c_bus.h"

void i2c_device_init(i2c_device_t *dev)
{
    dev->init(dev->bus);
}
