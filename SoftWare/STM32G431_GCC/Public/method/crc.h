#ifndef _CRC_H_
#define _CRC_H_

#include <stdint.h>

extern void printf_test(const char* name, uint8_t* buff, uint8_t len);

extern unsigned long CalcCRC32(const unsigned char *s, unsigned int len);

extern uint8_t CalcCRC8(unsigned char *ptr, unsigned int len);

/* 分段计算CRC16 */
extern uint16_t CRC16_Subsection(uint8_t *pBuf,uint16_t value, uint32_t size);

/* 分段计算CRC32 */
extern uint32_t CRC32_Subsection(const unsigned char *s, uint32_t value, uint32_t len);

#endif


