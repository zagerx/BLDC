/**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		2023-08-17
** Version:
** Descriptions:	
**
********************************************************************************************************/
#include "fifo.h"

#define FIFO_LOCK 1
#define FIFO_UNLOCK 0
static char bytefifo_isfull(byte_fifo_t *pfifo)
{
	if((pfifo->head+1)%pfifo->buflen == pfifo->tail)
	{
		return 0;
	}
	return 1;
}
static char bytefifo_isempty(byte_fifo_t *pfifo)
{
	if(pfifo->head == pfifo->tail)
	{
		return 0;
	}
	return 1;
}
static char bytefifo_writebyte(byte_fifo_t *pfifo,byte_t data)
{
    if(bytefifo_isfull(pfifo) == 0)
    {
        return 1;
    }
    pfifo->pbuf[pfifo->head] = data;
    pfifo->head = (pfifo->head + 1) % pfifo->buflen ;
	pfifo->remain_byte--;
    return 0;
}
static char bytefifo_readbyte(byte_fifo_t *pfifo,byte_t *pdata)
{
    if (bytefifo_isempty(pfifo) == 0)
    {
        return 1;
    }
	*pdata = pfifo->pbuf[pfifo->tail];
    pfifo->tail = (pfifo->tail + 1)%pfifo->buflen ;
	pfifo->remain_byte++;
	return 0;
}

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

	if(len > (pfifo->buflen  - bytefifo_usendbyte(pfifo)-1))
	{
        pfifo->lockstate = FIFO_UNLOCK;
		return 1;
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
		return 1;
	}
	while(len--)
	{
		statues = bytefifo_readbyte(pfifo,&temp);
		*buf++ = temp;
	}
    pfifo->lockstate = FIFO_UNLOCK;
	return statues;
}
