
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

$i_c = cos(\theta + \frac{2}{3}\pi) * e^{-j\frac{2\pi}{3}} = cos(\theta + \frac{2}{3}\pi) * (**cos**(\frac{2\pi}{3}) - jsin(\frac{2\pi}{3}))$

- 合成磁场$i_{abc}$

    $i_{abc} = \frac{3}{2}e^{\frac{2\pi}{3} \theta j}$

$\frac{2}{3}$

### 读取角度

- 定位坐标系。`svpwm(1,0)`，强拖转子到$\alpha \beta$坐标系下(1,0)位置。
- 读取转子当前角度 `read_angle()`，此时得到的角度为物理角度。假设值为1.13。说明传感器零点和坐标系零点偏差1.13度。
  
  注意:该角度值并不唯一。角度值的个数与电机极对数相关

- 给电机自增角度，经过角度变换的一些操作，传感器读到的角度应该和自增角度完全一致
  
    ``` C
    switch(statu){
        case 0:
            _2r_2s(0,1,ele_angle);
            svpwm(alpha,beta);
            ele_angle += 0.31415926f;
            statu = 1;
        break;
        case 1:
            _2r_2s(0,1,ele_angle);
            svpwm(alpha,beta);
            ele_angle += 0.31415926f;        
        break;
        delafu:
        break;
    }
    ...
    ```

- 使用传感器角度，使电机运行在开环状态下
  
  - `_2r_2s(1,0,ele_angle)` 执行该函数，电机无法转动。且alpha值越大，转动电机的阻力越大
  
  - `_2r_2s(0,1,ele_angle);`执行该函数，电机应该逆时针转动

  - `_2r_2s(0,1,ele_angle);`执行该函数，电机应该顺时针转动

  - 随着iq的变化，所观测到的$i_a,i_b,i_c$的大小应该同相变化。  










  