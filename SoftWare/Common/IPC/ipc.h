#ifndef __IPC__H
#define __IPC__H

#include "board.h"
#ifndef USE_IPC_ENABLE
    typedef enum{
        EN_EVENT_01 = 0,
    }EN_EVENT;
    typedef enum{
        EN_EVENT_ONEBIT = 1,
        EN_EVENT_TWOBIT,
        EN_EVENT_THREEBIT
    }EN_EVENT_SIZE;

typedef enum{
    PUBLIC_DATA_START_INDEX, 
    PUBLIC_DATA_STOP_INDEX,
}E_PUBLIC_DATA;
#endif

float ipc_read_data(E_PUBLIC_DATA index);
char ipc_write_data(E_PUBLIC_DATA index,float data);

#endif
