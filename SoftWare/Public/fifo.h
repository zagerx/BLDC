#ifndef _FIFO_H
#define _FIFO_H
typedef unsigned char byte_t;
typedef struct byte_fifo{
    unsigned short head;
    unsigned short tail;
    volatile short remain_byte;
    byte_t *pbuf;
    volatile unsigned short buflen;
    volatile unsigned char lockstate;
}byte_fifo_t;
extern void bytefifo_init(byte_fifo_t *pfifo,byte_t *arry,unsigned short len);
extern char bytefifo_writemulitebyge(byte_fifo_t *pfifo,byte_t *buf,unsigned short len);
extern char bytefifo_readmulintebyte(byte_fifo_t *pfifo,byte_t *buf,unsigned short len);
#endif
