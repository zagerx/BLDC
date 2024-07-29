#include "motorctrl_common.h"
#include "mc_protocol.h"
#include "board.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>  
#include <ctype.h> // for isspace()  

#include "debuglog.h"
typedef struct _cmdmap
{
    unsigned short cmd;
    void *(*pfunc)(unsigned char *pdata, unsigned short datalen);
    char *motorstatus_str;
} cmdmap_t;

static void test_func(unsigned char *pdata, unsigned short datalen);
static void _cmd_motorstart(unsigned char *pdata, unsigned short datalen);
static void _cmd_motorstop(unsigned char *pdata, unsigned short datalen);
static void _cmd_setMotorNormalM(unsigned char *pdata, unsigned short datalen);
static void _cmd_setpidparam(unsigned char *pdata, unsigned short datalen);
static cmdmap_t commend_map[] = {
    {M_SET_SPEED,      test_func,            NULL},
    {M_SET_START,      _cmd_motorstart,      "motor_start"},
    {M_SET_STOP,       _cmd_motorstop,       "motor_stop"},
    {M_SET_NormalM,    _cmd_setMotorNormalM, "motor_normalmode"},
    {M_SET_DebugM,     test_func,            NULL},
    {M_GET_MotorInfo,  test_func,            NULL},
    {M_GET_PCBAInfo,   test_func,            NULL},
    {M_SET_PIDParam,   _cmd_setpidparam,     "pid_param"},
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
static void test_func(unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("mc_protocl.c \n");
}

static void _cmd_motorstart(unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("Motor Start CMD\n");
    motordebug.cur_cmd = "motor_start";
}

static void _cmd_motorstop(unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("Motor Stop CMD\n");
    motordebug.cur_cmd = "motor_stop";
}
static void _cmd_setMotorNormalM(unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("Set Motor enter Normal Model CMD\n");
    motordebug.cur_cmd = "motor_normalmode";
    
}
static void _cmd_setpidparam(unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("Set Motor PID Param CMD\n");
}


// 命令映射表  TODO
commandmap_t sg_commandmap[] = {  
    {"motor_stop",      4,      NULL,      "motor state stop"},
    {"motor_start",     5,   NULL,     "motor state runing"},
    {"motor_normalmode",     5,   NULL,     "motor state runing"},   
    {"motor_debugmode",     5,   NULL,     "motor state debug runing"},       
    {"pid_param",      6,   NULL,            "pid param set"},
    {"set_tar",      6,   NULL,            "pid param set"},
};


