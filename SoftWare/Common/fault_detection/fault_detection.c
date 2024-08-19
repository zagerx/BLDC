#include "protection_module.h"

/*

该模块的目的主要是处理，需要连续检测的一些故障，典型的就是电源电压的故障检查
对Vbus电压进行检测，允许电压在一定时间内低于或高于上下限
若长时间不在范围内，则报错


板子上的常用故障:
电压/电流故障


故障检测模块





void func1(viod)
{
    writevalue_to(this,valu);
    if(get_curstate())
    {
        //错误
    }
}

void writevalue_to(this,value)

getcurstate(this)



错误索引，阈值，错误码/错误类型， 时间  错误处理
{VBUS,H_LIMIT,L_LIMIT,ERR_CODE,TIME,CALLBACK}


要实现两个基本功能:




1、连续模式

bool flaut_jude(VBUS,vlaue)




*/
typedef enum {  
    FAULT_TYPE_RECOVERABLE,  
    FAULT_TYPE_UNRECOVERABLE  
} fault_type_t;

// 枚举定义故障状态  
typedef enum {  
    FAULT_PRESENT,    // 表示有故障  
    FAULT_ABSENT      // 表示没有故障  
} fault_status_t;

typedef struct _fault
{
    /* data */
    float h_limit;
    float l_limit;
    float h_rev;
    float l_rev;
    float threshold;
}fault_t;
// 枚举定义不同类型的系统故障  
typedef enum {  
    FAULT_TYPE_VOLTAGE,    // 电压故障  
    FAULT_TYPE_CURRENT,    // 电流故障  
    FAULT_TYPE_SPEED,      // 速度故障  
    FAULT_TYPE_UNKNOWN,    // 未知故障类型（可选）  
} SystemFaultType; 
void fault_process(void)
{


}

fault_status_t fault_read(SystemFaultType fault_index)
{

}

void fault_init(fault_type_t _type,fault_t p)
{

}





