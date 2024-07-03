#include "protocol_comment.h"
#include "protocol_cfg.h"

#undef  NULL
#define NULL 0   

#define RESEND_CNT  5
#define TIMEOUT     1000


__attribute__((weak)) void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
}


static char _send_proframe(pro_frame_t *msg,unsigned short len)
{
    unsigned char *pucmsg;
    unsigned char *pdata;
    unsigned short data_len;
    data_len = len-sizeof(pro_frame_t);
    unsigned char buf[2] = {0};
    pucmsg = heap_malloc(len);
    memcpy(pucmsg,msg,len);
    memcpy(buf,(unsigned char*)&pucmsg[FRAM_TAIL_OFFSET],2);
    pdata = heap_malloc(data_len);
    memcpy(pdata,(unsigned char*)&pucmsg[sizeof(pro_frame_t)],data_len);
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET],pdata,data_len);    
    heap_free(pdata);
    unsigned short crc_16;
    crc_16 = crc16_calc((unsigned char*)&pucmsg[2],0xFFFF,FRAM_PDATA_OFFSET+data_len-2);
    crc_16 = __SWP16(crc_16);
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len],(unsigned char*)&crc_16,sizeof(crc_16));   
    memcpy((unsigned char*)&pucmsg[FRAM_PDATA_OFFSET+data_len+2],buf,2); 
    _bsp_protransmit(pucmsg,sizeof(pro_frame_t)-sizeof(void *)+data_len);
    heap_free(pucmsg);
	return 0;
}

char protocol_nowtransmit(unsigned char cmd_type,unsigned char cmd,\
                            void *pdata,unsigned short data_len)
{
    pro_frame_t* p_fram;
    p_fram = _packet_proframe(cmd | (cmd_type<<8),pdata,data_len);
    _send_proframe(p_fram,sizeof(pro_frame_t)+data_len);
    heap_free(p_fram);
    return NULL;
}



fsm_rt_t _trancemit_statemach(fsm_cb_t *ptThis)
{
    enum{
        SEND_CMD = USER,
        WAIT_ACK,
        RECIVE_ACK,
        IDLE,
    };
    pro_pack_t *pmsg = (pro_pack_t*)ptThis;
    pro_frame_t *pframe;
    unsigned short cmd ;    
    unsigned char cmd_fun;  
    unsigned char cmd_type;
    unsigned short data_len;
    unsigned short fsm_cycle = 0;
    fsm_cycle = pmsg->statemach_cb.cycle;
    pframe = pmsg->frame;
    cmd  = __SWP16(pframe->func_c);
    cmd_fun  = (unsigned char)cmd;
    cmd_type  = (unsigned char)(cmd>>8); 
    data_len = __SWP16(pframe->len);

    unsigned char event = 0;
    switch (ptThis->chState)
    {
    case START:
        ptThis->chState = SEND_CMD;
    case SEND_CMD:
        USER_DEBUG_NORMAL("send one cmd = 0x%x\r\n",cmd_fun);
        ptThis->chState = WAIT_ACK;        
    case WAIT_ACK:
        if (pmsg->recnt >= RESEND_CNT)
        {
            pmsg->recnt = 0;
            USER_DEBUG_NORMAL("cmd=0x%x no ack\r\n",cmd_fun);
            ptThis->chState = EXIT;
            break;
        }
        event =search_msgmap_event(cmd_fun);
        {
            /* code */
            if (pmsg->timeout++ >= TIMEOUT/fsm_cycle)
            {
                /* code */
                USER_DEBUG_NORMAL("cmd=0x%x timeout\r\n",cmd_fun);
                pmsg->timeout = 0;
                pmsg->recnt++;
                ptThis->chState = SEND_CMD;
            }
            break;
        }
        ptThis->chState = RECIVE_ACK;
    case RECIVE_ACK:
        USER_DEBUG_NORMAL("recive cmd(0x%x) ack\r\n",cmd_fun);
        ptThis->chState = EXIT;        
    case EXIT:
        pmsg->timeout = 0;
        pmsg->recnt = 0;
        return fsm_rt_cpl;
        break;
    default:
        break;
    }
    return fsm_rt_on_going;   
}
void protocol_transmit(unsigned char cmd_type,unsigned char cmd,\
                            void *pdata,unsigned short data_len)
{
    pro_frame_t* p_fram;
    p_fram = _packet_proframe(cmd | (cmd_type<<8),pdata,data_len);

    pro_pack_t *pmsg;
    pmsg = _packet_propack(p_fram,1000,5);
    _node_t *pnode;
    pnode = list_creatnode(pmsg,sizeof(node_item_t));
    list_insert_node(gtransmit_list,pnode);
}

void protocol_transmitprocess(void)
{
    _node_t *cur_node;
    node_item_t *cur_item;
    pro_pack_t *pack;
    pro_frame_t* p_fram;     
    cur_node = gtransmit_list->head;
    while (cur_node != 0)
    {
        cur_item = cur_node->pitem;
        pack = (pro_pack_t *)cur_item->pdata;
        p_fram = (pro_frame_t *)pack->frame;
        if(DISPATCH_FSM(((fsm_cb_t *)pack)) == fsm_rt_cpl)
        {
            list_delete_node(gtransmit_list,cur_node);
            USER_DEBUG_NORMAL("free \r\n");
        }
        cur_node = cur_node->next; 
    }
}


void protocol02_transmit(unsigned char *pdata,unsigned short datalen)
{
    unsigned char *pdata1;
    unsigned short len = 0;
    unsigned char databuf[128];
    len = datalen + 6;
    pdata1 = databuf;
    pdata1[0] = 0xAA;
    pdata1[1] = 0xFF;
    pdata1[2] = 0xF1;
    pdata1[3] = datalen;
    memcpy(&(pdata1[4]),pdata,datalen);
    unsigned char sumcheck = 0;
    unsigned char sum_sumcheck = 0;
    for (unsigned char i = 0; i < len-2; i++)
    {
        /* code */
        sumcheck += pdata1[i];
        sum_sumcheck += sumcheck;
    }
    pdata1[len-2] = sumcheck;
    pdata1[len-1] = sum_sumcheck;
    _bsp_protransmit(pdata1,len);
}

#define SEND_NUMBER 8
void protocol02_process(void)
{
    int buf2[SEND_NUMBER];
    // buf2[0] = ipc_read_data(PUBLIC_DATA_IA)*1000;
    // buf2[1] = ipc_read_data(PUBLIC_DATA_IB)*1000;
    // buf2[2] = ipc_read_data(PUBLIC_DATA_IC)*1000;
    // buf2[3] = ipc_read_data(PUBLIC_DATA_IALPHA)*1000;
    // buf2[4] = ipc_read_data(PUBLIC_DATA_IBETA)*1000;
    // buf2[5] = ipc_read_data(PUBLIC_DATA_ID)*1000;
    // buf2[6] = ipc_read_data(PUBLIC_DATA_IQ)*1000;
    // buf2[7] = ipc_read_data(PUBLIC_DATA_TEMP0)*1000;
    protocol02_transmit((unsigned char*)buf2,sizeof(buf2));    
}
