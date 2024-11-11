#include "protocol.h"

static protocol_t mc_protocl = {
    .init = bytefifo_init,
};

void uart_readdata_callback(uint8_t* pdata,uint16_t len)
{
    protocol_getdata(&mc_protocl,pdata,len);
}

void main_1ms(void)
{
    protocl_process(&mc_protocl);
    
}
