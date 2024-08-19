$$ \theta = \omega t $$
给定角度$2\pi$,3s旋转一圈。则角速度$\omega = 2 \pi/t$\
task()函数执行周期0.000125s
所以每次角度自增为:\
$$\theta = 2\pi/t * 0.000125 * cnt$$
```C

void mc_test(float *iabc)
{
    duty_t duty = {0};
    static unsigned short cnt = 0;
    float theta = 0.0f;
    theta = S_TOtil/TIME_TOTIL * 0.000125F * cnt;//机械角度
    theta = theta / 7.0f;//电角度
    next_theta = S_TOtil/TIME_TOTIL/7.0f;
    duty = currment_loop(iabc,theta,next_theta);
    motor_set_pwm(duty._a,duty._b,duty._c);
    cnt++;
}

```