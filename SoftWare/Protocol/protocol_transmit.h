#ifndef __PROT_SEND__H
#define __PROT_SEND__H
#include "stdbool.h"
#include "stdint.h"
typedef struct _fsm_cb
{
    unsigned short time_out;
    unsigned short time_count;
    short _state;
}fsmcb_t;

// 定义任务结构体
typedef enum{
    RT_FINISH,
    RT_RUNING,
    RT_START,
}_copy_fsm_rt_t;

typedef struct msg_node {
    fsmcb_t fsm_cblock;
    unsigned char ack;
    unsigned char *pdata;
    unsigned short datalen;
    struct msg_node* next_node;
} msg_node_t;

typedef struct msg_list { 
    msg_node_t* head; 
    msg_node_t* tail; 
    msg_node_t* cur;
    int16_t node_num; 
} msg_list_t; 

bool insert_msg_list_tail(msg_list_t* pthis, msg_node_t* node);
void msglist_process(void);
#endif
