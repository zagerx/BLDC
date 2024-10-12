#ifndef __SPI_BUS__H
#define __SPI_BUS__H
#include "stdint.h"

typedef struct spi_bus
{
    void (*init)(void);
    void (*read)(uint16_t,uint8_t,uint8_t*,uint16_t);
    void (*write)(uint16_t,uint8_t,uint8_t*,uint16_t);
    void (*rw)(uint16_t,uint8_t,uint8_t*,uint16_t);

}spi_bus_t;

void spi_bus1_init(void);
void spi_bus1_read(uint16_t DevAddress, uint8_t register_addr,uint8_t *pData, uint16_t Size);
void spi_bus1_write(uint16_t DevAddress, uint8_t register_addr,uint8_t *pData, uint16_t Size);
void spi_bus1_rw(uint8_t *pw,uint8_t *pr,uint16_t datalen);

#endif