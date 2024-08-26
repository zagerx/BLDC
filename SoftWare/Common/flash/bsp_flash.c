#include "bsp_flash.h"

#include "debuglog.h"
#include "main.h"

#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_4   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (ADDR_FLASH_PAGE_63 + FLASH_PAGE_SIZE - 1)   /* End @ of user Flash area */

#define DATA_32                 ((uint32_t)0x12345678)
#define DATA_64                 ((uint64_t)0x1234567812345678)

uint32_t FirstPage = 0, NbOfPages = 0;
uint32_t Address = 0, PageError = 0;
__IO uint32_t MemoryProgramStatus = 0;
__IO uint32_t data32 = 0;
static uint32_t GetPage(uint32_t Address);

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;

void bsp_flash_write(uint32_t addr,uint8_t *padta,uint16_t datalen)
{
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

  FirstPage = GetPage(FLASH_USER_START_ADDR);
  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    while (1)
    {
        USER_DEBUG_NORMAL("FLASH Erase Err\n");
        HAL_Delay(1000);
    }
  }


    Address = FLASH_USER_START_ADDR;
  while (Address < FLASH_USER_END_ADDR)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, DATA_64) == HAL_OK)
    {
      Address = Address + 8;  /* increment to next double word*/
    }
   else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
      while (1)
      {
        USER_DEBUG_NORMAL("Flash Write Err\n");
      }
    }

    HAL_FLASH_Lock();
  }
}
void bsp_flash_read(uint32_t addr,uint8_t *pdata,uint16_t datalen)
{
  Address = FLASH_USER_START_ADDR;
  MemoryProgramStatus = 0x0;    
  while (Address < FLASH_USER_END_ADDR)
  {
    data32 = *(__IO uint32_t *)Address;

    if (data32 != DATA_32)
    {
      MemoryProgramStatus++;
    }
    Address = Address + 4;
  }  
  /*Check if there is an issue to program data*/
  if (MemoryProgramStatus == 0)
  {
    USER_DEBUG_NORMAL("flash read OK\n");
  }
  else
  {
    /* Error detected. LED2 will blink with 1s period */
    while (1)
    {
        USER_DEBUG_NORMAL("Flash read Err\n");
    }
  }  
}

static uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}
