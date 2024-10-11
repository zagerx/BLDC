#ifndef __I2C_BUS__H
#define __I2C_BUS__H
#include "stdint.h"

typedef struct i2c_bus
{
    void (*init)(void);
    void (*read)(uint16_t,uint8_t,uint8_t*,uint16_t);
    void (*write)(uint16_t,uint8_t,uint8_t*,uint16_t);
}i2c_bus_t;

void i2c_bus1_init(void);
void i2c_bus1_read(uint16_t DevAddress, uint8_t register_addr,uint8_t *pData, uint16_t Size);
void i2c_bus1_write(uint16_t DevAddress, uint8_t register_addr,uint8_t *pData, uint16_t Size);

#endif
