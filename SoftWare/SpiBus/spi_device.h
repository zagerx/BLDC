#ifndef __SPI_DEVICE__H
#define __SPI_DEVICE__H

#include "stdint.h"
#include "spi_bus.h"
typedef struct spi_device
{
    void (*read)(void *,uint8_t);
    void (*write)(uint8_t *,uint8_t); 
    void (*init)(spi_bus_t*);
    spi_bus_t *bus;
}spi_device_t;

void spi_device_init(spi_device_t *dev);
#endif
