#include "mt6816.h"
#include "spi.h"
extern SPI_HandleTypeDef hspi1;
#if 1

static unsigned char Spi_TxData[4]={0x83,0x84,0x05,0xff};///////03 04 05 瀵勫瓨鍣ㄥ瓨瑙掑害
static unsigned char Spi_pRxData[4]={0};
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
#else
mt6816_t mt6816_spi;
float aaa__xx;
#define  MT6816_SPI_CS_L() HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_RESET)///CSN LOW   
#define  MT6816_SPI_CS_H() HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_SET)
void MT6816_SPI_Get_AngleData(void)
{
    uint16_t data_t[2];  //SPI发送数据
    uint16_t data_r[2];  //SPI接收数据，即角度数据
    uint8_t h_count;  //奇偶校验计数用

    data_t[0] = 0x8300;  //根据MT6816 Datasheet, 0x8300读取0x03寄存器数据
    data_t[1] = 0x8400;  //同上

    for(uint8_t i=0; i<3; i++)
    {
        MT6816_SPI_CS_L();
        HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&data_t[0], (uint8_t *)&data_r[0], 1, 0xFFFF);
        MT6816_SPI_CS_H();
        MT6816_SPI_CS_L();
        HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&data_t[1], (uint8_t *)&data_r[1], 1, 0xFFFF);
        MT6816_SPI_CS_H();

        mt6816_spi.sample_data = ((data_r[0] & 0xFF) << 8 | (data_r[1]) & 0xFF);  //16bits SPI接收数据

        //以下为奇偶校验
        h_count = 0;
        for(uint8_t j=0; j<16; j++)
        {
            if(mt6816_spi.sample_data & (0x0001 << j))
                h_count++;
        }
        if(h_count & 0x01)
        {
            mt6816_spi.pc_flag = false;
        }
        else{
            mt6816_spi.pc_flag = true;
            break;  //奇偶校验结束
        }

    }
    if(mt6816_spi.pc_flag)
    {
        mt6816_spi.angle = mt6816_spi.sample_data >> 2;  //前14bits 角度数据
        aaa__xx = mt6816_spi.angle;
        mt6816_spi.no_mag_flag = (bool) (mt6816_spi.sample_data & 0x0010);  //弱磁标志位，根据datasheet
    }
}
#endif

