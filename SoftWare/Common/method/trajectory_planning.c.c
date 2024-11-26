#include "trajectory_planning.c.h"
#include "debuglog.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
 #define EPSILON 0.01f            // 用于浮点数比较的容差

// static enum State {
//     INIT = 0,
//     ACC_ACC,
//     CONST_ACC,
//     DECE_DECE,
//     CONST_VEC,
//     FALL_DECE_DECE,
//     FALL_CONST_ACC,
//     FALL_ACC_ACC,
//     IDLE
// } status = INIT;



// static float last_target = 0.0f;
// static float current_Speed = 0.0f;
// static float Ja = 0.0f;//加加速度
// static uint32_t tau=0;//
// static uint32_t T1,T2,T3,T4,T5,T6,T7;
// static float V0,V1,V2,V3,V4,V5,V6,V7;
// static uint32_t t = 0;


// /**
//  * 参考链接:https://blog.csdn.net/u010632165/article/details/104951091
//  * 
//  * 
//  * 在固定周期内实现速度曲线
//  */
// float S_constant_accel_decel(float current_target)
// {
//     if (last_target != current_target)
//     {
//         // 初始化阶段
//         last_target = current_target;

//         Ja = 1.0F;

//         //T1=T3=T5=T7
//         //T2=T6
//         //T4
//         T1 = 50;T3 = T1;T5 = T1;T7 = T1;
        
//         T2 = 100;T6 = T2;
        
//         T4 = 200;

//         V0 = -100.0F;


//         status = INIT;
//         tau = 0;

//         USER_DEBUG_NORMAL("Start planning\n");
//     }
//     switch (status)
//     {
//         case INIT:
//             status = ACC_ACC;
//             break;
 
//         case ACC_ACC://加加速度阶段  1
//             if (t++<T1)
//             {
//                 current_Speed = V0 + 0.5f*Ja*tau*tau;
//                 tau++;
//                 break;
//             }
//             V1 = current_Speed;
//             t = 0;
//             tau=0;
//             status = CONST_ACC;
//             break;
 
//         case CONST_ACC://匀加速阶段  2
//             if (t++<T2)
//             {
//                 current_Speed =V1 + T1*Ja*tau;
//                 tau++;
//                 break;
//             }
//             t = 0;
//             V2 = current_Speed;
//             status = DECE_DECE;  
//             tau = 0;
//             break;
 
//         case DECE_DECE://减加速度阶段  3
//             if (t++<T3)
//             {
//                 current_Speed = V2 + T1*Ja*tau - ((0.5f*(Ja))*tau*tau);
//                 tau++;
//                 break;
//             }
//             V3 = current_Speed;
//             t = 0;
//             status = CONST_VEC;
//             tau=0;
//             break;
 
//         case CONST_VEC://匀速阶段  4
//             if (t++<T4)
//             {
//                 current_Speed = V3;
//                 break;
//             }
//             V4 = current_Speed;
//             status = FALL_DECE_DECE;
//             tau = 0;
//             t = 0;
//             break;
//         case FALL_DECE_DECE:  //5
//             if (t++<T5)
//             {
//                 current_Speed = V4 - (0.5f*Ja*tau*tau);
//                 tau++;
//                 break;
//             }
//             V5 = current_Speed;
//             t = 0;
//             tau=0;            
//             status = FALL_CONST_ACC;
//             break;
//         case FALL_CONST_ACC: //6
//             if (t++<T6)
//             {
//                 current_Speed = V5 - (Ja*T5*tau);
//                 tau++;
//                 break;
//             }
//             V6 = current_Speed;
//             tau = 0;
//             t = 0;
//             status = FALL_ACC_ACC;
//             break;

//         case FALL_ACC_ACC: 
//             if (t++<T7)
//             {
//                 current_Speed = V6 - (Ja*T5*tau) + (0.5F*Ja*tau*tau);
//                 tau++;
//                 break;
//             }
//             V7 = current_Speed;
//             tau = 0;
//             t = 0;
//             status = IDLE;
//             break;
 
//         case IDLE:
//             break;
 
//         default:
//             break;
//     }
//     return current_Speed;
// }

/**
 * S型路径规划
 * 
 * 
 * 
 */
