
# Motorctrl模块
## 模块配置
### 对外接口
-   `motorctrl_process()函数`
-   `motorctrl_init()函数`
-   `mc_hightfreq_task(float *iabc)函数`
### 对内配置
- `__attribute__((weak)) void motor_enable(void)`
- `__attribute__((weak)) void motor_disable(void)`
- `__attribute__((weak)) void motor_set_pwm(...)`
- `__attribute__((weak)) void _bsp_protransmit(...)`
一般情况，上面的函数放在`board.c/h`定义和声明

## 函数说明
### motorctrl_process()函数
- 正常模式(normalmodel.c)
- 调试模式(debugmodel.c)
### mc_hightfreq_task(float *iabc)函数
- 电流环入口(8KHZ/10KHZ/20KHZ...)

### mc_debugmode.c
#### mc_test()函数说明

##### 原理:
- 基本公式:
    - $\theta = \omega t$
    - $\omega = 2 \pi /t$
- 给定角度$2\pi$,3s旋转一圈。
- `mc_test()`函数被调周期周期0.000125s
- 每次角度自增为:$\theta = 2\pi/t * 0.000125 * cnt$

##### 功能:
- 可观察函数三相电流
- 编码器角度
- iq,id
### mc_encoderopenlooptest()
- 编码器开环
    - 通过使能该功能，开启该功能
- 调试过程
    - 用手堵转电机，`i_abc,iq,id`如下:
![](./images/编码器开环效果图.png)


