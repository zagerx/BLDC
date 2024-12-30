#include "random.h"
#include "stdint.h"
#define NULL 0 
/**
 * 输入一个随机数
 * 输出想要的随机数
 * 
 * 
 * 希望输出的随机数
 * 变化范围:-24444~24444之间
 * 数据间隔:至少是800
 */
 int32_t test_cur_value = 0;
 int32_t test_diff_value = 0;
 int32_t test_ret_value = 0;

/*==========================================================================================
 * @brief        计算随机数
 * @FuncName     
 * @param        random 
 * @param        seed 
 * @return       int32_t 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
int32_t random_cacle(random_t* random, int32_t seed)
{
    if (random == NULL) return -1; // 检查指针是否为空

    int32_t ret_value;
    int32_t min_value = -random->max_value; // 最小值等于负的最大值

    // 限制取值范围在 [min_value, max_value] 之间
    ret_value = seed % random->max_value;//(random->max_value - min_value + 1) + min_value;
    random->test_vaule1 = ret_value;
    // 检查随机数是否满足最小间隔要求
    int32_t diff = ret_value - random->last_value;
    if ((diff > 0 ? diff : -diff) < random->min_interval)
    {
        // 如果不满足，调整随机数直到满足条件
        do
        {
            seed = (seed * 1103515245 + 12345) % ((uint32_t)0xFFFFFFFF); // 简单的线性同余生成器
            ret_value = seed % (random->max_value - min_value + 1) + min_value;
            diff = ret_value - random->last_value;
        } while ((diff > 0 ? diff : -diff) < random->min_interval);
    }

    random->last_value = ret_value; // 更新上一次的值
    return ret_value;
}

/*==========================================================================================
 * @brief        初始化
 * @FuncName     
 * @param        random 
 * @param        max_value 
 * @param        min_interval 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void random_init(random_t* random,int32_t max_value,int32_t min_interval)
{
    random->last_value = 0;
    random->max_value = max_value;
    random->min_interval = min_interval;
}

void random_deinit(random_t* random)
{
    random->last_value = 0;
    random->max_value = 0;
    random->min_interval = 0;
}
