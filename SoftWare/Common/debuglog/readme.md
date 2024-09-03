## 模块移植
- 模块默认使用RTT打印
    - 开启UART，添加宏定义`C_DEFS += -DDEBUG_MODE_UART`
- makefile文件中添加如下文件及头文件路径
```
C_SOURCES += $(wildcard ../Common/debuglog/SEGGER_RTT_V792h/RTT/*.c)
C_SOURCES += $(wildcard ../Common/debuglog/*.c)
C_INCLUDES += -I../Common/debuglog/SEGGER_RTT_V792h/Config 
C_INCLUDES += -I../Common/debuglog/SEGGER_RTT_V792h/RTT 
C_INCLUDES += -I../Common/debuglog 
```
- UART模式下，在`uart.c`文件中添加`printf`重定向函数
```C
int _write(int file, char *data, int len)
{
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, (uint8_t*)data, len, 1000);
    return (status == HAL_OK ? len : 0);
}
```
