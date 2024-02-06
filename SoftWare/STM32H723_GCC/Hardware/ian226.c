/*
    INA226:电流/功率监视器
*/

#include "_common.h"

#define IAN226_BASE_ADDR                (0x40)
#define IAN226_DEVICE_READADDR          (IAN226_BASE_ADDR)<<1
#define IAN226_DEVICE_WRITEADDR         ((IAN226_BASE_ADDR)<<1)|0X01
#define IAN226_MANUFIDREGISTER_ADDR     (0xFE00)
#define IAN226_DIEIDREGISTER_ADDR       (0xFF00)


#include "i2c.h"
static void ina226_read(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    i2c2_read(DevAddress, MemAddress, 1, pData, Size);
}
void* ina226_read_data(void)
{
    static uint8_t id_buf[2] = {0xFF,0xFF};
    ina226_read(IAN226_DEVICE_READADDR,IAN226_MANUFIDREGISTER_ADDR,id_buf,sizeof(id_buf));
    return (void *)id_buf;
}

