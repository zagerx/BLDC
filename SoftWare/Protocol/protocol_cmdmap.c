#include "protocol_cmdmap.h"
#include "debuglog.h"

#undef NULL
#define NULL (0)
#define TEST_CMD0 0X0101
#define TEST_CMD1 0X0101

typedef struct _cmdmap
{
    unsigned short cmd;
    void (*pfunc)(unsigned char *pdata, unsigned short datalen);
} cmdmap_t;
static void test_func(unsigned char *pdata, unsigned short len);
static cmdmap_t commend_map[] = {
    {TEST_CMD0, test_func},
    {TEST_CMD1, test_func},
};
__attribute__((weak)) void _forch_cmdmap(unsigned short cmd, unsigned char *pdata, unsigned short len)
{
    for (unsigned short i = 0; i < sizeof(commend_map) / sizeof(cmdmap_t); i++)
    {
        if (cmd != commend_map[i].cmd)
        {
            continue;
        }

        if (commend_map[i].pfunc != NULL)
        {
            commend_map[i].pfunc(pdata, len);
        }
    }
}
static void test_func(unsigned char *pdata, unsigned short len)
{
    for (unsigned short i = 0; i < len; i++)
    {
        USER_DEBUG_NORMAL("0x%x ", pdata[i]);
    }
    USER_DEBUG_NORMAL("\n");
}
