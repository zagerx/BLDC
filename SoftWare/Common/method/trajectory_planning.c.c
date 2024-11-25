#include "trajectory_planning.c.h"
#include "debuglog.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

static enum State {
    INIT = 0,
    ACC_ACC,
    CONST_ACC,
    DECE_DECE,
    CONST_VEC,
    FALL_DECE_DECE,
    FALL_CONST_ACC,
    FALL_ACC_ACC,
    IDLE
} status = INIT;



static float last_target = 0.0f;
static float current_Speed = 0.0f;
static float Ja = 0.0f;//加加速度
static uint32_t tau=0;//
static uint32_t T1,T2,T3,T4,T5,T6,T7;
static float V0,V1,V2,V3,V4,V5,V6,V7;
static uint32_t t = 0;


/**
 * 参考链接:https://blog.csdn.net/u010632165/article/details/104951091
 * 
 * 
 * 在固定周期内实现速度曲线
 */
float S_constant_accel_decel(float current_target)
{
    if (last_target != current_target)
    {
        // 初始化阶段
        last_target = current_target;

        Ja = 1.0F;

        //T1=T3=T5=T7
        //T2=T6
        //T4
        T1 = 50;T3 = T1;T5 = T1;T7 = T1;
        
        T2 = 100;T6 = T2;
        
        T4 = 200;

        V0 = -100.0F;
        status = INIT;
        tau = 0;

        USER_DEBUG_NORMAL("Start planning\n");
    }
    switch (status)
    {
        case INIT:
            status = ACC_ACC;
            break;
 
        case ACC_ACC://加加速度阶段  1
            if (t++<T1)
            {
                current_Speed = V0 + 0.5f*Ja*tau*tau;
                tau++;
                break;
            }
            V1 = current_Speed;
            t = 0;
            tau=0;
            status = CONST_ACC;
            break;
 
        case CONST_ACC://匀加速阶段  2
            if (t++<T2)
            {
                current_Speed =V1 + T1*Ja*tau;
                tau++;
                break;
            }
            t = 0;
            V2 = current_Speed;
            status = DECE_DECE;  
            tau = 0;
            break;
 
        case DECE_DECE://减加速度阶段  3
            if (t++<T3)
            {
                current_Speed = V2 + T1*Ja*tau - ((0.5f*(Ja))*tau*tau);
                tau++;
                break;
            }
            V3 = current_Speed;
            t = 0;
            status = CONST_VEC;
            tau=0;
            break;
 
        case CONST_VEC://匀速阶段  4
            if (t++<T4)
            {
                current_Speed = V3;
                break;
            }
            V4 = current_Speed;
            status = FALL_DECE_DECE;
            tau = 0;
            t = 0;
            break;
        case FALL_DECE_DECE:  //5
            if (t++<T5)
            {
                current_Speed = V4 - (0.5f*Ja*tau*tau);
                tau++;
                break;
            }
            V5 = current_Speed;
            t = 0;
            tau=0;            
            status = FALL_CONST_ACC;
            break;
        case FALL_CONST_ACC: //6
            if (t++<T6)
            {
                current_Speed = V5 - (Ja*T5*tau);
                tau++;
                break;
            }
            V6 = current_Speed;
            tau = 0;
            t = 0;
            status = FALL_ACC_ACC;
            break;

        case FALL_ACC_ACC: 
            if (t++<T7)
            {
                current_Speed = V6 - (Ja*T5*tau) + (0.5F*Ja*tau*tau);
                tau++;
                break;
            }
            V7 = current_Speed;
            tau = 0;
            t = 0;
            status = IDLE;
            break;
 
        case IDLE:
            break;
 
        default:
            break;
    }
    return current_Speed;
}


#include "taskmodule.h"
void test(void) {
    float target = 100.0f;
    {
        float output;
        output = S_constant_accel_decel(target);
    }
    return 0;
}  











/**----------------------------------------------------------------------
 * 
 * linear_interpolation
 * 直线插补算法
 * 
 * delt_y://init时赋值
 * count://时间 根据count计算，delt_y
 * y = delt_y*count;
 * diff
 * 
 * 该函数被一定频率调用
 ------------------------------------------------------------------------*/
float linear_interpolation(void *Object,float new_targe)
{
    enum {
        INIT = 0,
        RASING,//上升插补
        FALLING,//下降插补
        IDLE
    };
    linear_in_t* linear = (linear_in_t*)Object;

    if (linear->last_targe!=linear->cur_targe)
    {
        
    }
    
    switch (linear->status)
    {
    case INIT:
        break;
    case ACCELERATING:
        break;
    case DECELERATING:
        break;
    case IDLE:
        break;    
    default:
        break;
    }
}

void linear_interpolation_init(void *Object)
{
    linear_in_t* linear = (linear_in_t*)Object;
    linear->delt_y = 0.0f;
    linear->count = 0;
}

void linear_interpolation_deinit(void *Object)
{

}

board_task(test)


