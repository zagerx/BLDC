#ifndef __PROTOCOL_CMDMAP__H
#define __PROTOCOL_CMDMAP__H

typedef struct _cmdmap
{
    unsigned short cmd;
    void *(*pfunc)(unsigned char *pdata, unsigned short datalen);
} cmdmap_t;

void _forch_cmdmap(unsigned short cmd, unsigned char *pdata, unsigned short len);
#endif
