#ifndef __RANDOM__H
#define __RADDOM__H
#include "stdint.h"
typedef struct _random
{
    int32_t max_value;
    int32_t min_interval;
    int32_t last_value;
    int32_t test_vaule1;
}random_t;
int32_t random_cacle(random_t* random, int32_t seed);
void random_init(random_t* random,int32_t max_value,int32_t min_interval);
void random_deinit(random_t* random);


#endif
