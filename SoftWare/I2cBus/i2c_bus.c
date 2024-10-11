#include "i2c_bus.h"
#include "i2c.h"
void i2c_bus1_init(void)
{
    MX_I2C2_Init();
}
void i2c_bus1_read(uint16_t DevAddress, uint8_t register_addr, uint8_t *pData, uint16_t Size)
{
    i2c2_read(DevAddress,register_addr,pData,Size);
}
void i2c_bus1_write(uint16_t DevAddress, uint8_t register_addr,uint8_t *pData, uint16_t Size)
{
    i2c2_write(DevAddress,register_addr,pData,Size);
}


