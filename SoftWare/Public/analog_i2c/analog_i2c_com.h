#ifndef __ANALOG_I2C_COM__H
#define __ANALOG_I2C_COM__H
#include "analog_i2c.h"
#include "analog_i2c_cfg.h"

extern i2c_bus_info_t i2c_bus_infoarry[I2C_BUS_NUMBER_MAX];
extern i2c_bus_t* i2c_bus_arry[I2C_BUS_NUMBER_MAX];

int i2c_bitops_bus_xfer_read(i2c_bus_t *i2c_bus,i2c_dev_info_t *info);
int i2c_bitops_bus_xfer_write(i2c_bus_t *i2c_bus,i2c_dev_info_t *info);

#endif