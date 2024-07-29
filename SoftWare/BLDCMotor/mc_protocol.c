#include "motorctrl_common.h"
#include "mc_protocol.h"
#include "board.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>  
#include <ctype.h> // for isspace()  

#include "debuglog.h"

typedef struct _cmdmap cmdmap_t;
struct _cmdmap
{
    unsigned short cmd;
    void *(*pfunc)(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
};

static void test_func(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_motorstart(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_motorstop(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_setMotorNormalM(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_setpidparam(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static cmdmap_t commend_map[] = {
    {M_SET_SPEED,      test_func,              },
    {M_SET_START,      _cmd_motorstart,        },
    {M_SET_STOP,       _cmd_motorstop,         },
    {M_SET_NormalM,    _cmd_setMotorNormalM,   },
    {M_SET_DebugM,     test_func,              },
    {M_GET_MotorInfo,  test_func,              },
    {M_GET_PCBAInfo,   test_func,              },
    {M_SET_PIDParam,   _cmd_setpidparam,       },
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
            commend_map[i].pfunc(&(commend_map[i]),pdata, len);
        }
    }
}
static void test_func(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("mc_protocl.c/test_func()\n");
}

static void _cmd_motorstart(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("Motor Start CMD\n");
    motordebug.rec_cmd = pactor->cmd;
}

static void _cmd_motorstop(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("Motor Stop CMD\n");
    motordebug.rec_cmd = pactor->cmd;
}
static void _cmd_setMotorNormalM(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    motordebug.rec_cmd = pactor->cmd;
    USER_DEBUG_NORMAL("Set Motor enter Normal Model CMD\n");
}
static void _cmd_setpidparam(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("Set Motor PID Param CMD\n");
}


