#include "spi_device.h"
#include "spi_bus.h"

void spi_device_init(spi_device_t *dev)
{
    dev->init(dev->bus);
}
