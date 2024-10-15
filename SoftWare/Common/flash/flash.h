#ifndef __FLASH__H
#define __FLASH__H
#include "stdint.h"
#pragma pack(push,1)
typedef struct bsp_flash
{
  char name[32];
  float fbuf[4];
}flash_t;
#pragma pack(pop)


#ifdef STM32F405xx
  #define PID_PARSE_ADDR  ((uint32_t)0x080C0000)
  #define PID_PARSE_SIZE  (48)
#endif

#ifdef STM32G431xx
  #define PID_PARSE_ADDR  ((uint32_t)0x0800F000)
  #define PID_PARSE_SIZE  (48)
#endif

#ifdef STM32G473xx
  #define PID_PARSE_ADDR  ((uint32_t)0x0800F000)
  #define PID_PARSE_SIZE  (48)
#endif

void user_flash_test(void);
void user_flash_earse(uint32_t addr,uint16_t datalen);
void user_flash_write(uint32_t addr,uint8_t *pdata,uint16_t datalen);
void user_flash_read(uint32_t addr,uint8_t *pdata,uint16_t datalen);

#endif
