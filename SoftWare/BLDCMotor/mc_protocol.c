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
#include "math.h"

extern msg_list_t* msg_list;

typedef struct _cmdmap cmdmap_t;
struct _cmdmap
{
    unsigned short cmd;
    void (*pfunc)(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
};
typedef union {  
    float f;  
    uint32_t i;  
} float_int_union; 
static void convert_floats(unsigned char *pdata, unsigned short datalen, float *floats); 
static void test_func(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_motorstart(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_motorstop(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_setMotorNormalM(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_setMotorNormalD(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);

static void _cmd_setpidparam(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_getpcbainfo(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static cmdmap_t commend_map[] = {
    {M_SET_SPEED,      test_func,              },
    {M_SET_START,      _cmd_motorstart,        },
    {M_SET_STOP,       _cmd_motorstop,         },
    {M_SET_NormalM,    _cmd_setMotorNormalM,   },
    {M_SET_DebugM,     _cmd_setMotorNormalD,   },
    {M_GET_MotorInfo,  test_func,              },
    {M_GET_PCBAInfo,   _cmd_getpcbainfo,       },
    {M_SET_PIDParam,   _cmd_setpidparam,       },
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
static void _cmd_setMotorNormalD(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    motordebug.rec_cmd = pactor->cmd;
    USER_DEBUG_NORMAL("Set Motor enter Debug Model CMD\n");    
}

static void _cmd_setpidparam(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    if(pactor->cmd == M_SET_PIDParam)
    {
        USER_DEBUG_NORMAL("Set Motor PID Param CMD\n");
        /*写入FLASH指定位置 TODO*/
        /*复位系统 TODO*/
        user_softresetsystem();
    }else if(pactor->cmd == M_SET_PIDTarge){
        // motordebug.id_targe = 
        motordebug.iq_targe = 0.0f;
    }
}

static void convert_floats(unsigned char *pdata, unsigned short datalen, float *floats) 
{  
    if (datalen % 4 != 0) {  
        // 如果datalen不是4的倍数，则不能完整地包含float数  
        printf("Error: Data length must be a multiple of 4.\n");  
        return;  
    }
    unsigned int num_floats = datalen / 4;  
    for (unsigned int i = 0; i < num_floats; i++) {  
        float_int_union u;  
        // 从pdata中读取4个字节（即一个float）  
        u.i = (uint32_t)pdata[i*4]  | (uint32_t)pdata[i*4+1] << 8 |  
              (uint32_t)pdata[i*4+2] << 16 | (uint32_t)pdata[i*4+3]<< 24;          
        floats[i] = u.f; // 转换为float并存储  
    }  
} 
static void _cmd_getpcbainfo(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    float fbuf[5];
    convert_floats(pdata,datalen,fbuf);
    //响应部分
    unsigned char buf[] = {0x02,0x03};
    float f_val[2] = {3.1415926f,2.2154f};
    frame_t frame;
    frame.cmd = ~(pactor->cmd);
    frame.pdata = (unsigned char*)&f_val;
    frame.datalen = sizeof(f_val);
    /*封包*/
    unsigned char *p = 0;
    p = _pack_proframe(&frame);
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
    msg1->datalen = frame.datalen + sizeof(frame_t) - 4;
    insert_msg_list_tail(msg_list, msg1);
}

/**  
 * @brief 发送周期性变化的电机速度控制命令到协议层（待优化，后续改为通用的发送函数）    
 *  
 * @param 无  
 * @return 无（通过修改全局或外部资源）  
 *  
 * @author [zager]  
 * @version 1.0  
 * @date [2024-08-19]  
 */  
void mc_protocol_sendspeed(void)
{
    float fspeed;
    frame_t frame;
    frame.cmd = S_MotorSpeed;
    static float theta = 0.0f;
    fspeed = 10.0f*sinf(theta);
    theta += 0.1f;
    frame.pdata = (unsigned char*)&fspeed;
    frame.datalen = sizeof(float);   
    unsigned char *p = 0;
    p = _pack_proframe(&frame);    
    msg_node_t* msg1 = (msg_node_t*)heap_malloc(sizeof(msg_node_t));
    if (msg1 == NULL)
    {
        USER_DEBUG_NORMAL("malloc fail\n");
        return;
    }
    msg1->fsm_cblock.time_count = 0;
    msg1->fsm_cblock.time_out = 20;
    msg1->fsm_cblock._state = 0;
    msg1->pdata = p;
    msg1->datalen = frame.datalen + sizeof(frame_t) - 4;
    insert_msg_list_tail(msg_list, msg1);
}

void mc_protocol_send(unsigned short cmd,unsigned char* pdata,unsigned short datalen,\
                      unsigned char time_count,unsigned short time_out)
{
    frame_t frame;
    frame.cmd = cmd;
    frame.pdata = (unsigned char*)pdata;
    frame.datalen = datalen;
    unsigned char *p = 0;
    p = _pack_proframe(&frame); 
    msg_node_t* msg = (msg_node_t*)heap_malloc(sizeof(msg_node_t));
    if (msg == NULL)
    {
        USER_DEBUG_NORMAL("msg_node_t malloc fail\n");
        return;
    }
    msg->fsm_cblock.time_count = time_count;
    msg->fsm_cblock.time_out = time_out;
    msg->fsm_cblock._state = 0;
    msg->pdata = p;
    msg->datalen = frame.datalen + sizeof(frame_t) - 4;
    insert_msg_list_tail(msg_list, msg);
}

