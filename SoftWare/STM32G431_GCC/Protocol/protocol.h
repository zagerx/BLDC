#ifndef __PROTOCOL__H
#define __PROTOCOL__H


/*--------------------------------------------------
���ļ�Ϊģ��Ľӿ��ļ�
--------------------------------------------------*/
enum{
    CMD_ORIGE = 0x00,
    CMD_RESP,
    CMD_ACK,
};
#if 1
    typedef enum{
        PRO_FUNC_TESTCMD01 = 1,
        PRO_FUNC_CMD01 ,
        PRO_FUNC_CMD02,        
        PRO_FUNC_CMD03= 0x0A,
        PRO_FUNC_CMD04,
        CMD_NUMBER
    }E_CMD;
#else
    #define PRO_FUNC_CMD01	0x00	//����������
    #define PRO_FUNC_CMD02	0x01	//�ӻ�������
    #define PRO_FUNC_CMD04	        0x0B	//�¶�����
    #define PRO_FUNC_CMD03	    0x0A	//�¶�����
#endif

/*---------------------------------Э��ӿ�------------------------------------------*/
extern void protocol_init(void);
extern void protocol_process(void);
extern void _bsp_protransmit(unsigned char* pdata,unsigned short len);
extern unsigned char protocol_reciverdata_tofifo(unsigned char *pdata,unsigned short len);
extern char protocol_nowtransmit(unsigned char cmd_type,\
                        unsigned char cmd,void *pdata,unsigned short data_len);
extern void protocol_transmit(unsigned char cmd_type,unsigned char cmd,\
                            void *pdata,unsigned short data_len);      
void protocol_transmitprocess(void);
/*--------------------------------------------------------------------------------------*/
#endif
