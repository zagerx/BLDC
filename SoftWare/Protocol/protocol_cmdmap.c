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
typedef struct _cmdmap cmdmap_t;
struct _cmdmap
{
    unsigned short cmd;
    void (*pfunc)(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
    void *pdata;
};
static void test_func(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_motorstart(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_motorstop(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_setMotorNormalModel(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_setMotorSpeedModel(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_setMotorEncoderModel(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_setparam(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_getinfo(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static cmdmap_t commend_map[] = {
    {M_SET_SPEED,              _cmd_setparam,               },
    {M_SET_POS,                _cmd_setparam,               },    
    {M_SET_START,              _cmd_motorstart,             },
    {M_SET_STOP,               _cmd_motorstop,              },
    {M_SET_NormalM,            _cmd_setMotorNormalModel,    },
    {M_SET_SpeedM,             _cmd_setMotorSpeedModel,     },
    {M_SET_EncoderLoopM,       _cmd_setMotorEncoderModel,   },
    {M_GET_MotorInfo,          test_func,                   },
    {M_GET_CtrlParaseInfo,     _cmd_getinfo,                },
    {M_SET_PIDParam,           _cmd_setparam,               },
    {M_SET_PIDTarge,           _cmd_setparam,               },    
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
    motor_get_motorstart(pactor->pdata,0,0);
}

static void _cmd_motorstop(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("Motor Stop CMD\n");
    motor_get_motorstart(pactor->pdata,0,0);
}

static void _cmd_setMotorNormalModel(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    motor_get_normolmode(pactor->pdata,0,0);
}
static void _cmd_setMotorSpeedModel(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    motor_get_speedmode(pactor->pdata,0,0);
}

static void _cmd_setMotorEncoderModel(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    motor_get_encodermode(pactor->pdata,0,0);
}
static void _cmd_setparam(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    if(pactor->cmd == M_SET_PIDParam)
    {
        motot_get_reset(pactor->pdata,0,0);
    }else if(pactor->cmd == M_SET_PIDTarge){
        motot_get_pidtarge(pactor->pdata,pdata,datalen);
    }else if(pactor->cmd == M_SET_SPEED){
    }else if(pactor->cmd == M_SET_POS){
    }
}

static void _cmd_getinfo(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
}
