#include "as5047.h"
#include "spi.h"
/*
*  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
*  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
*  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;   
*/
static uint32_t g_rawdata;
static uint16_t as5047_read_data(uint16_t _txdata);
static uint8_t ams_parity(uint16_t v);

void as5047_init(void)
{
    g_rawdata = 0;
}
void* as5047_readangle(void)
{
	uint16_t data;
    data = as5047_read_data(0xFFFF); 
	g_rawdata = data;
	return (void*)(&g_rawdata);
}

static uint8_t ams_parity(uint16_t v)
{
	v ^= v >> 8;
	v ^= v >> 4;
	v ^= v >> 2;
	v ^= v >> 1;
	return v & 1;
}
static uint16_t as5047_read_data(uint16_t _txdata)
{
    uint16_t pos,rawVal;
    spi_wr_data((uint8_t *)&_txdata,(uint8_t *)&rawVal,1);
	if(ams_parity(rawVal)) 
    {
        return 0xFFFF;
    }else{
        return pos = (rawVal & 0x3fff);
    }
}

uint32_t as5047_get_data(void)
{
    uint32_t data = as5047_read_data(0xFFFF);
    return data;
}


