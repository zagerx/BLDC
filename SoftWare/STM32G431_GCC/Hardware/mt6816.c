#include "mt6816.h"
#include "spi.h"
extern SPI_HandleTypeDef hspi1;
#if 1

static unsigned char Spi_TxData[4]={0x83,0x84,0x05,0xff};///////03 04 05 寄存器存角度
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
    uint16_t data_t[2];  //SPI��������
    uint16_t data_r[2];  //SPI�������ݣ����Ƕ�����
    uint8_t h_count;  //��żУ�������

    data_t[0] = 0x8300;  //����MT6816 Datasheet, 0x8300��ȡ0x03�Ĵ�������
    data_t[1] = 0x8400;  //ͬ��

    for(uint8_t i=0; i<3; i++)
    {
        MT6816_SPI_CS_L();
        HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&data_t[0], (uint8_t *)&data_r[0], 1, 0xFFFF);
        MT6816_SPI_CS_H();
        MT6816_SPI_CS_L();
        HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&data_t[1], (uint8_t *)&data_r[1], 1, 0xFFFF);
        MT6816_SPI_CS_H();

        mt6816_spi.sample_data = ((data_r[0] & 0xFF) << 8 | (data_r[1]) & 0xFF);  //16bits SPI��������

        //����Ϊ��żУ��
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
            break;  //��żУ�����
        }

    }
    if(mt6816_spi.pc_flag)
    {
        mt6816_spi.angle = mt6816_spi.sample_data >> 2;  //ǰ14bits �Ƕ�����
        aaa__xx = mt6816_spi.angle;
        mt6816_spi.no_mag_flag = (bool) (mt6816_spi.sample_data & 0x0010);  //���ű�־λ������datasheet
    }
}
#endif

