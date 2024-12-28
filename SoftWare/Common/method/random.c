#include "random.h"
#include "stdint.h"

/**
 * 输入一个随机数
 * 输出想要的随机数
 * 
 * 
 * 希望输出的随机数
 * 变化范围:-24444~24444之间
 * 数据间隔:至少是800
 */
int32_t random_cacle(random_t* random, int32_t seed)
{
    int32_t ret_value;
    int32_t cur_value = seed %  random->max_value;//限制取值范围
    int32_t diff = cur_value - random->last_value;
    if ((diff > 0?diff : -diff) < random->min_interval)//限制最小间隔
    {
        /* code */
        // diff>0?
        return ret_value;
    }
    
    ret_value = cur_value;
    return ret_value;
}

void random_init(random_t* random,int32_t max_value,int32_t min_interval)
{
    random->max_value = max_value;
    random->min_interval = min_interval;
}

void random_deinit(random_t* random)
{
    random->last_value = 0;
    random->max_value = 0;
    random->min_interval = 0;
}
