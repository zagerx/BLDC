#ifndef __PROTOCOL__H
#define __PROTOCOL__H

enum{
    CMD_ORIGE = 0x00,
    CMD_RESP,
    CMD_ACK,
};
#if 1
    typedef enum{
        PRO_FUNC_TESTCMD01 = 1,
        PRO_FUNC_CMD02,
        PRO_FUNC_CMD03,        
        PRO_FUNC_CMD04,
        CMD_NUMBER
    }E_CMD;
#else
    #define PRO_FUNC_CMD01	0x00	
    #define PRO_FUNC_CMD02	0x01	
    #define PRO_FUNC_CMD04  0x0B	
    #define PRO_FUNC_CMD03  0x0A
#endif

extern void protocol_init(void);
extern void protocol_process(void);
extern void _bsp_protransmit(unsigned char* pdata,unsigned short len);
extern unsigned char protocol_reciverdata_tofifo(unsigned char *pdata,unsigned short len);
extern char protocol_nowtransmit(unsigned char cmd_type,\
                        unsigned char cmd,void *pdata,unsigned short data_len);
extern void protocol_transmit(unsigned char cmd_type,unsigned char cmd,\
                            void *pdata,unsigned short data_len);      
void protocol_transmitprocess(void);

void protocol02_transmit(unsigned char *pdata,unsigned short datalen);
void protocol02_process(void);




/*
protocol.c
void protocol_process(void)
{
    //获取数据getdatafromfifo

    //解包  根据协议具体内容    "可以灵活定制"
    protocol01_unpack();
    
    //遍历协议/命令地图
}


//协议地图
protocol01.c
#include "protocol.h + tim.h..."

void protocol_init(void)
{


    protocol_registerCMD(CMD,CMD_Index,func);

}

灵活定制，采用注册方法


void excufunc(void)
{
    //设置PWM--->添加到消息地图
}
*/



*/






#endif
