#ifndef __BUS_DRIVER__H
#define __BUS_DRIVER__H
#include "stdint.h"
typedef struct bus_driver
{
    uint16_t bus_numb;
    void (*bus_init)(void);
    void (*bus_close)(void);
}bus_dv_t;

#define BUS_NUM  2

void bus_resgiter(bus_dv_t *this);
void bus_init(void);

#endif
