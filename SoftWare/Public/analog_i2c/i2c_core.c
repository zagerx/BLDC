#include "analog_i2c.h"
#include "analog_i2c_com.h"

int i2c_bus_xfer_read(i2c_dev_t *i2c_dev)
{
    i2c_dev_info_t *msgs;
    msgs = i2c_dev->dev_msg;
	return i2c_bitops_bus_xfer_read((i2c_bus_t*)(i2c_dev->i2c_phy),msgs);
}
int i2c_bus_xfer_write(i2c_dev_t *i2c_dev)
{
    i2c_dev_info_t *msgs;
    msgs = i2c_dev->dev_msg;    
	return i2c_bitops_bus_xfer_write((i2c_bus_t*)(i2c_dev->i2c_phy),msgs);
}
