#include "prot_send.h"

enum{
    ENTER = 0,
    TIME_OUT,
    IDLE,
    EXIT,
};

#include "linkedlist.h"
#include "debuglog.h"
#include "string.h"

CREAT_LIST_WITH_TYPE(msg, msg_node_t)
// 创建任务链表

#define CREAT_LIST(list)                    create_##list(list)
#define INSERT_LIST_TAIL(list, node)        insert_##list##_tail(list, node)
#define IS_LIST_EMPTY(list)                 is_##list##_empty(list)
#define DELETE_LIST(list)                   delete_##list(list)
#define DELETE_LIST_NODE(list, node)        delete_##list##_node(list, node)
#define GET_LIST_NODE(list)                 get_##list##_head(list)
#define TRAVERSE_LIST(list)                 traverse_##list(list)//遍历链表












static fsm_rt_t msg_send(fsmcb_t *fsm)
{
    switch (fsm->_state)
    {
    case ENTER:
        
        break;
    case TIME_OUT:
        
        break;
    case IDLE:
        
        break;              
    default:
        break;
    }
}
msg_list_t* msg_list = 0;

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
        // 创建任务链表
        CREAT_LIST(msg_list);        
        break;
    case RUNING:
        // 检查任务链表是否为空
        if (IS_LIST_EMPTY(msg_list)) {
            return;
        }
        USER_DEBUG_NORMAL("Task list is not empty.\n");
        msg_node_t *cur_node;
        do
        {
            cur_node = TRAVERSE_LIST(msg_list);
            if (msg_send(&(cur_node->fsm_cblock)) == RT_FINISH)
            {
                free(cur_node->pdata);
                DELETE_LIST_NODE(msg_list, cur_node);
            }
        } while (cur_node);
        // msg_node_t *cur_msg_node = GET_LIST_NODE(msg_list);
        break;    
    default:
        break;
    }



    
    // 在链表中插入任务节点

    
    
    // 在链表中删除任务节点
    
    
    // 销毁任务链表
    // DELETE_LIST(msg_list);
    
    // return;
}


extern void protocol_sendprocess(void)
{
    /*从发送链表上取出一个节点*/
    


}

