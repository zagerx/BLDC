#ifndef __SPI_DEV__H
#define __SPI_DEV__H

typedef struct spi_dev
{
    uint16_t bus_num;
    void (*pread)(void);
    void (*pwrite)(void);
    void (*prw)(uint16_t *,uint16_t *,uint16_t);
}spi_dev_t;



#endif
