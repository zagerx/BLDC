# 串行存储

## 模块配置

## 接口函数
``` C
 uint8_t bsp_flash_write(uint32_t addr,uint8_t *padta,uint16_t datalen);
  void bsp_flash_read(uint32_t addr,uint8_t *pdata,uint16_t datalen);
```

## STM32内部FLASH

### STM32F405RG
- 参考资料:
- `xxx\STM32Cube\Repository\STM32Cube_FW_F4_V1.28.0\Projects\STM32F4-Discovery\Examples\FLASH\FLASH_EraseProgram`
