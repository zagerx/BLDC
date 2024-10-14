
### STM32的DSP

### DSP库的下载
    - `XXX\STM32G431_GCC\Drivers\CMSIS\DSP`
### DSP库用法
- 源文件:`/DSP/Source/`
- 头文件:`DSP/Include/`
需要哪些功能就添加哪些功能的源文件，如sin，cos三角函数
#### 三角函数的添加
```
C_INCLUDES += -IDrivers/CMSIS/DSP/Include
C_SOURCES += $(wildcard Drivers/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.c)
C_SOURCES += $(wildcard Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.c)
```
#### 复数

- 复数的点乘
  
    `void arm_cmplx_dot_prod_f32(...)`
- 基础的使用方法:

```C
float32_t pSrcA[10] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
float32_t pSrcB[10] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
float32_t realResult;
float32_t imagResult;

arm_cmplx_dot_prod_f32(pSrcA, pSrcB, 5, &realResult, &imagResult);
```
