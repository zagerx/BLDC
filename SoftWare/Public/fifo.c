/**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		2023-08-17
** Version:
** Descriptions:	先进先出队列
**
********************************************************************************************************/
#include "fifo.h"


#define FIFO_LOCK 1
#define FIFO_UNLOCK 0
static char bytefifo_isfull(byte_fifo_t *pfifo)
{
	if((pfifo->head+1)%pfifo->buflen == pfifo->tail)
	{
		return 0;//已满
	}
	return 1;
}
/*FIFO空，不能读数据*/
static char bytefifo_isempty(byte_fifo_t *pfifo)
{
	if(pfifo->head == pfifo->tail)
	{
		return 0;//空
	}
	return 1;
}
/*FIFO满不能写数据*/
static char bytefifo_writebyte(byte_fifo_t *pfifo,byte_t data)
{
    if(bytefifo_isfull(pfifo) == 0)
    {
        return 1;//满
    }
    pfifo->pbuf[pfifo->head] = data;
    pfifo->head = (pfifo->head + 1) % pfifo->buflen ;
	pfifo->remain_byte--;
    return 0;//写入成功
}
static char bytefifo_readbyte(byte_fifo_t *pfifo,byte_t *pdata)
{/*FIFO为空 不能读*/
    if (bytefifo_isempty(pfifo) == 0)
    {
        return 1;//无法读取
    }
	*pdata = pfifo->pbuf[pfifo->tail];
    pfifo->tail = (pfifo->tail + 1)%pfifo->buflen ;
	pfifo->remain_byte++;
	return 0;
}

/*获取FIFO已使用的字节数*/
static unsigned short bytefifo_usendbyte(byte_fifo_t *pfifo)
{
	if(pfifo->tail <= pfifo->head)
	{
		return (pfifo->head-pfifo->tail);		
	}else{
		return (pfifo->buflen  + pfifo->head - pfifo->tail);
	}
}
void bytefifo_init(byte_fifo_t *pfifo,byte_t *arry,unsigned short len)
{
    pfifo->head = 0;
    pfifo->tail = 0;
    pfifo->remain_byte = len;
    pfifo->pbuf = arry;  
    pfifo->buflen = len;
    pfifo->lockstate = FIFO_UNLOCK;
}
char bytefifo_writemulitebyge(byte_fifo_t *pfifo,byte_t *buf,unsigned short len)
{
	byte_t *pw;
	pw = buf;
    if (pfifo->lockstate)
    {
        return 1;
    }
    pfifo->lockstate = FIFO_LOCK;

	if(len > (pfifo->buflen  - bytefifo_usendbyte(pfifo)-1))//-1，因为有一个字节始终没有用到
	{
        pfifo->lockstate = FIFO_UNLOCK;
		return 1;//写入失败
	}
	while(len--)
	{
		bytefifo_writebyte(pfifo,*pw);
		pw++;
	}
    pfifo->lockstate = FIFO_UNLOCK;
	return 0;
}
char bytefifo_readmulintebyte(byte_fifo_t *pfifo,byte_t *buf,unsigned short len)
{
	byte_t temp = 0;
	char statues = 1;
    if (pfifo->lockstate)
    {
        return 1;
    }
    pfifo->lockstate = FIFO_LOCK;
	if(len > bytefifo_usendbyte(pfifo))//
	{
        pfifo->lockstate = FIFO_UNLOCK;
		return 1;//读取失败
	}
	while(len--)
	{
		statues = bytefifo_readbyte(pfifo,&temp);
		*buf++ = temp;
	}
    pfifo->lockstate = FIFO_UNLOCK;
	return statues;
}