#include "ntc.h"

#include <stdint.h>  
#include <stdbool.h>  
  
#define ARRY_SIZE  5  
  
typedef struct ntc_point {  
    uint16_t ADC;  
    float tem;  
} ntc_point_t;  
  
typedef struct ntc_table {  
    ntc_point_t points[ARRY_SIZE];  
} ntc_table_t;  
  
// 假设我们有一个全局的查找表，并且已经按ADC值排序  
ntc_table_t ntc_lookup_table = {  
    {{4096, 80.0f}, {3072, 60.0f}, {2048, 25.0f}, {1024, 0.0f}, {0, -20.0f}}  
};  

// 二分查找函数  
int binary_search(uint16_t adc, ntc_point_t *points, int size) {  
    int left = 0;  
    int right = size - 1;  
    int mid;  
  
    while (left <= right) {  
        mid = left + (right - left) / 2;  
        if (adc < points[mid].ADC) {  
            right = mid - 1;  
        } else if (adc > points[mid].ADC) {  
            left = mid + 1;  
        } else {  
            // 找到完全匹配的ADC值，直接返回索引  
            return mid;  
        }  
    }  
  
    // 没有找到完全匹配的ADC值，返回左侧相邻点的索引（或-1，如果adc小于所有点）  
    if (left == size) {  
        return -1; // 表示adc大于所有点  
    }  
    return left; // left指向了应该插入的位置，也就是右侧相邻点的左侧  
}  
  
// 函数用于在查找表中找到两个最近的点进行插值  
float ntc_readtemp(uint16_t adc) {  
    int index = binary_search(adc, ntc_lookup_table.points, ARRY_SIZE);  
  
    // 如果没有找到点（即adc小于所有点），则使用最低点  
    if (index == -1) {  
        return ntc_lookup_table.points[0].tem;  
    }  
  
    // 使用找到的索引进行插值  
    // 如果index是最后一个点，则使用最后一个点和它自身进行插值（实际上不会改变值）  
    ntc_point_t *lower = &ntc_lookup_table.points[index];  
    ntc_point_t *upper = (index < ARRY_SIZE - 1) ? &ntc_lookup_table.points[index + 1] : lower;  

    // 进行线性插值  
    float temp = lower->tem + (upper->tem - lower->tem) * ((float)(adc - lower->ADC) / (upper->ADC - lower->ADC));  
    return temp;  
}  