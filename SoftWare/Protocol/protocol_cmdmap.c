#include "protocol_cmdmap.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>  
#include <ctype.h>
#include "heap.h"
#include "debuglog.h"
#include "protocol.h"
#include "math.h"
#include "datatypecovert.h"
static cmdmap_t commend_map[255];
void _forch_cmdmap(unsigned short cmd, unsigned char *pdata, unsigned short len)
{
    for (unsigned short i = 0; i < 255; i++)
    {
        if (cmd != commend_map[i].cmd)
        {
            continue;
        }

        if (commend_map[i].pfunc != NULL)
        {
            commend_map[i].pfunc((commend_map[i].pdata),pdata, len);
        }
    }
}

void protocol_cmd_register(unsigned short cmd,void *func,void *pdata)
{
    static unsigned char count = 0;
    commend_map[count].cmd = cmd;
    commend_map[count].pfunc = func;
    commend_map[count].pdata = pdata;
    count++;
}


