#ifndef __METHOD__H
#define __METHOD__H
typedef struct lowfilter
{
    /* data */
    float freq;//�˲�Ƶ��
    float pre_val;

}lowfilter_t;


void lowfilter_init(lowfilter_t *pfilter,float freq);
float lowfilter_cale(lowfilter_t *pfilter,float val);
#endif


