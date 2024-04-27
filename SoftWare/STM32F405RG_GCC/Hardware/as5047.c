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


//计算奇偶函数
uint16_t Parity_bit_Calculate(uint16_t data_2_cal)
{
	uint16_t parity_bit_value=0;
	while(data_2_cal != 0)
	{
		parity_bit_value ^= data_2_cal; 
		data_2_cal >>=1;
	}
	return (parity_bit_value & 0x1); 
}
//SPI发送读取函数
uint16_t SPI_ReadWrite_OneByte(uint16_t _txdata)
{

	HAL_GPIO_WritePin(SPI3_SOFTCS_GPIO_Port,SPI3_SOFTCS_Pin,GPIO_PIN_RESET);	//cs拉低
	uint16_t rxdata;
	if(HAL_SPI_TransmitReceive(&hspi3,(uint8_t *)&_txdata,(uint8_t *)&rxdata,1,1000) !=HAL_OK)
    {
        rxdata=0;
    }
	HAL_GPIO_WritePin(SPI3_SOFTCS_GPIO_Port,SPI3_SOFTCS_Pin,GPIO_PIN_SET);
	return rxdata;
}
void* as5047_read(void)
{
    uint16_t add = 0;
	uint16_t data;
    add = ANGLEUNC;
	add |= 0x4000;	//读指令 bit14 置1
	if(Parity_bit_Calculate(add)==1) 
    {
        add=add|0x8000; //如果前15位 1的个数位偶数，则Bit15 置1
    }
	SPI_ReadWrite_OneByte(add);		//发送一条指令，不管读回的数据
    data=SPI_ReadWrite_OneByte(0x4000); //发送一条空指令，读取上一次指令返回的数据。
	data &=0x3fff;
    rawdata = data;
    covdata = data * 402;
	return (void *)&sg_as5047data;
}

void as5047_init(void)
{
    // for (uint8_t i = 0; i < 10; i++)
    // {    
    //     /*mt6816 首次上电读取数据错误*/
    //     HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_RESET);  
    //     HAL_SPI_TransmitReceive(&hspi1, &Spi_TxData[0], &Spi_pRxData[0],0x03,0xffff);
    //     HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_SET);
    // }
    sg_as5047data.raw_buf = &rawdata;
    sg_as5047data.covdata_buf = &covdata;
    sg_as5047data.filterdata_buf = &filterdata;

}
