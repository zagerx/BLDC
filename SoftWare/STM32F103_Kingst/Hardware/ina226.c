/*
    INA226:电流/功率监视器
*/
// #include "i2c.h"
#include "_common.h"

#define IAN226_BASE_ADDR                (0x40)
#define IAN226_DEVICE_READADDR          (IAN226_BASE_ADDR)<<1
#define IAN226_DEVICE_WRITEADDR         ((IAN226_BASE_ADDR)<<1)|0X01
#define IAN226_MANUFIDREGISTER_ADDR     (0xFE00)
#define IAN226_DIEIDREGISTER_ADDR       (0xFF00)

typedef struct _data
{
    unsigned int *raw_buf;
    unsigned int *covdata_buf;
    unsigned int *filterdata_buf;
    unsigned short buf_column;
}ina622_data_t;

static ina622_data_t sg_data;
static signed int rawdata_buf[3] = {1,2,33};
static signed int covdata_buf[3] = {4,5,-23};
static signed int filterdata_buf[3] = {7,8,9};
static void ina226_read(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    // i2c2_read(DevAddress, MemAddress, 1, pData, Size);
}

void ina226_init(void)
{
    sg_data.raw_buf = rawdata_buf;
    sg_data.covdata_buf = covdata_buf;
    sg_data.filterdata_buf = filterdata_buf;
    sg_data.buf_column = 3;
}
void* ina226_read(void)
{
    static uint8_t id_buf[2] = {0xFF,0xFF};
    // ina226_read(IAN226_DEVICE_READADDR,IAN226_MANUFIDREGISTER_ADDR,id_buf,sizeof(id_buf));
    return (void *)(&sg_data);
}

