#include "mt6816.h"
#include "pubilc.h"

typedef struct mt6816_data
{
    unsigned int raw; //原始数据
    float cov_data;   //转换后的数据    
    float filter_data;    //滤波后数据
}mt6816_data_t;


extern SPI_HandleTypeDef hspi1;
static unsigned char Spi_TxData[4]={0x83,0x84,0x05,0xff};
static unsigned char Spi_pRxData[4]={0};
/*
��Լ��Ҫ26us
ʹ��cubemx����ʱ:ע�⵱ϵͳʱ��Ƶ��170MHz,Baud Rate����Ӧ��10.635MBits/s��������
1������������ԭ�������?
*/
void* mt6816_read(void)
{
    static mt6816_data_t data;

    unsigned int AngleIn17bits = 0;
    HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_RESET);  
    HAL_SPI_TransmitReceive(&hspi1, &Spi_TxData[0], &Spi_pRxData[0],0x03,0xffff);
    HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_SET);
    AngleIn17bits =(((Spi_pRxData[1]&0x00ff)<<8) | (Spi_pRxData[2]&0x00fc))>>2;
    AngleIn17bits = 16384 - AngleIn17bits;
    data.raw = AngleIn17bits;
    data.cov_data = AngleIn17bits * 0.00038349f;

    return (void*)&data;
}