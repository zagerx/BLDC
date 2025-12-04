# 程序框架

## 万物介对象
从什么地方开始呢？我们就以人的思维来控制一个电机。人通常会这样说：
- 电机进入空闲状态
- 电机进入运行状态
- 电机进入位置模式/速度模式
- 电机以xx速度转动
- 电机去到xx位置
- 电机当前处在什么状态
- 电机立即停止

好了，那么我们就以上面的内容来一步一步的构造我们的电机库

## 状态机
在构造电机库之前，我们先来做一个状态机模块。
下面的代码实现了什么功能：
- 定义了一个对象fsm_handle，当拿到这个handle，就可以获取状态机的所有功能以及数据
- 程序在main函数里面不停的调度(1ms)这个状态机，此时这个状态机会不停的执行idle_state这个函数，使这个函数不停的执行case RUNING里面的内容
- 当接收到外部触发，执行TRAN_STATE之后，他就会立即执行idle_state状态下的EXIT条件，然后执行新状态下的EXTEN条件，之后就会不停的执行新状态下的RUNNIGN。


```c
struct fsm_cb_t{
        void (*fsm)(fsm_cb_t*);
}
TRAN_STATE(fsm_handle,targe_state)
DISPATCH(fsm_handle)
```

```C
main.c

struct fsm_cb_t fsm_handle = {
        .fsm = idle_state,
};

void idle_state(fsm_cb_t *this)
{
        switch(this->state){
                case ENTER:
                break;
                case RUNING:
                break;
                case EXIT:
                break;
        }
}

void falut_state(fsm_cb_t *this)
{
        switch(this->state){
                case ENTER:
                break;
                case RUNING:
                break;
                case EXIT:
                break;
        }
}

void main(void)
{
        while(1)
        {
                HAL_Delay(1);
                DISPATH(&fsm_handle)
        }
}

xx.c
void cmd_task(void)
{
        if(){
                TRAN_TASTE(&fsm_handle,falut_state);
        }

}
```

## 定义电机对象

```C
fsm_cb_t motor1_fsm = {
        .fsm = motor_idle_state;
};

struct device {
        char *name;
        void *config,
        void *data,
        void *api,
}
struct motor_data{
        fsm_cb_t *fsm_hanle;
}

struct motor_data motor1_data = {
        .fsm_handle = &motor1_fsm;
}
struct device motor_1;

void motor_idle_state(fsm_cb_t* this)
{

}
void motor_falut_state(fsm_cb_t* this)
{

}

main()
{
        struct motor_data *m_data = motor_1.data;
        while(1)
        {
                if(flag)
                {
                        TRAN_STATE(m_data->fsm_handle,motor_falut_state);
                }
        }

}

void irq_adc(void)
{
        adc_raw[0] = HAL_XXX()
        ...
        foc_curr_reglar(&motor_1,adc_raw);
}
void foc_curr_reglar(struct device *motor,uint32_t *uvw_raw)
{
        struct motor_data *m_data = motor_1.data;

        DISPATCH(m_data->fsm_handle);
}
```


