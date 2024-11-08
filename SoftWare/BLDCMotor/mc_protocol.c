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
#include "flash.h"
#include "motorctrl_cfg.h"
extern msg_list_t* msg_list;
extern motor_t motor1;
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
static void _cmd_setMotorModel(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);

static void _cmd_setparam(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static void _cmd_getinfo(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen);
static cmdmap_t commend_map[] = {
    {M_SET_SPEED,      _cmd_setparam,              },
    {M_SET_POS,      _cmd_setparam,              },    
    {M_SET_START,      _cmd_motorstart,        },
    {M_SET_STOP,       _cmd_motorstop,         },
    {M_SET_NormalM,    _cmd_setMotorModel,   },
    {M_SET_SpeedM,     _cmd_setMotorModel,   },
    {M_SET_PosM,       _cmd_setMotorModel,   },
    {M_GET_MotorInfo,  test_func,              },
    {M_GET_CtrlParaseInfo,   _cmd_getinfo,       },
    {M_SET_PIDParam,   _cmd_setparam,       },
    {M_SET_PIDTarge,   _cmd_setparam,       },    
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
    motor1.curmode = pactor->cmd;
}

static void _cmd_motorstop(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("Motor Stop CMD\n");
    motor1.curmode = pactor->cmd;
}
static void _cmd_setMotorNormalM(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    motor1.curmode = pactor->cmd;
    USER_DEBUG_NORMAL("Set Motor enter Normal Model CMD\n");
}
static void _cmd_setMotorNormalD(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    motor1.curmode = pactor->cmd;
    USER_DEBUG_NORMAL("Set Motor enter Debug Model CMD\n");    
}
static void _cmd_setMotorModel(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    motor1.curmode = pactor->cmd;
}

extern void mc_tarpos_update(float delte_dist);

static void _cmd_setparam(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    if(pactor->cmd == M_SET_PIDParam)
    {
        /*写入FLASH指定位置 TODO*/
        user_flash_earse(PID_PARSE_ADDR,PID_PARSE_SIZE);

        // flash_t temp;
        float fbuf[4];
        convert_floats(pdata,datalen,fbuf);
        flash_t temp = {
            .name = "hello world,dev1_flash",
            .fbuf = {-1.28f,2.78f,1.0f,0.0f},
        };
        temp.fbuf[0] = fbuf[0];
        temp.fbuf[1] = fbuf[1];
        temp.fbuf[2] = fbuf[2];
        temp.fbuf[3] = fbuf[3];
        user_flash_write(PID_PARSE_ADDR,(uint8_t *)&temp,PID_PARSE_SIZE);
        /*复位系统 TODO*/
        motor1.reset_system();
    }else if(pactor->cmd == M_SET_PIDTarge){
        float temp;
        convert_floats(pdata,datalen,&(temp));   
        motor1.currment_handle.id_tar = temp;
        motor1.currment_handle.iq_tar = 0.0f;
        // motordebug.iq_targe = temp;
        USER_DEBUG_NORMAL("PID Targe update = %.02f\n",temp);
    }else if(pactor->cmd == M_SET_SPEED){
        float temp;
        convert_floats(pdata,datalen,&(temp));
        motor1.speed_handle.tar = temp;
    }else if(pactor->cmd == M_SET_POS){
        float temp;
        convert_floats(pdata,datalen,&(temp));
        USER_DEBUG_NORMAL("pos = %.2f\n",temp);
        mc_tarpos_update(temp);
    }
}

static void convert_floats(unsigned char *pdata, unsigned short datalen, float *floats) 
{  
    if (datalen % 4 != 0) {  
        // 如果datalen不是4的倍数，则不能完整地包含float数  
        USER_DEBUG_NORMAL("Error: Data length must be a multiple of 4.\n");  
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
static void _cmd_getinfo(cmdmap_t *pactor,unsigned char *pdata, unsigned short datalen)
{
    USER_DEBUG_NORMAL("D Axise PID:Kp=%.4f Ki=%.4f\n",motor1.currment_handle.d_pid.kp,motor1.currment_handle.d_pid.ki);
    USER_DEBUG_NORMAL("Q Axise PID:Kp=%.4f Ki=%.4f\n",motor1.currment_handle.q_pid.kp,motor1.currment_handle.q_pid.ki);
    USER_DEBUG_NORMAL("Speed   PID:Kp=%.4f Ki=%.4f\n",motor1.speed_handle.pid.kp,motor1.speed_handle.pid.ki);
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
extern void _bsp_protransmit(unsigned char* pdata,unsigned short len);

void mc_protocol_nowsend(unsigned short cmd,unsigned char* pdata,unsigned short datalen)
{
    frame_t frame;
    frame.cmd = cmd;
    frame.pdata = (unsigned char*)pdata;
    frame.datalen = datalen;
    unsigned char *p = 0;
    p = _pack_proframe(&frame); 
    uint16_t len = frame.datalen + sizeof(frame_t) - 4;
    _bsp_protransmit(p,len);
    // motor1.bsptransmit(p,len);//TODO
    heap_free(p);
}
