#include "i2c_device.h"
#include "i2c_bus.h"

void i2c_device_init(i2c_device_t *dev)
{
    dev->init(dev->bus);
}

void i2c_device_read(void *obj,uint8_t* pdata,uint16_t datalen)
{
    i2c_device_t *dev = (i2c_device_t *)obj;
    dev->bus->read(dev->device_addr,dev->reg_addr,pdata,datalen);
}
void i2c_device_write(void *obj,uint8_t pdata,uint16_t datalen)
{
    i2c_device_t *dev = (i2c_device_t *)obj;
    dev->bus->write(dev->device_addr,dev->reg_addr,pdata,datalen);
}
