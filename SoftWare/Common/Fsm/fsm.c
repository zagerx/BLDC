#include "fsm.h"
#include "heap.h"

#undef NULL
#define NULL 0

void fsm_init(fsm_cb_t *pThis, unsigned short cycle,fsm_t *machine)
{
    pThis->fsm = (fsm_t *)machine;
    pThis->chState = ENTER;
    pThis->cycle = cycle;
    return;
}

fsm_cb_t *fsm_creat(fsm_t *initstat,unsigned short cycel)
{
    fsm_cb_t *pthis;
    pthis = (fsm_cb_t *)heap_malloc(sizeof(fsm_cb_t));
    if(pthis==NULL)
    {
        return NULL;
    }
    fsm_init(pthis,cycel,initstat);
      
    return pthis;
}

char fsm_destructor(fsm_cb_t *pthis)
{
    if (pthis!=NULL)
    {
        heap_free(pthis);
    }
    return 0;
}


