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

#define IAN226_CFG_VAL                   (0x4727)
#define IAN226_CALIB_VAL                 (0x0A00)

#define IAN226_SHUNTVOLITE_LSB           (0.0025f)//mV/bit
#define IAN226_BUSVOLITE_LSB             (0.00125f)
#define IAN226_CURRENT_LSB               (0.001f)
#define IAN226_POWER_LSB                 (0.025f)


typedef struct _data
{
    int32_t *raw_buf;     //原始数据
    int32_t *covdata_buf;       //转换后的数据    
    int32_t *filterdata_buf;    //滤波后数据
    int16_t buf_column;
}ina622_data_t;

static ina622_data_t sg_data;
static unsigned int rawdata_buf[3];
static float covdata_buf[3];
static float filterdata_buf[3];
static uint8_t Rx_buf[2] = {0};
static void ina226_read(uint16_t MemAddress, uint16_t *pData)
{
    static uint8_t buf[2] = {0};
    i2c2_read(IAN226_BASE_ADDR, MemAddress, Rx_buf, sizeof(Rx_buf));
    HAL_Delay(10);//临时，等待中断更新buf
    *pData = (uint16_t)buf[0]<<8 | buf[1];
}
static void ina226_write( uint8_t MemAddress, uint16_t *cmd)
{
    static uint8_t buf[2] = {0};
    buf[0] = (uint8_t)(*cmd>>8);
    buf[1] = (uint8_t)(*cmd);
    i2c2_write(IAN226_BASE_ADDR, MemAddress, buf, sizeof(buf));
}
void ina226_init(void)
{
    /*读取ID号*/
    uint16_t id,cfg_val;
    uint16_t cmd_cfg = IAN226_CFG_VAL;
    uint16_t cmd_calib = IAN226_CALIB_VAL;
    // ina226_read(IAN226_DIEIDREGISTER_ADDR,&id);
    // USER_DEBUG_NORMAL("id 0x%x\r\n",id);
    // ina226_read(IAN226_CFGREGISTER_ADDR,&cfg_val);
    // USER_DEBUG_NORMAL("cfg_val 0x%x\r\n",cfg_val);
    // ina226_write(IAN226_CFGREGISTER_ADDR,&cmd_cfg);
    // ina226_read(IAN226_CFGREGISTER_ADDR,&cfg_val);
    // USER_DEBUG_NORMAL("id 0x%x\r\n",cfg_val);    
    // HAL_Delay(100);
    cmd_cfg = 0x4537;
    ina226_write(IAN226_CFGREGISTER_ADDR,&cmd_cfg);
    ina226_read(IAN226_CFGREGISTER_ADDR,&cmd_cfg);
    ina226_read(IAN226_CFGREGISTER_ADDR,&cmd_cfg);
    USER_DEBUG_NORMAL("id 0x%x\r\n",cfg_val); 
    // i2c2_init();
    return;


    // sg_data.raw_bufdata = rawdata_buf;
    // sg_data.p_covdata = covdata_buf;
    // sg_data.p_filterdata = filterdata_buf;
    // sg_data.buf_column = 3;
}

typedef struct ian226
{
    float bus_volite;
    float shunt_volite;
    float currment;
    float power;
}st_ian226_t;

st_ian226_t IAN226_Vale = {0};

void* ina226_read_data(void)
{
    return (void *)(&sg_data);
    uint16_t temp = 0;
    ina226_read(IAN226_SHUNTVOLTAGEREGISTER_ADDR,&temp);
    USER_DEBUG_NORMAL("shunt voltage:%d  ",temp);
    IAN226_Vale.shunt_volite = temp*IAN226_SHUNTVOLITE_LSB;

    ina226_read(IAN226_VBUSREGISTER_ADDR,&temp);
    USER_DEBUG_NORMAL("Bus voltage:%d  ",temp);
    IAN226_Vale.bus_volite = temp*IAN226_BUSVOLITE_LSB;

    ina226_read(IAN226_CURRENTREGISTER_ADDR,&temp);
    USER_DEBUG_NORMAL("current:%d  ",temp);
    IAN226_Vale.currment = temp*IAN226_CURRENT_LSB;

    ina226_read(IAN226_POWERREGISTER_ADDR,&temp);
    USER_DEBUG_NORMAL("Power:%d  \r\n",temp);
    IAN226_Vale.power = temp*IAN226_POWER_LSB;
    return (void *)(&sg_data);
}

