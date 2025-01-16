#include "protocol_transmit.h"
#include "taskmodule.h"
#include "main.h"
#include "linkedlist.h"
#include "debuglog.h"
#include "string.h"
#include "heap.h"
#include "protocol_frame.h"

CREAT_LIST_WITH_TYPE(msg, msg_node_t)
// 创建任务链表
#define CREAT_LIST(type)                    create_##type##_list()
#define IS_LIST_EMPTY(list)                 is_##list##_empty(list)
#define DELETE_LIST(list)                   delete_##list(list)
#define DELETE_LIST_NODE(list, node)        delete_##list##_node(list, node)
#define GET_LIST_NODE(list)                 get_##list##_head(list)
#define TRAVERSE_LIST(list)                 traverse_##list(list)//遍历链表

msg_list_t* msg_list = 0;
static _copy_fsm_rt_t msg_send(msg_node_t *msg);
__attribute__((weak)) void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
    USER_DEBUG_NORMAL("please write _bsp_protransmit(protocol_transmit.c)\n");    
}
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
static _copy_fsm_rt_t msg_send(msg_node_t *msg)
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
        if (msg->fsm_cblock.time_count++/(1u) > msg->fsm_cblock.time_out)
        {
            msg->fsm_cblock.time_count = 0;
            msg->fsm_cblock.time_out = 0;
            msg->fsm_cblock._state = TIME_OUT;
        }
        break;
    case TIME_OUT:
        // USER_DEBUG_NORMAL(" send finish\n");
        // for (unsigned short i = 0; i < msg->datalen; i++)
        // {
        //     /* code */
        //     USER_DEBUG_NORMAL("0x%x  ",msg->pdata[i]);
        // }
        // USER_DEBUG_NORMAL("\n");
        _bsp_protransmit(msg->pdata,msg->datalen);
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



extern msg_list_t* msg_list;
void mc_protocol_send(unsigned short cmd,unsigned char* pdata,unsigned short datalen,\
                      unsigned char time_count,unsigned short time_out)
{
    frame_t frame;
    frame.cmd = cmd;
    frame.pdata = (unsigned char*)pdata;
    frame.datalen = datalen;
    unsigned char *p = 0;
    p = _pack_proframe(&frame); 
    msg_node_t* msg = (msg_node_t*)heap_malloc(sizeof(msg_node_t));
    if (msg == NULL)
    {
        USER_DEBUG_NORMAL("msg_node_t malloc fail\n");
        return;
    }
    msg->fsm_cblock.time_count = time_count;
    msg->fsm_cblock.time_out = time_out;
    msg->fsm_cblock._state = 0;
    msg->pdata = p;
    msg->datalen = frame.datalen + sizeof(frame_t) - 4;
    insert_msg_list_tail(msg_list, msg);
}

void mc_protocol_nowsend(unsigned short cmd,unsigned char* pdata,unsigned short datalen)
{
    frame_t frame;
    frame.cmd = cmd;
    frame.pdata = (unsigned char*)pdata;
    frame.datalen = datalen;
    unsigned char *p = 0;
    p = _pack_proframe(&frame); 
    uint16_t len = frame.datalen + sizeof(frame_t) - 4;
    _bsp_protransmit(p,len);
    heap_free(p);
}







