#ifndef __I2C_BUS__H
#define __I2C_BUS__H
#include "stdint.h"

typedef struct i2c_bus
{
    void (*init)(void);
    void (*read)(uint8_t,uint8_t);
    void (*write)(uint8_t,uint8_t);
}i2c_bus_t;

void i2c_bus1_init(void);
void i2c_bus1_read(uint8_t addr,uint8_t *pdata,uint8_t datalen);
void i2c_bus1_write(uint8_t addr,uint8_t *pdata,uint8_t datalen);

#endif
