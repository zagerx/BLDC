/*
    INA226:电流/功率监视器
*/
#include "i2c.h"
#include "_common.h"

#define IAN226_BASE_ADDR                 (0x80)
#define IAN226_DEVICE_READADDR           (IAN226_BASE_ADDR)
#define IAN226_DEVICE_WRITEADDR          ((IAN226_BASE_ADDR))|0X01
#define IAN226_MANUFIDREGISTER_ADDR      (0xFE)
#define IAN226_DIEIDREGISTER_ADDR        (0xFF)

#define IAN226_CFGREGISTER_ADDR          (0x00)
#define IAN226_CALIBREGISTER_ADDR        (0x05)
#define IAN226_SHUNTVOLTAGEREGISTER_ADDR (0x01)
#define IAN226_VBUSREGISTER_ADDR         (0x02)
#define IAN226_POWERREGISTER_ADDR        (0x03)
#define IAN226_CURRENTREGISTER_ADDR      (0x04)
typedef struct _data
{
    unsigned int *raw_bufdata;
    unsigned int *p_covdata;
    unsigned int *p_filterdata;
    unsigned short buf_column;
}ina622_data_t;

static ina622_data_t sg_data;
static unsigned int rawdata_buf[3];
static float covdata_buf[3];
static float filterdata_buf[3];
static void ina226_read(uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    i2c2_read(IAN226_BASE_ADDR, MemAddress, pData, Size);
}
static void ina226_write( uint8_t MemAddress, uint8_t *pData, uint16_t Size)
{
    i2c2_write(IAN226_BASE_ADDR, MemAddress, pData, Size);
}
void ina226_init(void)
{

    /*读取ID号*/
    uint8_t buf[2] = {0x00,0x00};
    ina226_read(IAN226_DIEIDREGISTER_ADDR,buf,sizeof(buf));
    USER_DEBUG_NORMAL("id 0x%x\r\n",(buf[0]<<8) | buf[1]);

    uint8_t cmdbuf[2] = {0x45,0x27};//16次平均 1.1ms 连续测量分流电压和总线电压
    ina226_write(IAN226_CFGREGISTER_ADDR,cmdbuf,2);

    ina226_read(IAN226_CFGREGISTER_ADDR,buf,sizeof(buf));
    USER_DEBUG_NORMAL("id 0x%x\r\n",(buf[0]<<8) | buf[1]);    

	//写校准寄存器
	//LSB选择0.02mA,分压电阻选0.0001R Cal=0.00512/(0.02mA*0.0001R)=2560  cmdbuf[0] = 0x0A;cmdbuf[1] = 0x00;
    //LSB选择0.02mA,分压电阻选0.002R(2mR) Cal=0.00512/(0.02mA*0.002R)=128  cmdbuf[0] = 0x00;cmdbuf[1] = 0x08;
    cmdbuf[0] = 0x00;cmdbuf[1] = 0x08;
    ina226_write(IAN226_CFGREGISTER_ADDR,cmdbuf,2);

    sg_data.raw_bufdata = rawdata_buf;
    sg_data.p_covdata = covdata_buf;
    sg_data.p_filterdata = filterdata_buf;
    sg_data.buf_column = 3;
}




void* ina226_read_data(void)
{
    uint8_t temp_buf[2] = {0x55,0x55};
    ina226_read(IAN226_SHUNTVOLTAGEREGISTER_ADDR,temp_buf,sizeof(temp_buf));
    USER_DEBUG_NORMAL("shunt voltage %d\r\n",(temp_buf[0]<<8) | temp_buf[1]);

    ina226_read(IAN226_VBUSREGISTER_ADDR,temp_buf,sizeof(temp_buf));
    USER_DEBUG_NORMAL("vbus  %d\r\n",(temp_buf[0]<<8) | temp_buf[1]);
    ina226_read(IAN226_POWERREGISTER_ADDR,temp_buf,sizeof(temp_buf));
    USER_DEBUG_NORMAL("power  %d\r\n",(temp_buf[0]<<8) | temp_buf[1]);
    ina226_read(IAN226_CURRENTREGISTER_ADDR,temp_buf,sizeof(temp_buf));
    USER_DEBUG_NORMAL("current  %d\r\n",(temp_buf[0]<<8) | temp_buf[1]);    
    return (void *)(&sg_data);
}

