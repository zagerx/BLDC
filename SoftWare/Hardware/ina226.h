#ifndef __INA226__H
#define __INA226__H

#if 1
    #include "i2c_bus.h"
    void dev_ina226_init(i2c_bus_t *bus);
#else
    void ina226_init(void);
    void* ina226_read(void);
#endif
#endif
