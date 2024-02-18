#include "spi.h"

typedef struct mt6816_data
{
    unsigned int *raw_buf;
    unsigned int *covdata_buf;
    unsigned int *filterdata_buf;
    unsigned short buf_column;
}mt6816_data_t;


extern SPI_HandleTypeDef hspi1;
static unsigned char Spi_TxData[4]={0x83,0x84,0x05,0xff};
static unsigned char Spi_pRxData[4]={0};

void* mt6816_read(void)
{
    static mt6816_data_t data;

    static unsigned int AngleIn17bits = 0;
    // static signed int AngleIn17bits_Cov = 0;
    HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_RESET);  
    HAL_SPI_TransmitReceive(&hspi1, &Spi_TxData[0], &Spi_pRxData[0],0x03,0xffff);
    HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_SET);
    AngleIn17bits =(((Spi_pRxData[1]&0x00ff)<<8) | (Spi_pRxData[2]&0x00fc))>>2;
    AngleIn17bits = 16384 - AngleIn17bits;
    // data.raw_buf = &AngleIn17bits; 
    // AngleIn17bits_Cov = AngleIn17bits * 12;
    // data.covdata_buf = &AngleIn17bits_Cov;
    static float theta;
    theta = AngleIn17bits * 0.0003834f;
    return (void*)&theta;
}

