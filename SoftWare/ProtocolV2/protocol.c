#include "protocol.h"

void protocl_init(protocol_t *this)
{
    this->init((byte_fifo_t*)&(this->recivefifo),(byte_t*)&(this->recivebuf),128);
}

void protocol_getdata(protocol_t *this,uint8_t* pdata,uint16_t len)
{
    bytefifo_readmulintebyte(&(this->recivefifo),pdata,len);
}

void protocol_process(protocol_t *this)
{
    // this->forch()
}

