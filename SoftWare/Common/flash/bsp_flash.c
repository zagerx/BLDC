#include "bsp_flash.h"
#include "string.h"

#include "debuglog.h"
#include "main.h"
#pragma pack(push,1)
typedef struct bsp_flash
{
  char name[32];
  float fbuf[2];
}flash_t;
#pragma pack(pop)

typedef union {  
    uint8_t bytes[8];  
    uint64_t value; 
} Uint8ToUint64;  
  
typedef union {  
    uint32_t value;    
    uint8_t bytes[4];  
} Uint32ToUint8Array;  
  
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

void user_flash_test(void)
{
  flash_t test_write = {
    .name = "hello world,hi zager",
    .fbuf = {1.28f,2.78f},
  };
  bsp_flash_earse(ADDR_FLASH_PAGE_31,40);
  bsp_flash_write(ADDR_FLASH_PAGE_31,(uint8_t *)&test_write,40);
  flash_t read_data;
  bsp_flash_read(ADDR_FLASH_PAGE_31,(uint8_t *)&read_data,40);
  USER_DEBUG_NORMAL("%s\n",read_data.name);
  USER_DEBUG_NORMAL("%f %f\n",read_data.fbuf[0],read_data.fbuf[1]);
}

static uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}

static uint32_t GetBank(uint32_t Addr)
{
  return FLASH_BANK_1;
}


