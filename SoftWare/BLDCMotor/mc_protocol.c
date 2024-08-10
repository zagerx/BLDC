#include "motorctrl_common.h"
#include "mc_protocol.h"
#include "board.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>  
#include <ctype.h> // for isspace()  
#include "heap.h"
#include "debuglog.h"
#include "protocol.h"
extern msg_list_t* msg_list;

typedef struct _cmdmap cmdmap_t;
struct _cmdmap
{
    unsigned short cmd;
    void (*pfunc)(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
};

static void test_func(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_motorstart(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_motorstop(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_setMotorNormalM(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_setpidparam(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_getpcbainfo(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static cmdmap_t commend_map[] = {
    {M_SET_SPEED,      test_func,              },
    {M_SET_START,      _cmd_motorstart,        },
    {M_SET_STOP,       _cmd_motorstop,         },
    {M_SET_NormalM,    _cmd_setMotorNormalM,   },
    {M_SET_DebugM,     test_func,              },
    {M_GET_MotorInfo,  test_func,              },
    {M_GET_PCBAInfo,   _cmd_getpcbainfo,       },
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
            commend_map[i].pfunc((cmdmap_t *)&(commend_map[i]),pdata, len);
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

static void _cmd_getpcbainfo(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{

    unsigned char buf[] = {0x02,0x03};
    frame_t frame;
    frame.cmd = ~(pactor->cmd);
    frame.pdata = buf;
    frame.datalen = sizeof(buf);
    /*封包*/
    unsigned char *p = 0;
    p = _pack_proframe(&frame);

    // for (uint16_t i = 0; i < (frame.datalen + sizeof(frame_t) - 4); i++)
    // {
    //     USER_DEBUG_NORMAL("0x%02x ",p[i]);
    // }
    // USER_DEBUG_NORMAL("\n");
    
    msg_node_t* msg1 = (msg_node_t*)heap_malloc(sizeof(msg_node_t));
    if (msg1 == NULL)
    {
        USER_DEBUG_NORMAL("malloc fail\n");
        return;
    }
    
    msg1->fsm_cblock.time_count = 20;
    msg1->fsm_cblock.time_out = 0;
    msg1->fsm_cblock._state = 0;
    msg1->pdata = p;
    insert_msg_list_tail(msg_list, msg1);
}


