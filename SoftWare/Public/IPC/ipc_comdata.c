/*-------------------------公共数据区-----------------------------------
该文件根据具体的项目实现，不具有普遍性
---------------------------------------------------------------------*/
#include "ipc.h"
typedef enum{
    PUBLIC_DATA_START_INDEX,
    PUBLIC_DATA_STOP_INDEX,
}E_PUBLIC_DATA;

static float gIpc_datazone[PUBLIC_DATA_STOP_INDEX] = {
    0.0f,
};

float ipc_read_data(E_PUBLIC_DATA index)
{
    return gIpc_datazone[index];
}

char ipc_write_data(E_PUBLIC_DATA index,float data)
{
    if ((E_PUBLIC_DATA)index > PUBLIC_DATA_STOP_INDEX)
    {
        return 1;
    }
    gIpc_datazone[index] = data;
}

