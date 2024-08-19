#ifndef _CRC_H_
#define _CRC_H_

#include <stdint.h>


extern unsigned long crc32_calc(const unsigned char *s, unsigned int len);

extern uint8_t crc8_calc(unsigned char *ptr, unsigned int len);

extern uint16_t crc16_calc(uint8_t *pBuf,uint16_t value, uint32_t size);

extern uint32_t crc32_subsection(const unsigned char *s, uint32_t value, uint32_t len);

#endif


