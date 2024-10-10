#ifndef __I2C_DRVICE__H
#define __I2C_DRVICE__H
#include "stdint.h"
typedef struct i2c_device
{
    char *i2c_bus_name;
    uint8_t i2c_bus_id;
    void (*init)(void);
    void (*read)(uint8_t,uint8_t);
    void (*write)(uint8_t,uint8_t);    
}i2c_device_t;


#endif
