#include "spi.h"
#include "debuglog.h"


/*
*  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
*  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
*  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;   
*/
extern SPI_HandleTypeDef hspi1;
unsigned char Spi_TxData[4]={0x83,0x84,0x85,0x00};
unsigned char Spi_pRxData[4]={0};

static uint32_t sg_mt6816data = 0;
void* mt6816_read(void)
{
    uint32_t AngleIn17bits = 0;
    HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_RESET);  
    HAL_SPI_TransmitReceive(&hspi1, &Spi_TxData[0], &Spi_pRxData[0],0x03,0xFF);
    HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_SET);

    AngleIn17bits =(((Spi_pRxData[1]&0x00ff)<<8) | (Spi_pRxData[2]&0x00fc))>>2;
    AngleIn17bits = 16384 - AngleIn17bits;
    sg_mt6816data = AngleIn17bits;
    return (void*)&sg_mt6816data;
}

void mt6816_init(void)
{
    for (uint8_t i = 0; i < 10; i++)
    {    
        /*mt6816 首次上电读取数据错误*/
        HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_RESET);  
        HAL_SPI_TransmitReceive(&hspi1, &Spi_TxData[0], &Spi_pRxData[0],0x03,0xffff);
        HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_SET);
    }
}


