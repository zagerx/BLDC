#ifndef __BSP_ONCHIPFLASH__H
#define __BSP_ONCHIPFLASH__H

#include "stdint.h"
uint8_t bsp_flash_write(uint32_t addr,uint8_t *padta,uint16_t datalen);
void bsp_flash_read(uint32_t addr,uint8_t *pdata,uint16_t datalen);
uint8_t bsp_flash_earse(uint32_t addr,uint16_t datalen);

#endif
