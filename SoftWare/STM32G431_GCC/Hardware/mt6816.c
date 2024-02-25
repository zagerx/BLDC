#include "spi.h"
extern SPI_HandleTypeDef hspi1;
static unsigned char Spi_TxData[4]={0x83,0x84,0x05,0xff};
static unsigned char Spi_pRxData[4]={0};


typedef struct mt6816_data
{
    int32_t *raw_buf;     //原始数据
    int32_t *covdata_buf;       //转换后的数据    
    int32_t *filterdata_buf;    //滤波后数据
    int16_t buf_column;
}mt6816_data_t;

static mt6816_data_t sg_mt6816data = {0};
 int32_t rawdata,covdata,filterdata;
static int16_t column;


void* mt6816_read(void)
{
    unsigned int AngleIn17bits = 0;
    HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_RESET);  
    HAL_SPI_TransmitReceive(&hspi1, &Spi_TxData[0], &Spi_pRxData[0],0x03,0xffff);
    HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_SET);
  
    AngleIn17bits =(((Spi_pRxData[1]&0x00ff)<<8) | (Spi_pRxData[2]&0x00fc))>>2;
    AngleIn17bits = 16384 - AngleIn17bits;
    rawdata = AngleIn17bits;
    covdata = AngleIn17bits * 12;

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

    sg_mt6816data.raw_buf = &rawdata;
    sg_mt6816data.covdata_buf = &covdata;
    sg_mt6816data.filterdata_buf = &filterdata;
}