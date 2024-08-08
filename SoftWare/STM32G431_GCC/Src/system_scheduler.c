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


// #include "msgqueue.h"
// void msg_test(void)
// {
//     USER_DEBUG_NORMAL("Msg test\n");
// }
// void thread_01(void)
// {
//     msg_list_t* msg_list =0 ;
//     msg_list = CREAT_MSG_LIST();

//     msg_node_t* msg1 = (msg_node_t*)malloc(sizeof(msg_node_t));
//     if (msg1)
//     {
//         msg1->msg.msg_id = 1;
//         msg1->msg.pfunc = msg_test;
//         USER_DEBUG_NORMAL("msg malloc ok\n");
//     }

//     if (!INSERT_LIST_TAIL(msg_list, msg1))
//     {
//         return;
//     }

//     // 检查任务链表是否为空
//     if (IS_LIST_EMPTY(msg_list)) {
//         USER_DEBUG_NORMAL("msg list is empty.\n");
//         return;
//     } 

//     USER_DEBUG_NORMAL("msg list is not empty.\n");
//     msg_node_t *cur_node;

//     for (; ;)
//     {
//         USER_DEBUG_NORMAL("traverse msg list\n");
//         cur_node = TRAVERSE_LIST(msg_list);
//         if (!cur_node)
//         {
//             USER_DEBUG_NORMAL("traverse finish\n");
//             break;
//         }
        
//         if (!(cur_node->msg.pfunc))
//         {
//             continue;
//         }
        
//         cur_node->msg.pfunc();
//     }
    
    

    // msg_node_t *cur_msg_node = GET_LIST_NODE(msg_list);

// }


/*
msg_queue.c     /     queue.c
*/

/*  
thread01.c                  thread01.c                
void thread_01(void)        void thread_01(void)        
{                           {                     
    生产一条消息                 消费一条消息
}                           }                      
*/                   


/*
thread02.c                   thread02.c                      
void thead_02(void)          void thead_02(void)
{                            {                  
    生产一条消息                 消费一条消息     
}                            }                  
*/                 
