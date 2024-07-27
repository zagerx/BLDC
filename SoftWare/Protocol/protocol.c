#include "protocol_comment.h"
#include "protocol_cfg.h"
#include "protocol.h"
#include "fifo.h"

static unsigned char pro_recivefifo_buf[PRO_FIFO_SIZE];
static byte_fifo_t pro_recivefifo;

static char _readdata_fromrecivefifo(unsigned char *pbuf, unsigned short *buf_size);
static void _recivethead(void);

extern void protocol_init(void)
{
    /*接收缓冲器初始化*/
    bytefifo_init(&pro_recivefifo, pro_recivefifo_buf, sizeof(pro_recivefifo_buf));
}
extern void protocol_process(void)
{
    _recivethead();
}
extern void protocol_getdata_tofifo(unsigned char *pdata, unsigned short len)
{
    bytefifo_writemulitebyge(&pro_recivefifo, pdata, len);
}

static void _recivethead(void)
{
    unsigned char pbuf[64] = {0};
    unsigned short datalen = 0;
    if (!_readdata_fromrecivefifo(pbuf, datalen))
    {
        return;
    }

    protocol_pause(pbuf, datalen); // todo
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
