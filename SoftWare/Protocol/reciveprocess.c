
#include "frame.h"

static frame_t* _unpack_proframe(unsigned char *pdata,unsigned short len);

void protocol_pause(unsigned char *pbuf,unsigned short buf_size)
{
    /*解包*/
    frame_t *temp;
    temp = _unpack_proframe(pbuf,buf_size);
    /*遍历地图*/
    _forch_cmdmap(temp->cmd,temp->pdata,temp->datalen);
}


frame_t* _unpack_proframe(unsigned char *pdata,unsigned short len)
{
    unsigned char *pr_buf_1;
    unsigned short r_fram_len;
    unsigned short r_data_len;
    unsigned short r_crc_16;
    r_fram_len = len;
    pr_buf_1 = heap_malloc(r_fram_len);
    memcpy(pr_buf_1,pdata,r_fram_len);
    r_crc_16 = crc16_calc((unsigned char *)pr_buf_1,0xFFFF,r_fram_len-4);
    if(r_crc_16 != (pr_buf_1[r_fram_len-4]<<8 | pr_buf_1[r_fram_len-3]))
    {
        USER_DEBUG_NORMAL("CRC16 Err 0x%2x\r\n",r_crc_16);
        heap_free(pr_buf_1);
        return NULL;
    }
    r_data_len =  pr_buf_1[4]<<8 | pr_buf_1[5];
    unsigned char *pr_buf_2;
    pr_buf_2 = 0;
    pr_buf_2 = heap_malloc(sizeof(frame_t)+r_data_len);
    memcpy(&pr_buf_2[2],pr_buf_1,r_fram_len);
    heap_free(pr_buf_1);

    unsigned char *pr_buf_data;
    pr_buf_data = heap_malloc(r_data_len);
    if (pr_buf_data==NULL && r_data_len!=0)
    {
        heap_free(pr_buf_2);
        heap_free(pr_buf_data);
        return NULL;
    }
    memcpy(pr_buf_data,&pr_buf_2[FRAM_PDATA_OFFSET],r_data_len);   
    memcpy(&pr_buf_2[FRAM_CRC_OFFSET],&pr_buf_2[FRAM_PDATA_OFFSET+r_data_len],4);
    memcpy(&pr_buf_2[FRAM_BUF_OFFSET],pr_buf_data,r_data_len);    
    heap_free(pr_buf_data);

    frame_t *p_r_fram;
    p_r_fram = (frame_t *)pr_buf_2;
    p_r_fram->pdata = &pr_buf_2[FRAM_BUF_OFFSET];

    return p_r_fram;
}


