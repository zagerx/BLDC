#include "protocol_cfg.h"
#include "protocol_comment.h"
/*
该文件需完善
*/
static char pro_testfunc(cmdmsg_t *p,void *pdata,unsigned short datalen);
static char read_sensordata(cmdmsg_t *p,void *pdata,unsigned short datalen);

static const cmdmsg_t sgMsgTable[] = {
    [0] = {
        .cmd = PRO_FUNC_TESTCMD01,        
        .flagbit = CMD_EVENT_UNINVAIL,
        .fnHandler = pro_testfunc,        
    }, 
    [1] = {
        .cmd = PRO_FUNC_CMD03,        
        .flagbit = CMD_EVENT_01,
        .fnHandler = read_sensordata,
    },
    [2] = {
        .cmd = PRO_FUNC_CMD04,        
        .flagbit = CMD_EVENT_02,
        .fnHandler = read_sensordata,        
    },   
};

char pro_testfunc(cmdmsg_t *p,void *pdata,unsigned short datalen)
{
    float data;
    data = *(float *)pdata;
    ipc_write_data(PUBLIC_DATA_IQ_TARGET,data);
    IPC_SET_EVENT(gEventGroup,IPC_SET_IQ);
    return 1;
}

char read_sensordata(cmdmsg_t *p,void *pdata,unsigned short datalen)
{
    int flagbit;
    flagbit = p->flagbit;
    if (flagbit != CMD_EVENT_UNINVAIL)
    {
        IPC_SET_EVENT(g_protocol_event,flagbit);
    }
    return 0;
}

char search_msgmap(unsigned char cmd,
                   void *pData,
                   unsigned short hwSize)
{
    for (int n = 0; n < dimof(sgMsgTable); n++) {
        const cmdmsg_t *ptItem = &sgMsgTable[n];
        if (cmd != ptItem->cmd) {
            continue;
        }
        if (NULL == ptItem->fnHandler) {
            continue; 
        }       
        ptItem->fnHandler((cmdmsg_t *)ptItem, pData, hwSize); 
        return 1;
    }    
    return false;   //!< 没找到对应的消息
}

short search_msgmap_event(unsigned char cmd)
{
    const cmdmsg_t *ptItem ;
    for (int n = 0; n < dimof(sgMsgTable); n++) 
    {
        ptItem = &sgMsgTable[n];
        if (cmd != ptItem->cmd) {
            continue;
        }
        return ptItem->flagbit;
    }
    return 0;

}