float s_interpolation(void *object,float new_targe)
{
    enum{
        RISING,
        TS_STEP1,
        TS_STEP2,
        TS_STEP3,
        TS_STEP5,
        TS_STEP6,
        TS_STEP7,        
        FALLING,
        IDLE,
    };
    s_in_t *s = (s_in_t*)object;
    if (s->last_targe != new_targe)//路径规划
    {
        if((new_targe - s->last_targe) > EPSILON)
        {
            s->Ja = 1.0f;
            s->status = RISING;
            s->Ts[1] = 50;s->Ts[3] = 50;
            s->Ts[2] = 100;
        }else{
            s->Ts[5] = 50;s->Ts[7] = 50;
            s->Ts[6] = 100;
            s->Ja = 1.0f;            
            s->status = FALLING;
            USER_DEBUG_WARING("s dece step\n");
        }
        s->last_targe = new_targe;

        s->V0 = s->cur_output;
        s->step = 0;s->tau = 0;
    }
    switch (s->status)
    {
    case RISING:
        s->status = TS_STEP1;
    case TS_STEP1:
        if (s->step++ < s->Ts[1])
        {
            s->cur_output = s->V0 + (0.5f)*s->Ja*s->tau*s->tau;
            s->tau++;
            break;
        }
        s->V[1] = s->cur_output;
        s->step = 0;
        s->tau = 0;
        s->status = TS_STEP2;
        break;

    case TS_STEP2:
        if (s->step++ < s->Ts[2])
        {
            s->cur_output = s->V[1] + s->Ts[1]*s->Ja*s->tau;
            s->tau++;
            break;
        }
        s->V[2] = s->cur_output;
        s->step = 0;
        s->tau = 0;
        s->status = TS_STEP3;
        break;
    case TS_STEP3:
        if (s->step++ < s->Ts[3])
        {
            s->cur_output = s->V[2] \
                            + s->Ts[1]*s->Ja*s->tau \
                            - 0.5f*s->Ja*s->tau*s->tau;
            s->tau++;
            break;
        }
        s->V[3] = s->cur_output;
        s->step = 0;
        s->tau = 0;
        s->status = IDLE;
        break;    



    case FALLING:
        s->status = TS_STEP5;
    case TS_STEP5:
        if (s->step++ < s->Ts[5])
        {
            s->cur_output = s->V[0] \
                            - 0.5f*(s->Ja)*(s->tau)*(s->tau);
            s->tau++;            
            break;
        }
        s->V[5] = s->cur_output;
        s->step = 0;
        s->tau = 0;
        s->status = TS_STEP6;
        break;
    case TS_STEP6:
        if (s->step++ < s->Ts[6])
        {
            s->cur_output = s->V[5] \
                            - (s->Ja)*(s->Ts[5])*(s->tau);
            s->tau++;            
            break;
        }
        s->V[6] = s->cur_output;
        s->step = 0;
        s->tau = 0;
        s->status = TS_STEP7;
        break;
    case TS_STEP7:
        if (s->step++ < s->Ts[7])
        {
            s->cur_output = s->V[6] \
                            - (s->Ja)*(s->Ts[5])*(s->tau)\
                            + 0.5f*(s->Ja)*(s->tau)*(s->tau);
            s->tau++;            
            break;
        }
        s->V[7] = s->cur_output;
        s->step = 0;
        s->tau = 0;
        s->status = IDLE;
        break;
    default:
        break;
    }
    return s->cur_output;
}

// #include "taskmodule.h"
// void test(void) {
//     float target = 100.0f;
//     {
//         float output;
//         output = S_constant_accel_decel(target);
//     }
//     return 0;
// }  

// board_task(test)










/**----------------------------------------------------------------------
 * 
 * linear_interpolation
 * 直线插补算法
 * 
 * 
 * 该函数被一定频率调用
 ------------------------------------------------------------------------*/
#include "debuglog.h"
float linear_interpolation(void *Object,float new_targe)
{
    enum {
        INIT = 1,
        RASING,//上升插补
        FALLING,//下降插补
        IDLE
    };
    linear_in_t* linear = (linear_in_t*)Object;

    if (linear->last_targe != new_targe)//路径规划
    {
        float diff = new_targe - linear->last_targe;
        diff > 0 ? (linear->acc = 2.0f) : (linear->acc = -2.0f);
        linear->last_targe = new_targe;
        linear->status = INIT;
    }

    switch (linear->status)
    {
    case INIT:
        linear->status = (linear->acc > 0) ? RASING : FALLING;
        break;
    case RASING:
        linear->cur_output += linear->acc;
        if (linear->cur_output >= (new_targe - EPSILON))
        {
            linear->cur_output = new_targe;
            linear->status = IDLE;
            USER_DEBUG_NORMAL("Acceleration Finish\n");
        }else{
            USER_DEBUG_NORMAL("Accelerating, Current Value: %f\n", linear->cur_output);
        }        
        break;
    case FALLING:
        linear->cur_output += linear->acc;
        if (linear->cur_output <= (new_targe + EPSILON))
        {
            linear->cur_output = new_targe;
            linear->status = IDLE;
            USER_DEBUG_NORMAL("Acceleration Finish\n");
        }else{
            USER_DEBUG_NORMAL("Accelerating, Current Value: %f\n", linear->cur_output);
        }        
        break;
    case IDLE:
        break;    
    default:
        break;
    }
    return linear->cur_output;
}

void linear_interpolation_init(void *Object,float max_acc,float max_dece)
{
    linear_in_t* linear = (linear_in_t*)Object;
    linear->max_acc = max_acc;
    linear->max_dece = max_dece;
}

void linear_interpolation_deinit(void *Object)
{
    linear_in_t* linear = (linear_in_t*)Object;
    linear->max_acc = 0.0f;
    linear->max_dece = 0.0f;
}



