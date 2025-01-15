/****************************************************************************
Filename: datatypecover.h
Functions list:
Notes:
Copyright (c) 2022, Syrius Robotics
All rights reserved.
****************************************************************************/


#ifndef __DATATYPECOVER__H
#define __DATATYPECOVER__H

#ifdef __cplusplus
extern "C"{
#endif

typedef union {  
    float f;  
    uint32_t i;  
} float_int_union; 
void convert_floats(unsigned char *pdata, unsigned short datalen, float *floats);
#ifdef __cplusplus
}
#endif

#endif	/* __DATATYPECOVER__H */
