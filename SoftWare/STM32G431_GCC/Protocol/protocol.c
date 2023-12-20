/*-----------------------------模块提交说明--------------------------------------
优化了消息地图的命名
    1、复位之后，发送第一条测试指令，如果0B指令先响应，则有内存断言失败情况出现
        但不会死机。

1、测试协议重发流程
    1、使用串口助手发送
        5A A5 00 01 00 27 92 FE FE指令，触发数据重发功能
    2、观察串口助手的打印数据    
        可以观察到有两组命令同时在发送

        1、使用串口助手发送5A A5 02 0A 00 08 CD CC CC 40 33 33 13 40 44 71 FE FE/5A A5 00 0B 00 08 CD CC CC 40 33 33 13 40 9A D2 FE FE
            可观察到对应的命令停止发送

2、存在的问题点 1、申请内存失败存在
               2、状态机调度过程中会发生hardfalt
3、结论：
    该例程基本上可以实现消息的非阻塞式重发功能，但还需继续优化
*/
#include "protocol_comment.h"
#include "protocol_cfg.h"
#include "protocol.h"
/*-------------------------------FIFO组件------------------------------------------*/
static unsigned char fifo_receive_buff[PRO_FIFO_SIZE];//fifo数据缓存区
static byte_fifo_t protocol_fifo_handle;//fifo控制块

_list_t *gtransmit_list;
unsigned short g_protocol_event;
/*-------------------------------协议间同步事件-----------------------------*/

/*********************************************************************************************************
** Function name(函数名称):				protocol_reciverdata_tofifo()
**
** Descriptions（描述）:				接收一帧数据给FIFO组件
**
** input parameters（输入参数）:		len:一帧数据长度(包含帧头、帧尾)
** Returned value（返回值）:				0:成功
**
** Calling modules（调用模块）:			fifo.c/h
********************************************************************************************************/
unsigned char protocol_reciverdata_tofifo(unsigned char *pdata,unsigned short len)
{
	bytefifo_writemulitebyge(&protocol_fifo_handle,pdata,len);
	return 0;
}
char _get_framedata_fromfifo(unsigned char *pbuf,unsigned short *pframe_len)
{
    enum {
        START,
        FIRST ,
        SECOND,
        IDLE ,			
    };
    unsigned short index = 0;	
    unsigned char data;
    unsigned char chState = START;
           
    switch (chState)
    {
    case START:
        chState = FIRST;
    case FIRST:
        bytefifo_readmulintebyte(&protocol_fifo_handle,&data,1);
        if(data == (unsigned char)(PRO_FRAME_HEAD>>8))
        {
            pbuf[index++] = data;
            data = 0;
            chState = SECOND;
        }else{
            index = 0;
            chState = START;
            break;
        }
    case SECOND:
        bytefifo_readmulintebyte(&protocol_fifo_handle,&data,1);
        if(data == (unsigned char)(PRO_FRAME_HEAD))
        {
            chState = IDLE;
            pbuf[index++] = data;
        }else{
            chState = START;
            break;
        }
        case IDLE:
            do
            {
                if(index >= PRO_FRAME_MAX_SIZE)
                {
                    /* code */
                    *pframe_len = 0;
                    index = 0;
                    chState = START;//获取到完整的数据帧  复位状态机
                    return 1;
                }
                bytefifo_readmulintebyte(&protocol_fifo_handle,&data,1);
                pbuf[index++] = data;
                if(data == (unsigned char)(PRO_FRAME_TAIL>>8))
                {
                    data = 0;
                    bytefifo_readmulintebyte(&protocol_fifo_handle,&data,1);
                    pbuf[index++] = data;
                    if (data == (unsigned char)(PRO_FRAME_TAIL>>8))
                    {
                        /* code */
                        *pframe_len = index;
                        index = 0;
                        chState = START;//获取到完整的数据帧  复位状态机
                        return 0;
                    }
                }
            } while (1);
            break;
    default:
        break;
    }
    return 1;
}


