#ifndef __PROTOCOL__H
#define __PROTOCOL__H

#include "protocol_frame.h"
#include "protocol_transmit.h"


#define PRO_FIFO_SIZE           (256U)
#define PRO_FRAME_HEAD          (0xA5A5)
#define PRO_FRAME_TAIL          (0x5A5A)
#define PRO_FRAME_MAX_SIZE      (PRO_FIFO_SIZE)//todo

extern void protocol_init(void);
extern void protocol_process(void);
extern void protocol_getdata_tofifo(unsigned char *pdata, unsigned short len);

#endif
