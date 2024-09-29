#include "bus_driver.h"

static bus_dv_t bus_arry[BUS_NUM];
static uint16_t count = 0;
void bus_resgiter(bus_dv_t *this)
{
    bus_arry[count].bus_num = this->bus_num;
    bus_arry[count].bus_init = this->bus_init;
}

void bus_init(void)
{
    for (uint16_t i = 0; i < count; i++)
    {
        bus_arry[i].bus_init();
    }
}


