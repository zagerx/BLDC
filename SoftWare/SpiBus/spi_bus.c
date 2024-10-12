#include "spi_bus.h"
#include "spi.h"
void spi_bus1_init(void)
{
    MX_SPI1_Init();
}
void spi_bus1_read(uint16_t DevAddress, uint8_t register_addr, uint8_t *pData, uint16_t Size)
{
}
void spi_bus1_write(uint16_t DevAddress, uint8_t register_addr,uint8_t *pData, uint16_t Size)
{
}

void spi_bus1_rw(uint8_t *pw,uint8_t *pr,uint16_t datalen)
{
    spi_wr_data(pw,pr,datalen);
}
