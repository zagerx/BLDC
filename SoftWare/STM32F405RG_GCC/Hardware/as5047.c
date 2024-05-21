#include "as5047.h"
#include "spi.h"
#include "gpio.h"

static uint16_t SPI_ReadWrite_OneByte(uint16_t _txdata);
static uint8_t ams_parity(uint16_t v);

void as5047_init(void)
{

}
uint16_t as5047_readangle(void)
{
	uint16_t data;
    data=SPI_ReadWrite_OneByte(0xFFFF); 
	return data;
}

static uint8_t ams_parity(uint16_t v)
{
	v ^= v >> 8;
	v ^= v >> 4;
	v ^= v >> 2;
	v ^= v >> 1;
	return v & 1;
}
static uint16_t SPI_ReadWrite_OneByte(uint16_t _txdata)
{

    uint16_t pos,rawVal;
	HAL_GPIO_WritePin(SPI3_SOFTCS_GPIO_Port,SPI3_SOFTCS_Pin,GPIO_PIN_RESET);	

    HAL_SPI_TransmitReceive(&hspi3,(uint8_t *)&_txdata,(uint8_t *)&rawVal,1,1000);
	HAL_GPIO_WritePin(SPI3_SOFTCS_GPIO_Port,SPI3_SOFTCS_Pin,GPIO_PIN_SET);

	if(ams_parity(rawVal)) 
    {
        return 0xFFFF;
    }else{
        return pos = (rawVal & 0x3fff);
    }
}



