/****************************************************************************
Filename: datatypecovert.c
Functions list:
Notes:
Copyright (c) 2022, Syrius Robotics
All rights reserved.
****************************************************************************/
#include "stdint.h"
#include "datatypecovert.h"
#include "debuglog.h"
void convert_floats(unsigned char *pdata, unsigned short datalen, float *floats) 
{  
    if (datalen % 4 != 0) {  
        // 如果datalen不是4的倍数，则不能完整地包含float数  
        USER_DEBUG_NORMAL("Error: Data length must be a multiple of 4.\n");  
        return;  
    }
    unsigned int num_floats = datalen / 4;  
    for (unsigned int i = 0; i < num_floats; i++) {  
        float_int_union u;  
        // 从pdata中读取4个字节（即一个float）  
        u.i = (uint32_t)pdata[i*4]  | (uint32_t)pdata[i*4+1] << 8 |  
              (uint32_t)pdata[i*4+2] << 16 | (uint32_t)pdata[i*4+3]<< 24;          
        floats[i] = u.f; // 转换为float并存储  
    }
}
