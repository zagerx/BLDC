#ifndef __PROTOCOL__H
#define __PROTOCOL__H

enum{
    CMD_ORIGE = 0x00,
    CMD_RESP,
    CMD_ACK,
};
#if 1
    typedef enum{
        PRO_FUNC_TESTCMD01 = 1,
        PRO_FUNC_CMD02,
        PRO_FUNC_CMD03,        
        PRO_FUNC_CMD04,
        CMD_NUMBER
    }E_CMD;
#else
    #define PRO_FUNC_CMD01	0x00	
    #define PRO_FUNC_CMD02	0x01	
    #define PRO_FUNC_CMD04  0x0B	
    #define PRO_FUNC_CMD03  0x0A
#endif

extern void protocol_init(void);
extern void protocol_process(void);
extern void _bsp_protransmit(unsigned char* pdata,unsigned short len);
extern unsigned char protocol_reciverdata_tofifo(unsigned char *pdata,unsigned short len);
extern char protocol_nowtransmit(unsigned char cmd_type,\
                        unsigned char cmd,void *pdata,unsigned short data_len);
extern void protocol_transmit(unsigned char cmd_type,unsigned char cmd,\
                            void *pdata,unsigned short data_len);      
void protocol_transmitprocess(void);

void protocol02_transmit(unsigned char *pdata,unsigned short datalen);
void protocol02_process(void);




/*
protocol.c
void protocol_process(void)
{
    //获取数据getdatafromfifo

    //解包  根据协议具体内容    "可以灵活定制"
    protocol01_unpack();
    
    //遍历协议/命令地图
}


//协议地图
protocol01.c
#include "protocol.h + tim.h..."

void protocol_init(void)
{
    //初始化fifo  接收fifo 发送fifo


    protocol_registerCMD(CMD,CMD_Index,func);

}

灵活定制，采用注册方法


void excufunc(void)
{
    //设置PWM--->添加到消息地图
}


//超时重发机制
protocol_transmit.h

enum{
SEND_STAR,
SEND_RUNING,
SEND_FINISH,
}send_state_t;

type struc{
    state
}send_fsm_t;

type struct{
    struct data,
    delte_flag,
    fsm sendfsm,
    send_pfunc,
    time
    counts
    next
}msg_t;

typedef struct msg_node {
    msg_t msg;
    struct msg_node* next_node;
} msg_node_t;

CREAT_LIST_WITH_TYPE(msg, msg_node_t)
#define CREAT_MSG_LIST()                    create_msg_list()  
#define INSERT_LIST_TAIL(list, node)        insert_##list##_tail(list, node)
#define IS_LIST_EMPTY(list)                 is_##list##_empty(list)
#define DELETE_LIST(list)                   delete_##list(list)
#define DELETE_LIST_NODE(list, node)        delete_##list##_node(list, node)
#define GET_LIST_NODE(list)                 get_##list##_head(list)
#define TRAVERSE_LIST(list)                 traverse_##list(list)




send_state_t send_fsm(send_fsm_t *pthis);



protocol_transmit.c
send_state_t send_fsm(send_fsm_t *pthis)
{
    switch(pthis->stat)
    case 1:
    ...
    case 2:
        !!!!!wait respond!!!!
    break;
}

app.c
#include "protocol.h"
#include "protocol_transmit.h"

数据打包
{
    id,
    len,
    data
    ...
    crc
}data


添加到发送缓冲区(链表)


send_msg.data = 01;
send_msg.delte_flag = 0,
send_pfunc = send_fsm;
count = 3
time = 20
next = NULL;

register_sendlist(sgthis,send_msg);

读取链表
不为空，从头取下
fot(sgthis)
{
    msg
    psend_framfunc,
}

if(send_pfunc(sendfsm) == finish)
{
    send_msg.delte_flag
}


移除该节点
未结束
当前消息的发送状态机----->查看当前状态
psend_framfunc()

*/






#endif
