#ifndef __PROTOCOL__H
#define __PROTOCOL__H
#include "fifo.h"
// #include "protocol_frame.h"
// #include "protocol_transmit.h"
#include "stdint.h"
typedef struct _protocol protocol_t;
 struct _protocol
{
    uint8_t recivebuf[128];
    byte_fifo_t recivefifo;
    void (*init)(byte_fifo_t *,byte_t *,unsigned short);
    void (*getdata)(uint8_t* ,uint16_t);
    void (*forch)(uint16_t,uint8_t*,uint16_t);
    void (*senddata)(uint8_t ,uint16_t);
};


#define PRO_FIFO_SIZE           (256U)
#define PRO_FRAME_HEAD          (0xA5A5)
#define PRO_FRAME_TAIL          (0x5A5A)
#define PRO_FRAME_MAX_SIZE      (PRO_FIFO_SIZE)//todo

void protocl_init(protocol_t *this);
void protocol_getdata(protocol_t *this,uint8_t* pdata,uint16_t len);
// extern void protocol_process(void);
// extern void protocol_getdata_tofifo(unsigned char *pdata, unsigned short len);

#endif
