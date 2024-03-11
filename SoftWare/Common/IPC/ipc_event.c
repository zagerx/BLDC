#include "ipc.h"
unsigned int gEventGroup = 0;
void ipc_set_event(EN_EVENT event,EN_EVENT_SIZE size,unsigned char val)
{
    if (size == EN_EVENT_ONEBIT)//表示当前事件占用1bit来表示
    {
        ((val) == 1 ? ((gEventGroup) | (1U << (event))) : ((gEventGroup) & ~(1U << (event))));
    }else{
        (gEventGroup & ~(((1U << size) - 1) << event)) | (((unsigned int)val & ((1U << size) - 1)) << event);
    }
}

