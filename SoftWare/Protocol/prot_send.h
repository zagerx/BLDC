#ifndef __PROT_SEND__H
#define __PROT_SEND__H


typedef struct fsm_cb
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
}fsm_rt_t;

typedef struct msg_node {
    fsmcb_t fsm_cblock;
    unsigned char *pdata;
    unsigned short datalen;
    struct msg_node* next_node;
} msg_node_t;

void test_aaa(void);
#include "stdbool.h"
#include "stdint.h"
// #define CREAT_LIST_WITH_TYPE(type, node_type) \
//     typedef struct type##_list { \
//         node_type* head; \
//         node_type* tail; \
//         node_type* cur;\
//         int16_t node_num; \
//     } type##_list_t; 
// CREAT_LIST_WITH_TYPE(msg, msg_node_t)


// bool insert_msg_list_tail(msg_list_t* pthis,msg_node_t* node);
#endif