/*----------------------------封包协议包----------------------------------------------------------*/
pro_pack_t* _packet_propack(pro_frame_t *frame,unsigned int timeout,unsigned char recnt)
{   
    fsm_cb_t fsmcb;
    pro_pack_t* p_propack = heap_malloc(sizeof(pro_pack_t));
    fsm_init(&fsmcb,1,(fsm_t *)_trancemit_statemach);
    p_propack->statemach_cb = fsmcb;
    p_propack->timeout = 0;
    p_propack->recnt = 0;
    p_propack->t0 = 0;
    p_propack->frame = frame;   
    return p_propack;
}

/*-----------------------------封包协议帧-------------------------------------------------------
**return       一段连续的内存
*/
pro_frame_t* _packet_proframe(unsigned short cmd,void *pdata,unsigned short len)
{
    unsigned char *puctemp;
    pro_frame_t *pfram;
    puctemp = heap_malloc(sizeof(pro_frame_t) + len);
    if(!puctemp)
    {
        heap_free(puctemp);
        return NULL;
    }
    pfram = (pro_frame_t *)puctemp;
    pfram->_x_x = 0x00;
    pfram->head = __SWP16(PRO_FRAME_HEAD);
    pfram->tail = __SWP16(PRO_FRAME_TAIL);
    pfram->func_c = __SWP16(cmd);
    pfram->len = __SWP16(len);
    pfram->pdata = &puctemp[sizeof(pro_frame_t)];
    if(pdata != 0)
    {
        memcpy((unsigned char*)&puctemp[sizeof(pro_frame_t)],(unsigned char*)pdata,len);
    }    
    return pfram;
}
/*-----------------------------解包协议帧-------------------------------------------------------
return 一段连续的内存
*/
pro_frame_t* _unpack_proframe(unsigned char *pdata,unsigned short len)
{
    /*申请内存*/
    unsigned char *pr_buf_1;
    unsigned short r_fram_len;
    unsigned short r_data_len;
    unsigned short r_crc_16;
    r_fram_len = len;
    pr_buf_1 = heap_malloc(r_fram_len);
    memcpy(pr_buf_1,pdata,r_fram_len);
    r_crc_16 = CRC16_Subsection((unsigned char *)pr_buf_1,0xFFFF,r_fram_len-4);
    if(r_crc_16 != (pr_buf_1[r_fram_len-4]<<8 | pr_buf_1[r_fram_len-3]))
    {
        USER_DEBUG_NORMAL("CRC16 Err 0x%2x\r\n",r_crc_16);
        heap_free(pr_buf_1);
        return NULL;
    }
    /*校验成功*/
    r_data_len =  pr_buf_1[4]<<8 | pr_buf_1[5];   //获取数据长度
    unsigned char *pr_buf_2;
    pr_buf_2 = 0;
    pr_buf_2 = heap_malloc(sizeof(pro_frame_t)+r_data_len);//申请数据帧内存
    memcpy(&pr_buf_2[2],pr_buf_1,r_fram_len);
    heap_free(pr_buf_1);

    unsigned char *pr_buf_data;
    pr_buf_data = heap_malloc(r_data_len);
    if (pr_buf_data==NULL && r_data_len!=0)
    {
        heap_free(pr_buf_2);
        heap_free(pr_buf_data);
        return NULL;
    }
    memcpy(pr_buf_data,&pr_buf_2[FRAM_PDATA_OFFSET],r_data_len);   
    memcpy(&pr_buf_2[FRAM_CRC_OFFSET],&pr_buf_2[FRAM_PDATA_OFFSET+r_data_len],4);
    memcpy(&pr_buf_2[FRAM_BUF_OFFSET],pr_buf_data,r_data_len);    
    heap_free(pr_buf_data);

    pro_frame_t *p_r_fram;
    p_r_fram = (pro_frame_t *)pr_buf_2;
    p_r_fram->pdata = &pr_buf_2[FRAM_BUF_OFFSET];

    return p_r_fram;
}

void protocol_init(void)
{
    bytefifo_init(&protocol_fifo_handle,fifo_receive_buff,sizeof(fifo_receive_buff));
    /*创建一个消息发送链表*/
    gtransmit_list = list_creat();
}

/*
process:线程
parse:解析
*/
void protocol_process(void)
{
    protocol_parse();
    protocol_transmitprocess();
}

