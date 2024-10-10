/*
    INA226:电流/功率监视器
*/
#include "i2c.h"
#include "debuglog.h"
#include "i2c_device.h"
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


static void _ina226read(uint16_t MemAddress, uint16_t *pData);
static void _ina226write( uint8_t MemAddress, uint16_t *cmd);

void ina226_init(void)
{
    uint16_t id,cfg_val;
    uint16_t cmd_cfg = IAN226_CFG_VAL;
    uint16_t cmd_calib = IAN226_CALIB_VAL;
    _ina226write(IAN226_CFGREGISTER_ADDR,&cmd_cfg);
    _ina226write(IAN226_CALIBREGISTER_ADDR,&cmd_calib);

    /*读取ID号*/
    _ina226read(IAN226_DIEIDREGISTER_ADDR,&id);
    _ina226read(IAN226_DIEIDREGISTER_ADDR,&id);
    USER_DEBUG_NORMAL("id 0x%x\r\n",id);
    _ina226read(IAN226_CFGREGISTER_ADDR,&cfg_val);
    _ina226read(IAN226_CFGREGISTER_ADDR,&cmd_cfg);
    USER_DEBUG_NORMAL("cmd_cfg 0x%x\r\n",cmd_cfg);
}

void* ina226_read(void)
{
    uint16_t temp = 0;
    enum{
        EN_READ_VBUS = 0,
        EN_READ_SHUNT,
        EN_READ_CURRENT,
        EN_READ_POWER,
        EN_READ_ID
    };
    static uint8_t stats = 0;
    switch (stats)
    {
    case EN_READ_VBUS:
        _ina226read(IAN226_VBUSREGISTER_ADDR,&temp);
        USER_DEBUG_NORMAL("Bus voltage:%d  %f\n",temp,temp*IAN226_BUSVOLITE_LSB);
        stats = EN_READ_SHUNT;
        break;

    case EN_READ_SHUNT:
        _ina226read(IAN226_SHUNTVOLTAGEREGISTER_ADDR,&temp);
        USER_DEBUG_NORMAL("shunt voltage:%d  %f\n",temp,temp*IAN226_SHUNTVOLITE_LSB);
        stats = EN_READ_CURRENT;
        break;

    case EN_READ_CURRENT:
        _ina226read(IAN226_CURRENTREGISTER_ADDR,&temp);
        USER_DEBUG_NORMAL("shunt currment:%d  %f\n",temp,temp*IAN226_CURRENT_LSB);
        stats = EN_READ_POWER;
        break;

    case EN_READ_POWER:
        _ina226read(IAN226_POWERREGISTER_ADDR,&temp);
        USER_DEBUG_NORMAL("Power:%d  %f\n",temp,temp*IAN226_POWER_LSB);
        stats = EN_READ_VBUS;
        break;
    default:
        break;
    }
    return (void *)(0);  
}

static void _ina226read(uint16_t MemAddress, uint16_t *pData)
{
    uint8_t buf[2] = {0};
    i2c2_read(IAN226_BASE_ADDR, MemAddress, buf, sizeof(buf));
    *pData = (uint16_t)buf[0]<<8 | buf[1];
}
static void _ina226write( uint8_t MemAddress, uint16_t *cmd)
{
    static uint8_t buf[2] = {0};
    buf[0] = (uint8_t)(*cmd>>8);
    buf[1] = (uint8_t)(*cmd);
    i2c2_write(IAN226_BASE_ADDR, MemAddress, buf, sizeof(buf));
}