
### STM32的DSP

### DSP库的下载

### DSP库用法

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

- 高级用法

三相电流的复数表达式:

$i_a = cos(\theta)$

$i_b = cos(\theta - \frac{2}{3}\pi) * e^{j\frac{2\pi}{3}} = cos(\theta - \frac{2}{3}\pi) * (cos(\frac{2\pi}{3}) + jsin(\frac{2\pi}{3}))$

$i_c = cos(\theta + \frac{2}{3}\pi) * e^{-j\frac{2\pi}{3}} = cos(\theta + \frac{2}{3}\pi) * (cos(\frac{2\pi}{3}) - jsin(\frac{2\pi}{3}))$

- 合成磁场$i_{abc}$

    $i_{abc} = \frac{3}{2}e^{\frac{2\pi}{3} \theta j}$



