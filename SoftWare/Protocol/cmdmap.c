#include "cmdmap.h"
#include "commend.h"

#include "debuglog.h"

#undef NULL
#define NULL (0)

static void test_func(unsigned char *pdata, unsigned short len);
static cmdmap_t commend_map[] = {
    {M_SET_SPEED, test_func},
    {M_SET_POS, test_func},
    {M_SET_STOP, test_func},
    {M_SET_NormalM, test_func},
    {M_SET_DebugM, test_func},
    {M_GET_MotorInfo, test_func},
    {M_GET_PCBAInfo, test_func},
};

void _forch_cmdmap(unsigned short cmd, unsigned char *pdata, unsigned short len)
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
