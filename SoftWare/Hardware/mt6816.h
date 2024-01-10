#ifndef __MT6816__H
#define __MT6816__H
#include "./hardware_cfg.h"
typedef struct mt6816
{
    /* data */
    uint8_t no_mag_flag;
    float angle;
    uint32_t pc_flag;
    uint32_t sample_data;
}mt6816_t;


typedef struct mt6816_data
{
    unsigned int raw; //原始数据
    float cov_data;   //转换后的数据    
    float filter_data;    //滤波后数据
}mt6816_data_t;


float mt6816_readangle(void);
void* mt6816_read(void);
#endif
