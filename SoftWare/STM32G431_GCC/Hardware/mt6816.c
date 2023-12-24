#include "mt6816.h"
extern SPI_HandleTypeDef hspi1;
static unsigned char Spi_TxData[4]={0x83,0x84,0x05,0xff};
static unsigned char Spi_pRxData[4]={0};
/*
大约需要26us
使用cubemx配置时:注意当系统时钟频率170MHz,Baud Rate速率应在10.635MBits/s！！！！
1、引起该现象的原因需查找
*/
float mt6816_readangle(void)
{
    float theta;
    unsigned int AngleIn17bits = 0;
    HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_RESET);///CSN LOW   
    HAL_SPI_TransmitReceive(&hspi1, &Spi_TxData[0], &Spi_pRxData[0],0x03,0xffff);
    HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_SET);///CSN HIGH
    AngleIn17bits =(((Spi_pRxData[1]&0x00ff)<<8)|(Spi_pRxData[2]&0x00fc))>>2;
    theta = AngleIn17bits * 0.00038349f;
    return (theta);	
}
