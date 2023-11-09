#ifndef __MT6816__H
#define __MT6816__H
#include "stdint.h"
#include "stdbool.h"
typedef struct mt6816
{
    /* data */
    uint8_t no_mag_flag;
    float angle;
    uint32_t pc_flag;
    uint32_t sample_data;
}mt6816_t;
void MT6816_SPI_Get_AngleData(void);
float mt6816_readangle(void);
#endif
