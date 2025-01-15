#include "protocol.h"
#include "protocol_cmdmap.h"
#include "fifo.h"
#include "initmodule.h"
#include "taskmodule.h"
#include "debuglog.h"
#include "protocol_cmdmap.h"
static unsigned char pro_recivefifo_buf[PRO_FIFO_SIZE];
static byte_fifo_t pro_recivefifo;

static char _readdata_fromrecivefifo(unsigned char *pbuf, unsigned short *buf_size);
static void _recivethread(void);

extern void protocol_init(void)
{
    USER_DEBUG_NORMAL("protocol_init\n");
    /*接收缓冲器初始化*/
    bytefifo_init(&pro_recivefifo, pro_recivefifo_buf, sizeof(pro_recivefifo_buf));
}
extern void protocol_process(void)
{
    _recivethread();
}
extern void protocol_getdata_tofifo(unsigned char *pdata, unsigned short len)
{
    // for (unsigned char i = 0; i < len; i++)
    // {
    //     USER_DEBUG_NORMAL("0x%x  ",pdata[i]);
    // }
    // USER_DEBUG_NORMAL("\n");
    bytefifo_writemulitebyge(&pro_recivefifo, pdata, len);
}

static void _recivethread(void)
{
    unsigned char pbuf[64] = {0};
    unsigned short len = 0;
    if (_readdata_fromrecivefifo(pbuf, &len))
    {
        return;
    }
    frame_t frame;
    _unpack_proframe(pbuf, len, &frame);
    // USER_DEBUG_NORMAL("head: %04X, cmd: %04X, datalen: %04X, crc: %04X, tail: %04X\n",
    //                   frame.head, frame.cmd, frame.datalen, frame.crc, frame.tail);
    _forch_cmdmap(frame.cmd, frame.pdata, frame.datalen);
}
static char _readdata_fromrecivefifo(unsigned char *pbuf, unsigned short *buf_size)
{
    enum
    {
        START,
        FIRST,
        SECOND,
        IDLE,
    };
    unsigned short index = 0;
    unsigned char data;
    unsigned char chState = START;

    switch (chState)
    {
    case START:
        chState = FIRST;
    case FIRST:
        bytefifo_readmulintebyte(&pro_recivefifo, &data, 1);
        if (data == (unsigned char)(PRO_FRAME_HEAD >> 8))
        {
            pbuf[index++] = data;
            data = 0;
            chState = SECOND;
        }
        else
        {
            index = 0;
            chState = START;
            break;
        }
    case SECOND:
        bytefifo_readmulintebyte(&pro_recivefifo, &data, 1);
        if (data == (unsigned char)(PRO_FRAME_HEAD))
        {
            chState = IDLE;
            pbuf[index++] = data;
        }
        else
        {
            chState = START;
            break;
        }
    case IDLE:
        do
        {
            if (index >= PRO_FRAME_MAX_SIZE)
            {
                *buf_size = 0;
                index = 0;
                chState = START;
                return 1;
            }
            bytefifo_readmulintebyte(&pro_recivefifo, &data, 1);
            pbuf[index++] = data;
            if (data == (unsigned char)(PRO_FRAME_TAIL >> 8))
            {
                data = 0;
                bytefifo_readmulintebyte(&pro_recivefifo, &data, 1);
                pbuf[index++] = data;
                if (data == (unsigned char)(PRO_FRAME_TAIL >> 8))
                {
                    *buf_size = index;
                    index = 0;
                    chState = START;
                    return 0;
                }
            }
        } while (1);
        break;
    default:
        break;
    }
    return 1;
}

board_init(protocol_init)
board_task(protocol_process)
