#ifndef __ANALOG_I2C_CFG__H
#define __ANALOG_I2C_CFG__H

#include "stdlib.h"
#include "macro_defined.h"
#include "gpio_pin.h"
typedef enum{
    I2C_BUS_01 = 0,
    I2C_BUS_NUMBER_MAX
}E_I2CBUS_NUM;



void i2c_bus_creat(void);
#endif