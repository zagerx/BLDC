#include "flash.h"
#include "bsp_onchipflash.h"
#include "debuglog.h"


void user_flash_test(void)
{
  flash_t test_write = {
    .name = "hello world,dev1_flash",
    .fbuf = {-1.28f,2.78f},
  };
  bsp_flash_earse(((uint32_t)0x0800E000),40);
  bsp_flash_write(((uint32_t)0x0800E000),(uint8_t *)&test_write,40);
  flash_t read_data;
  bsp_flash_read(((uint32_t)0x0800E000),(uint8_t *)&read_data,40);
  USER_DEBUG_NORMAL("%s\n",read_data.name);
  USER_DEBUG_NORMAL("%f %f\n",read_data.fbuf[0],read_data.fbuf[1]);
}


void user_flash_earse(uint32_t addr,uint16_t datalen)
{
  bsp_flash_earse(addr,datalen);
}

void user_flash_write(uint32_t addr,uint8_t *pdata,uint16_t datalen)
{
  bsp_flash_write(addr,pdata,datalen);
}

void user_flash_read(uint32_t addr,uint8_t *pdata,uint16_t datalen)
{
  bsp_flash_read(addr,pdata,datalen);
}