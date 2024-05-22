#include "as5047.h"
#include "spi.h"
#include "gpio.h"

typedef struct as5047_data
{
    int32_t *raw_buf;
    int32_t *covdata_buf;
    int32_t *filterdata_buf;
    int16_t buf_column;
}as5047_data_t;

static as5047_data_t sg_as5047data = {0};
int32_t rawdata,covdata,filterdata;
static int16_t column;


static uint16_t SPI_ReadWrite_OneByte(uint16_t _txdata);
static uint8_t ams_parity(uint16_t v);

void as5047_init(void)
{
    sg_as5047data.raw_buf = &rawdata;
    sg_as5047data.covdata_buf = &covdata;
    sg_as5047data.filterdata_buf = &filterdata;
}
void* as5047_readangle(void)
{
	uint16_t data;
    data=SPI_ReadWrite_OneByte(0xFFFF); 
	rawdata = data;
	return (void*)&sg_as5047data;
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



