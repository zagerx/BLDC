#ifndef __MSG_QUEUE__H
#define __MSG_QUEUE__H

#include "stdint.h"
typedef struct 
{ 
    int16_t id;
    int16_t msg_status;
    void (*pexcue)(void);
}msg_t;


#endif
