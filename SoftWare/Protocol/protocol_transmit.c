#include "protocol_transmit.h"
#include "taskmodule.h"
#include "main.h"
#include "linkedlist.h"
#include "debuglog.h"
#include "string.h"
#include "heap.h"

CREAT_LIST_WITH_TYPE(msg, msg_node_t)
// 创建任务链表
#define CREAT_LIST(type)                    create_##type##_list()
#define IS_LIST_EMPTY(list)                 is_##list##_empty(list)
#define DELETE_LIST(list)                   delete_##list(list)
#define DELETE_LIST_NODE(list, node)        delete_##list##_node(list, node)
#define GET_LIST_NODE(list)                 get_##list##_head(list)
#define TRAVERSE_LIST(list)                 traverse_##list(list)//遍历链表

msg_list_t* msg_list = 0;
static fsm_rt_t msg_send(msg_node_t *msg);

void msglist_process(void)
{
    enum{
        INIT = 0,
        RUNING,
    };
    static short _state = INIT; 
    switch (_state)
    {
    case INIT:
        msg_list = CREAT_LIST(msg);  
        _state = RUNING; 
        break;
    case RUNING:
        {
            msg_node_t *cur_node;
            cur_node = GET_LIST_NODE(msg_list);
            if (cur_node == NULL)
            {
                return;
            }
            if (msg_send((msg_node_t*)&(cur_node->fsm_cblock)) == RT_FINISH)
            {
                heap_free(cur_node->pdata);
                DELETE_LIST_NODE(msg_list, cur_node);
            }
        }
        break;    
    default:
        break;
    }
}
static fsm_rt_t msg_send(msg_node_t *msg)
{
    enum{
        ENTER = 0,
        TIME_OUT,
        IDLE,
        EXIT,
    };
    switch (msg->fsm_cblock._state)
    {
    case ENTER:
        if (msg->fsm_cblock.time_count++ > msg->fsm_cblock.time_out)
        {
            msg->fsm_cblock.time_count = 0;
            msg->fsm_cblock._state = TIME_OUT;
        }
        break;
    case TIME_OUT:
        USER_DEBUG_NORMAL(" send finish\n");
        return RT_FINISH;
        break;
    case IDLE:
        break;              
    default:
        break;
    }
    return RT_RUNING;
}
board_task(msglist_process)

