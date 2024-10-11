#include "at24c0x.h"
#include "debuglog.h"
#define AT24C02_BASE_ADDR 0xA0
void at24cxx_init(i2c_bus_t *bus)
{
    uint8_t buf[16] = {0};
    bus->read(AT24C02_BASE_ADDR, 0x80, buf, sizeof(buf));
    for (unsigned char i = 0; i < 16; i++)
    {
        USER_DEBUG_NORMAL("0x%x  ",buf[i]); //测试NG
        /* code */
    }
       USER_DEBUG_NORMAL("\n");
}
