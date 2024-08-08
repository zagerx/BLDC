#include "frame.h"
void _unpack_proframe(unsigned char *pdata, unsigned short len, frame_t *frame)
{
    if (len < 8)
    {
        return;
    }
    frame->head = (pdata[0] << 8) | pdata[1];
    frame->cmd = (pdata[2] << 8) | pdata[3];
    frame->datalen = (pdata[4] << 8) | pdata[5];
    if (len < 8 + frame->datalen + 2)
    {
        return;
    }
    frame->pdata = pdata + 6;
    frame->crc = (pdata[6 + frame->datalen] << 8) | pdata[7 + frame->datalen];
    frame->tail = (pdata[8 + frame->datalen] << 8) | pdata[9 + frame->datalen];
}

#include <stdlib.h>  
#include <string.h>  
  
unsigned char* _pack_proframe(frame_t *frame)  
{  
    // 计算整个帧的总长度（不包括 pdata 指向的内存，因为我们需要单独处理它）  
    unsigned int total_length = 6 + 2 + 2; // 头部 + 命令 + 数据长度 + CRC + 尾部  
    if (frame->pdata != NULL) {  
        total_length += frame->datalen; // 加上数据的长度  
    }  
  
    // 分配足够的内存来存储整个帧  
    unsigned char *packed_data = (unsigned char *)malloc(total_length);  
    if (packed_data == NULL) {  
        // 内存分配失败，可以返回 NULL 或处理错误  
        return NULL;  
    }  
  
    // 填充头部  
    packed_data[0] = (frame->head >> 8) & 0xFF;  
    packed_data[1] = frame->head & 0xFF;  
  
    // 填充命令  
    packed_data[2] = (frame->cmd >> 8) & 0xFF;  
    packed_data[3] = frame->cmd & 0xFF;  
  
    // 填充数据长度  
    packed_data[4] = (frame->datalen >> 8) & 0xFF;  
    packed_data[5] = frame->datalen & 0xFF;  
  
    // 如果有数据，复制数据  
    if (frame->pdata != NULL) {  
        memcpy(packed_data + 6, frame->pdata, frame->datalen);  
    }  
  
    // 填充 CRC  
    packed_data[6 + frame->datalen] = (frame->crc >> 8) & 0xFF;  
    packed_data[7 + frame->datalen] = frame->crc & 0xFF;  
  
    // 填充尾部  
    packed_data[8 + frame->datalen] = (frame->tail >> 8) & 0xFF;  
    packed_data[9 + frame->datalen] = frame->tail & 0xFF;  
  
    return packed_data;  
}

