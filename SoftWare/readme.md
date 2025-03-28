# 配置步骤
## 1、makefile配置
-   Jlink
```
JLINKEXE := JLink.exe 
JLINKGDBSERVER := JLinkGDBServer.exe
CHIP := STM32G431C8(替换成对应型号)
GDB := arm-none-eabi-gdb.exe

/*=========================================*/
jlinkdown:
	$(Q)echo "h" >> jlink.jlink
	$(Q)echo "loadfile" $(BUILD_DIR)/$(TARGET).hex >> jlink.jlink
	$(Q)echo "r" >> jlink.jlink             
	$(Q)echo "q" >> jlink.jlink   
	$(Q)$(JLINKEXE) -device $(CHIP) -Speed 4000 -IF SWD -CommanderScript jlink.jlink
```

- 增加printf打印Float功能
    `LDFLAGS += -lrdimon -u _printf_float`
- 开启一些编译报错

```
CFLAGS += -Werror=implicit-function-declaration
CFLAGS += -Werror=array-bounds -Werror=incompatible-pointer-types
```

## 2、添加debuglog模块
- 使能Uart打印
```
C_SOURCES += $(wildcard ../Common/debuglog/SEGGER_RTT_V792h/RTT/*.c)
C_SOURCES += $(wildcard ../Common/debuglog/*.c)
C_INCLUDES += -I../Common/debuglog/SEGGER_RTT_V792h/Config 
C_INCLUDES += -I../Common/debuglog/SEGGER_RTT_V792h/RTT 
C_INCLUDES += -I../Common/debuglog 
C_DEFS += -DDEBUG_MODE_UART
```
- 在uart.c文件中实现，printf函数重定向
```
#include "stdio.h"
int _write(int file, char *data, int len)
{
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart3, (uint8_t*)data, len, 1000);
    return (status == HAL_OK ? len : 0);
}
```

## 3、添加系统模块
```
C_INCLUDES += -I../Common/systemmodule
C_SOURCES += $(wildcard ../Common/systemmodule/*.c)
```
- 修改.ld文件
```
  .initcalls :
  {
    . = ALIGN(8);
    _stext = .;
    __initcall_start = .;
    KEEP(*(.initcall.0.init)) 
    KEEP(*(.initcall.1.init)) 
    KEEP(*(.initcall.2.init)) 
    KEEP(*(.initcall.3.init)) 
    KEEP(*(.initcall.4.init)) 
    KEEP(*(.initcall.5.init)) 
    __initcall_end = .;
    . = ALIGN(8);
  } >FLASH

  .taskcalls :
  {
    . = ALIGN(8);
    __taskcall_start = .;
    KEEP(*(.taskcall.0.task)) 
    KEEP(*(.taskcall.1.task)) 
    KEEP(*(.taskcall.2.task)) 
    KEEP(*(.taskcall.3.task)) 
    KEEP(*(.taskcall.4.task)) 
    KEEP(*(.taskcall.5.task)) 
    __taskcall_end = .;
    . = ALIGN(8);
  } >FLASH
```
- 修改启动.s文件
`bl do_initcalls`
- 修改main.c
```
    do_taskcalls();
    HAL_Delay(1);
```

## 4、添加协议模块
```

C_INCLUDES += -I../Common/data_structures
C_SOURCES += $(wildcard ../Common/data_structures/heap.c)
C_SOURCES += $(wildcard ../Common/data_structures/fifo.c)
C_INCLUDES += -I../Protocol
C_SOURCES += $(wildcard ../Protocol/*.c)

```
- 在`uart.c`文件中添加如下代码段
```C
#include "string.h"
static uint8_t sg_uartreceive_buff[125];

#include "protocol.h"
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    if(USART3 == huart->Instance)                                   
    {
        if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart);                     
            HAL_UART_DMAStop(huart);
            unsigned short data_length  = sizeof(sg_uartreceive_buff) - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
            protocol_getdata_tofifo(sg_uartreceive_buff,data_length);
            memset(sg_uartreceive_buff,0,data_length);
            data_length = 0;
            HAL_UART_Receive_DMA(huart, (uint8_t*)sg_uartreceive_buff, sizeof(sg_uartreceive_buff));                   
        }
    }
}
```
- 在串口中断回调中添加如下代码段:
```
USER_UART_IRQHandler(&huart3);
```
- 打开MCOM软件，发送速度指令
- 打开RTT View软件

## 5、添加motorctrol模块

motorctrol模块依赖:
- pid模块
- board.c/.h(配置项)
  - 根据电机编码器选择对应的模块
    - 霍尔编码器
      - 添加hall_sensor模块，并添加宏定义
      ```
      C_DEFS += -DENCODER_TYPE_HALL
      C_SOURCES += $(wildcard ../Hardware/hall_sensor.c)
      C_INCLUDES += -I../Hardware 
      ```
      - 添加HALL_SENSOR对应的引脚
      
    - 绝对值编码器
      根据需要添加对应的模块


```

C_INCLUDES += -I../Common/method 
C_SOURCES += $(wildcard ../Common/method/pid.c)
C_SOURCES += $(wildcard ../Common/method/filter.c)

C_INCLUDES += -I../Common/Fsm 
C_INCLUDES += -I../BLDCMotor 
C_SOURCES += $(wildcard ../BLDCMotor/*.c)
# C_DEFS += -DMOTOR_OPENLOOP 
# C_DEFS += -DMOTOR_OPENLOOP_ENCODER 
```

