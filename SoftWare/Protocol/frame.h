#ifndef __FRAME__H
#define __FRAME__H

typedef struct _frame
{
    unsigned short head;
    unsigned short cmd;
    unsigned short datalen;
    unsigned char  *pdata;
    unsigned short crc;
    unsigned short tail;
}frame_t;

#endif
