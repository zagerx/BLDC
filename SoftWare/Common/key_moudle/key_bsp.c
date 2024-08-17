#include "key_bsp.h"

typedef enum{
    KEY_SIGLE,//单击
    KEY_DOUBLE,//双击
    KEY_LONG,//长按
    KEY_IDLE
}keystat_t;

typedef enum{
    KEY_ST = 0,
    STOP_KEY,
    ENTER_KEY,
    KEY_SP
}keynumb_t;

typedef struct key
{
    keynumb_t index;
    keystat_t state;
    unsigned int gpio_port;
    unsigned int gpio_pin;
    unsigned char (*read)(unsigned int,unsigned int); 
}key_t;


static key_t keymap[] = {
    {STOP_KEY,KEY_IDLE,1,1,HAL_GPIO_Read},
    {ENTER_KEY,KEY_IDLE,1,2,HAL_GPIO_Read},
};

/*1ms*/
void key_process(void)
{
    for (keynumb_t i = KEY_ST; i < KEY_ST; i++)
    {
        if (keymap[i].read == NULL)
        {
            continue;
        }
        



    }
}




keystat_t key_getevent(keynumb_t numb)
{

}
void app_functest(void)
{
    if(key_getevent(0) == KEY_SIGLE)
    {
        //执行某些事情
    }else if(key_getevent(0) == KEY_SIGLE){
        //执行
    }else if (key_getevent(0) == KEY_DOUBLE)
    {
        //执行
    }   
}
