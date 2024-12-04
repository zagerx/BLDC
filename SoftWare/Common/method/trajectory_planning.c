#include "trajectory_planning.h"
#include "debuglog.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#define EPSILON 0.01f            // 用于浮点数比较的容差
/*==========================================================================================
 * @brief        S型插补(指数函数)
 * @FuncName     s_interpolation
 * @param        object 
 * @param        new_targe 
 * @return       float 
 * @version      0.1
 * 
 * 计算差值
 * 获取当前加速度
 * 
 * v1=v0+0.5*a*T1^2
 * V2 = V1+at
 * V3 = V2 + 0.5*a*T1^2
 
 * diff = v1+v2+v3 = v0+0.5*a*T1^2 + v0+0.5*a*T1^2+aT2 + v0+0.5*a*T1^2+aT2+0.5*a*T1^2
 *                 = 3v0+4(0.5*a*T1^2) + 2aT2
 *                 
 * 
--------------------------------------------------------------------------------------------*/
float s_interpolation(void *object,float new_targe)
{
    enum{
        RISING = 0,
        TS_STEP1 = 1,
        TS_STEP2 = 2,
        TS_STEP3 = 3,
        TS_STEP5 = 5,
        TS_STEP6 = 6,
        TS_STEP7 = 7,        
        FALLING  = 8,
        IDLE = 9,
    };
    static float out_val;
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
        s->step = 0;s->tau = 0;
    }
    switch (s->status)
    {
    case RISING:
        s->status = TS_STEP1;
    case TS_STEP1:
        out_val = s->V0 + (0.5f)*s->Ja*s->tau*s->tau;
        s->V[1] = out_val;
        if (s->step++ < s->Ts[1])
        {
            s->tau++;
            break;
        }
        s->V0 = s->V[1];
        s->step = 0;
        s->tau = 0;
        s->status = TS_STEP2;
        break;

    case TS_STEP2:
        out_val = s->V[1] + (s->Ts[1])*(s->Ja)*(s->tau);
        s->V[2] = out_val;   
        s->V0 = out_val; 
        if (s->step++ < s->Ts[2])
        {
            s->tau++;
            break;
        }
        s->step = 0;
        s->tau = 0;
        s->status = TS_STEP3;
        break;
    case TS_STEP3:
        s->V0 = out_val;
        out_val = s->V[2] \
                        + s->Ts[1]*s->Ja*s->tau \
                        - 0.5f*s->Ja*s->tau*s->tau;
        s->V[3] = out_val;
        if (s->step++ < s->Ts[3])
        {
            s->tau++;
            break;
        }
        s->step = 0;
        s->tau = 0;
        s->status = IDLE;
        break;    



    case FALLING:
        s->status = TS_STEP5;
    case TS_STEP5:
        out_val = s->V0 \
                        - 0.5f*(s->Ja)*(s->tau)*(s->tau);
        s->V[5] = out_val;
        if (s->step++ < s->Ts[5])
        {
            s->tau++;            
            break;
        }
        s->V0 = out_val;
        s->step = 0;
        s->tau = 0;
        s->status = TS_STEP6;
        break;
    case TS_STEP6:
        out_val = s->V[5] \
                        - (s->Ja)*(s->Ts[5])*(s->tau);
        s->V[6] = out_val;
        s->V0 = out_val;
        if (s->step++ < s->Ts[6])
        {
            s->tau++;            
            break;
        }
        s->step = 0;
        s->tau = 0;
        s->status = TS_STEP7;
        break;
    case TS_STEP7:
        out_val = s->V[6] \
                        - (s->Ja)*(s->Ts[5])*(s->tau)\
                        + 0.5f*(s->Ja)*(s->tau)*(s->tau);
        s->V[7] = out_val;
        s->V0 = out_val;
        if (s->step++ < s->Ts[7])
        {
            s->tau++;            
            break;
        }
        s->step = 0;
        s->tau = 0;
        s->status = IDLE;
        break;

    case IDLE:
        break;
    
    default:
        break;
    }
    s->cur_output = out_val;
    return out_val;
}

#include "taskmodule.h"
void test(void) {
    enum{
        INIT,
        STEP1,
        STEP2,
        STEP3,

    };
    static s_in_t test_s_val;
    static float target = 0.0f;
    static uint8_t flag = 0;
    static uint8_t cnt = 0;
    static uint8_t status_ = INIT;
    switch (status_)
    {
    case INIT:
        /* code */
        target = 100.0f;
        status_ = STEP1;
        break;
    case STEP1:
        if (cnt++>200)
        {
            cnt = 0;
            target += 100.0f;
            if (target>100.0f)
            {
                status_ = STEP2;
                break;
            }
        }
        break;
    case STEP2:
        if (cnt++>200)
        {
            cnt = 0;
            target -= 100.0f;
            if (target < -100.0f)
            {
                status_ = STEP3;
                break;
            }
        }
        break;
    default:
        break;
    }
    float output;
    output = s_interpolation(&test_s_val,target);    
    return 0;
}  


board_task(test)










/**----------------------------------------------------------------------
 * 
 * linear_interpolation
 * 直线插补算法
 * 
 * 
 * 该函数被一定频率调用
 ------------------------------------------------------------------------*/
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



