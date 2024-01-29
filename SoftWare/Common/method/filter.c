#include "./filter.h"
#include "./method_cfg.h"
void lowfilter_init(lowfilter_t *pfilter,float freq)
{
    pfilter->freq = 1.0f/freq;
    pfilter->pre_val = 0.0f;
}

float lowfilter_cale(lowfilter_t *pfilter,float val)
{
    float curval = 0.0f;
    curval =   pfilter->freq * val  + (1.0f-pfilter->freq) * pfilter->pre_val ;
    pfilter->pre_val = curval;
    return curval;
}



