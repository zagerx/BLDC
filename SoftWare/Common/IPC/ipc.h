#ifndef __IPC__H
#define __IPC__H

#include "list.h"
#include "macro_defined.h"

/*------------------------�¼��Ķ���(��ͬ��Ŀ�����¼���Ҫ���¶���)-------------------------------------*/
enum{
    INVAIL_EVENT = -1,
    KEY01_SHORT_PRESS,
    IPC_SET_IQ,
};
extern unsigned int gEventGroup;
#define IPC_SET_EVENT(__me,event)        USER_SET_BIT(__me,event)
#define IPC_GET_EVENT(__me,event)        USER_GET_BIT(__me,event)
#define IPC_CLEAR_EVENT(__me,event)      USER_CLEAR_BIT(__me,event)

typedef enum{
    PUBLIC_DATA_START_INDEX,
    PUBLIC_DATA_IQ_TARGET,
    PUBLIC_DATA_IA,
    PUBLIC_DATA_IB,    
    PUBLIC_DATA_IC,
    PUBLIC_DATA_IALPHA,
    PUBLIC_DATA_IBETA,
    PUBLIC_DATA_ID,
    PUBLIC_DATA_IQ,
    PUBLIC_DATA_TCMP1,
    PUBLIC_DATA_TCMP2,
    PUBLIC_DATA_TCMP3,
    PUBLIC_DATA_TEMP0,
    PUBLIC_DATA_TEMP1,    
    PUBLIC_DATA_STOP_INDEX,
}E_PUBLIC_DATA;
float ipc_read_data(E_PUBLIC_DATA index);
char ipc_write_data(E_PUBLIC_DATA index,float data);

node_item_t* ipc_mesg_packet_02(unsigned short id,unsigned short len,void *pbuf);
node_item_t* ipc_mesg_packet(unsigned short id,unsigned short len);
void ipc_msgpool_init(void);
void ipc_msgpool_write(node_item_t *msg);
node_item_t* ipc_msgpool_read(void);
node_item_t* ipc_msgpool_read_val(node_item_t* msg);
void ipc_msgpool_del(node_item_t *msg);
void ipc_msg_printf(void);
void ipc_msg_printf_number(void);

#endif