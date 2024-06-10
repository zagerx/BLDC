#include "system_scheduler.h"
#include "gpio.h"
#define SYSRUNNING_PERCI            (1)
#define DELAY_1MS                   (1)/SYSRUNNING_PERCI
#define DELAY_2MS                   (2)/SYSRUNNING_PERCI
#define DELAY_5MS                   (5)/SYSRUNNING_PERCI
#define DELAY_20MS                  (20)/SYSRUNNING_PERCI
#define DELAY_500MS                 (500)/SYSRUNNING_PERCI
#define DELAY_1000MS                (1000)/SYSRUNNING_PERCI
#define DELAY_5000MS                (5000)/SYSRUNNING_PERCI
typedef struct
{
    /* data */
    unsigned int time_cnt;
    unsigned char state;
}sys_run_t;
static sys_run_t sg_SYSRuning;
void sysrunning_process(void)
{
    enum{
        SYS_IDLE,
        SYS_NORMLE,
    };
    /*-----------------------*/
    sg_SYSRuning.time_cnt++;
    switch (sg_SYSRuning.state)
    {
        case SYS_IDLE:
            if(!(sg_SYSRuning.time_cnt % (DELAY_5000MS))){
                sg_SYSRuning.time_cnt = 0;
                break;
            }
            if(!(sg_SYSRuning.time_cnt % (DELAY_1000MS))){
            }
            if(!(sg_SYSRuning.time_cnt % (DELAY_500MS)))
            {
            }            
            if(!(sg_SYSRuning.time_cnt % (DELAY_20MS)))
            {
                HAL_GPIO_TogglePin(LED_01_GPIO_Port,LED_01_Pin);                
            }
            if(!(sg_SYSRuning.time_cnt % (DELAY_2MS))){
            }
        default:
            break;
    }
}

