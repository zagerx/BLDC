#include "bsp_onchipflash.h"
#include "string.h"
#include "debuglog.h"

typedef union {  
    uint8_t bytes[8];  
    uint64_t value; 
} Uint8ToUint64;  

typedef union {  
    uint32_t value;    
    uint8_t bytes[4];  
} Uint32ToUint8Array;  
static inline void uint32_to_uint8_array(uint32_t data, uint8_t *pdata);
static inline uint64_t uint8_array_to_uint64(const uint8_t *p);
static inline uint64_t uint8_array_to_uint64(const uint8_t *p) {  
    Uint8ToUint64 converter;  
    memcpy(converter.bytes, p, sizeof(converter.bytes));  
    return converter.value;  
}    
static inline void uint32_to_uint8_array(uint32_t data, uint8_t *pdata) {  
    Uint32ToUint8Array converter;  
    converter.value = data;  
    pdata[0] = converter.bytes[0];  
    pdata[1] = converter.bytes[1];  
    pdata[2] = converter.bytes[2];  
    pdata[3] = converter.bytes[3];  
}


#if defined(STM32F405XX)
#include "main.h"
#include "_stm32F405_onchipflash.h"
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_2   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_11  +  GetSectorSize(ADDR_FLASH_SECTOR_11) -1 /* End @ of user Flash area : sector start address + sector size -1 */
#define DATA_32                 ((uint32_t)0x12345678)
uint32_t FirstSector = 0, NbOfSectors = 0, Address = 0;
uint32_t SectorError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;
static FLASH_EraseInitTypeDef EraseInitStruct;
static uint32_t GetSector(uint32_t Address);
static uint32_t GetSectorSize(uint32_t Sector);

uint8_t bsp_flash_earse(uint32_t addr,uint16_t datalen)
{
  HAL_FLASH_Unlock();
  FirstSector = GetSector(FLASH_USER_START_ADDR);
  /* Get the number of sector to erase from 1st sector*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = FirstSector;
  EraseInitStruct.NbSectors = NbOfSectors;
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    HAL_FLASH_Lock();
    return -1;
  }

  __HAL_FLASH_DATA_CACHE_DISABLE();
  __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
  __HAL_FLASH_DATA_CACHE_RESET();
  __HAL_FLASH_INSTRUCTION_CACHE_RESET();
  __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
  __HAL_FLASH_DATA_CACHE_ENABLE();

  return 0;
}


uint8_t bsp_flash_write(uint32_t addr,uint8_t *padta,uint16_t datalen)
{
  if (datalen%8 != 0)
  {
    return -1;
  }
  HAL_FLASH_Unlock();
  Address = FLASH_USER_START_ADDR;

  while (Address < FLASH_USER_END_ADDR)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
    {
      Address = Address + 4;
    }else
    { 
      HAL_FLASH_Lock(); 
      USER_DEBUG_NORMAL("flash write err\n");
      return -1;
    }
  }
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock(); 
  return 0;
}
void bsp_flash_read(uint32_t addr,uint8_t *pdata,uint16_t datalen)
{
  Address = FLASH_USER_START_ADDR;
  MemoryProgramStatus = 0x0;
  
  while (Address < FLASH_USER_END_ADDR)
  {
    data32 = *(__IO uint32_t*)Address;

    if (data32 != DATA_32)
    {
      USER_DEBUG_NORMAL("Flash read err\n");
      return ;
    }

    Address = Address + 4;
  }  
  USER_DEBUG_NORMAL("Flash read sucessful\n");
  return;
}

static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;  
  }
  else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11) */
  {
    sector = FLASH_SECTOR_11;
  }

  return sector;
}

static uint32_t GetSectorSize(uint32_t Sector)
{
  uint32_t sectorsize = 0x00;

  if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3))
  {
    sectorsize = 16 * 1024;
  }
  else if(Sector == FLASH_SECTOR_4)
  {
    sectorsize = 64 * 1024;
  }
  else
  {
    sectorsize = 128 * 1024;
  }  
  return sectorsize;
}

#endif



#if defined(STM32G431xx) || defined(STM32G473xx)
#include "main.h"
#include "bsp_stm32G431_onchipflash.h"
static uint32_t GetPage(uint32_t Address);
static uint32_t GetBank(uint32_t Addr);
uint8_t bsp_flash_earse(uint32_t addr,uint16_t datalen)
{
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
  uint32_t FirstPage = 0, NbOfPages = 0,BankNumber = 0;
  FirstPage = GetPage(addr);
  NbOfPages = GetPage(addr + datalen) - FirstPage + 1;
  BankNumber = GetBank(addr);
  uint32_t PageError = 0;
  FLASH_EraseInitTypeDef EraseInitStruct;
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
  EraseInitStruct.Banks = BankNumber;
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
  {
    HAL_FLASH_Lock();
    return -1;
  }
  return 0;
}

uint8_t bsp_flash_write(uint32_t addr,uint8_t *padta,uint16_t datalen)
{
  if (datalen%8 != 0)
  {
    return -1;
  }
  uint32_t end_addr = addr + datalen;
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
  while (addr < end_addr)
  {
    uint64_t data = uint8_array_to_uint64(padta);
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, data) != HAL_OK)
    {
       USER_DEBUG_NORMAL("FLASH write ERR\n");
       HAL_FLASH_Lock();
       return -1;
    }
    padta += 8; 
    addr += 8;
  }
  HAL_FLASH_Lock();
  return 0;
}

void bsp_flash_read(uint32_t addr,uint8_t *pdata,uint16_t datalen)
{
  __IO uint32_t data32 = 0;

  uint32_t end_addr = addr+datalen;
  while (addr < end_addr)
  {
    data32 = *(__IO uint32_t *)addr;
    uint32_to_uint8_array(data32,pdata);
    pdata += 4;
    addr = addr + 4;
  }
}

static uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}

static uint32_t GetBank(uint32_t Addr)
{
  return FLASH_BANK_1;
}
#endif




