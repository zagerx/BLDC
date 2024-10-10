#include "i2c_device.h"
#include "i2c_bus.h"
void i2c_device_register(i2c_device_t *dev,i2c_bus_t *bus)
{
    dev->bus = bus;

}

void i2c_device_read(char *name,void *pdata,uint16_t data)
{
    i2c_device_t *dev;
    /*根据名字获取对应的dev*/
    dev->read(dev);
}
