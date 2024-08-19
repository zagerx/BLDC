#include "tle5012b.h"

#define TLE5012_READ_ANGLE_CMD           (0x80)
#define TLE5012_SEND_CMD_BYTES           (2)
#define TLE5012_READ_DATA_BYTES          (4)
#define TLE5012_TRANS_TIMEOUT            (2)
#define SPI_REG_ADRRESS_ANGLE 	         (2)

typedef struct _data
{
    int32_t *raw_buf;     //原始数据
    int32_t *covdata_buf;       //转换后的数据    
    int32_t *filterdata_buf;    //滤波后数据
    int16_t buf_column;
}tle5012b_data_t;

static tle5012b_data_t sg_tle5012data = {0};
 int32_t rawdata,covdata,filterdata;
static int16_t column;


static inline uint8_t SpiCheckCrc8SR(uint8_t* uc_data,uint8_t uc_len)
{
    uint8_t uc_crc_polynomial = 0x1d; //x^8 + x^4 + x^3 + x^2 + 1
    uint8_t uc_i,uc_j; 
    uint8_t u1Crc = 0xff;
    for(uc_j = 0;uc_j < uc_len; uc_j ++){
        u1Crc ^= *(uc_data + uc_j);
        for(uc_i = 0;uc_i < 8; uc_i ++){
            if(u1Crc & 0x80){
                u1Crc = (uint8_t)((u1Crc << 1) ^ uc_crc_polynomial);
            }else{
                u1Crc = (uint8_t)(u1Crc << 1);
            }
        }
    }

    return (~u1Crc);
}
// transmit and then receive and then check crc
static inline uint8_t SpiExchangeHalSR(SPI_HandleTypeDef *hSpi, uint16_t *pData)
{
	uint8_t u1Ret = 0u;
    uint8_t u1SpiBuf[TLE5012_SEND_CMD_BYTES + TLE5012_READ_DATA_BYTES] = {0};

    // variable init
	u1SpiBuf[0] = TLE5012_READ_ANGLE_CMD;
	u1SpiBuf[1] = (SPI_REG_ADRRESS_ANGLE << 4) + 1;

	u1Ret = 1;
    if (HAL_SPI_Transmit(hSpi, u1SpiBuf, TLE5012_SEND_CMD_BYTES, 0XFFFF) == 0u) {
        if (HAL_SPI_Receive(hSpi, &u1SpiBuf[TLE5012_SEND_CMD_BYTES], TLE5012_READ_DATA_BYTES, 0XFFFF) == 0u) {
            // get crc8
			uint8_t u1Crc = SpiCheckCrc8SR(u1SpiBuf, TLE5012_SEND_CMD_BYTES + TLE5012_READ_DATA_BYTES - 2);
			// check crc
			if(u1Crc == u1SpiBuf[TLE5012_SEND_CMD_BYTES + TLE5012_READ_DATA_BYTES - 1]){
				*pData = ((uint16_t)u1SpiBuf[TLE5012_SEND_CMD_BYTES] << 8) + u1SpiBuf[TLE5012_SEND_CMD_BYTES + 1];
				u1Ret = 0;
			}
        }
    }
    return u1Ret;
}
void tle5012b_init(void)
{

}
void* tle5012b_read(void)
{
    unsigned short pData;
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port,SPI3_CS_Pin,GPIO_PIN_RESET);
    for(short i = 50; i > 0 ; i--);
	SpiExchangeHalSR(&hspi3, &pData);
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port,SPI3_CS_Pin,GPIO_PIN_SET);

    unsigned int AngleIn17bits = 0;
    AngleIn17bits = (unsigned int)pData - 32768;

    rawdata = AngleIn17bits;
    covdata = (AngleIn17bits * 6.2831853f/32768.0f) * (1<<20);
    return (void*)&sg_tle5012data;
}
