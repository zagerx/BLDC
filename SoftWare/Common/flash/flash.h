#ifndef __FLASH__H
#define __FLASH__H

#pragma pack(push,1)
typedef struct bsp_flash
{
  char name[32];
  float fbuf[2];
}flash_t;
#pragma pack(pop)


void user_flash_test(void);

#endif
