#include "frame.h"
void _unpack_proframe(unsigned char *pdata, unsigned short len, frame_t *frame)
{
    if (len < 8)
    {
        return;
    }
    frame->head = (pdata[0] << 8) | pdata[1];
    frame->cmd = (pdata[2] << 8) | pdata[3];
    frame->datalen = (pdata[4] << 8) | pdata[5];
    if (len < 8 + frame->datalen + 2)
    {
        return;
    }
    frame->pdata = pdata + 6;
    frame->crc = (pdata[6 + frame->datalen] << 8) | pdata[7 + frame->datalen];
    frame->tail = (pdata[8 + frame->datalen] << 8) | pdata[9 + frame->datalen];
}
