#ifndef __FRAME__H
#define __FRAME__H

#pragma pack(push,1)
typedef struct _frame
{
    unsigned short head;
    unsigned short cmd;
    unsigned short datalen;
    unsigned char  *pdata;
    unsigned short crc;
    unsigned short tail;
}frame_t;
#pragma pack(pop)

void _unpack_proframe(unsigned char *pdata, unsigned short len, frame_t *frame);
unsigned char* _pack_proframe(frame_t *frame);

#endif
