#ifndef __METHOD__H
#define __METHOD__H
/*----------------------?????----------------------------------*/
typedef struct lowfilter
{
    /* data */
    float freq;//
    float pre_val;

}lowfilter_t;
extern void lowfilter_init(lowfilter_t *pfilter,float freq);
extern float lowfilter_cale(lowfilter_t *pfilter,float val);

/*----------------------PID???---------------------------------*/
#include "pid.h"


#endif


