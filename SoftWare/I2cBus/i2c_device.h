#ifndef __I2C_DRVICE__H
#define __I2C_DRVICE__H
#include "stdint.h"
#include "i2c_bus.h"
typedef struct i2c_device
{
    // uint16_t device_addr;
    // uint8_t reg_addr;
    // uint8_t pdata;
    // uint16_t datalen;

    void (*read)(void *,uint8_t);
    void (*write)(uint8_t *,uint8_t); 
    void (*init)(i2c_bus_t*);
    
    i2c_bus_t *bus;
}i2c_device_t;

void i2c_device_init(i2c_device_t *dev);

#endif
