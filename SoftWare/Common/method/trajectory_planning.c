#include "trajectory_planning.h"
#include "debuglog.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "string.h"
#include "stdlib.h"
#define EPSILON 0.01f // 用于浮点数比较的容差
enum
{
    RISING = 0,
    TS_STEP1 = 1,
    TS_STEP2 = 2,
    TS_STEP3 = 3,
    TS_STEP5 = 5,
    TS_STEP6 = 6,
    TS_STEP7 = 7,
    FALLING = 8,
    IDLE = 9,
    INIT = 10,
    FAIL = 11,
    S_SUCCESS = 12,
    ZERO_POINT = 13,
    OVER = 14,
    WAIT = 15,
};
/*==========================================================================================
 * @brief        S型轨迹规划
 * @FuncName
 * @param        object
 * @param        new_targe
 * @version      0.1
--------------------------------------------------------------------------------------------*/
static void s_shape_path_planning(void *object, float new_targe)
{
    enum
    {
        INIT,
        STEP1,
        STEP2,
        STEP3,
        PLAN_FAIL,
        PLAN_SUCESS,
    };
    s_in_t *s = (s_in_t *)object;
    float diff;
    float v0, v1, v2, v3;
    float temp_delte;
    float temp_ja;
    uint16_t T2;
    uint16_t T1;
    /*规划更新Ja*/
    s->state = INIT;
    while (1)
    {
        switch (s->state)
        {
        case INIT:
            diff = new_targe - s->last_targe;
            s->V0 = s->cur_output;
            v0 = s->cur_output;
            temp_ja = s->max_acc;
            s->state = STEP1;
            break;
        case STEP1:
            /*更新规划时间*/
            T2 = s->Tm / 2;
            T1 = (s->Tm - T2) / 2;
            if (diff > EPSILON)
            {
                s->Ts[1] = s->Ts[3] = T1; // 加速/减速阶段 时间需要保持一致
                s->Ts[2] = T2;
            }
            s->state = STEP2;
            break;
        case STEP2:
            if (temp_ja < 0.0f) // 规划失败
            {
                s->state = PLAN_FAIL;
            }
            temp_ja -= 0.01f;
            s->Ja = (diff > EPSILON) ? temp_ja : -temp_ja;
            v1 = v0 + (0.5f) * (s->Ja) * (T1) * (T1);
            v2 = v1 + (T1) * (s->Ja) * (T2);
            v3 = v2 + T1 * s->Ja * T1 - 0.5f * s->Ja * T1 * T1;
            temp_delte = (diff > EPSILON) ? (v3 - new_targe) : -(v3 - new_targe);
            if (temp_delte < EPSILON) // 规划成功
            {
                s->state = PLAN_SUCESS; // 规划成功
                break;
            }
            break;
        case PLAN_FAIL:
            return;
            break;
        case PLAN_SUCESS:
            s->cout = 0;
            s->tau = 0;
            s->status = RISING;
            s->state = INIT; // 规划成功
            return;
            break;
        default:
            break;
        }
    }
    return;
}
/*==========================================================================================
 * @brief        S型插补(指数函数)
 * @FuncName     s_type_interpolation
 * @param        object
 * @param        new_targe
 * @return       float
 * @version      0.1
 *
 * --------------------------------------------------------------------------------------------*/
float s_type_interpolation(void *object, float new_targe)
{
    float out_val;
    s_in_t *s = (s_in_t *)object;

    if (s->last_targe != new_targe) // 路径规划
    {
        s_shape_path_planning(s, (new_targe));
        s->last_targe = new_targe;
    }

    switch (s->status)
    {
    case RISING:
    case FALLING:
        s->status = TS_STEP1;
    case TS_STEP1:
        s->V[1] = s->V0 + (0.5f) * (s->Ja) * (s->tau) * (s->tau);
        out_val = s->V[1];
        if (s->cout++ < s->Ts[1]) // 第一阶段未完成
        {
            s->tau++;
            break;
        }
        s->cout = 0;
        s->tau = 0;
        s->status = TS_STEP2;
        break;

    case TS_STEP2:
        s->V[2] = s->V[1] + (s->Ts[1]) * (s->Ja) * (s->tau);
        ;
        out_val = s->V[2];
        if (s->cout++ < s->Ts[2])
        {
            s->tau++;
            break;
        }
        s->cout = 0;
        s->tau = 0;
        s->status = TS_STEP3;
        break;
    case TS_STEP3:
        s->V[3] = s->V[2] + s->Ts[1] * s->Ja * s->tau - 0.5f * s->Ja * s->tau * s->tau;
        out_val = s->V[3];
        if (s->cout++ < s->Ts[3])
        {
            s->tau++;
            break;
        }
        s->cout = 0;
        s->tau = 0;
        s->status = IDLE;
        break;
    case IDLE:
        return;
        break;

    default:
        break;
    }
    s->cur_output = out_val;
    return out_val;
}
void s_type_interpolation_init(void *object, float Ja_max, uint16_t T_max)
{
    s_in_t *s = (s_in_t *)object;
    s->status = IDLE;
    s->max_acc = Ja_max;
    s->cur_output = 0.0f;
    s->Tm = T_max;
}
void s_type_interpolation_deinit(void *object)
{
    memset(object, 0, sizeof(s_in_t));
}
#include "taskmodule.h"
#if 0
void test(void)
{
    enum
    {
        INIT,
        STEP1,
        STEP2,
        STEP3,
        STEP4,
        STEP5,
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
        s_type_interpolation_init(&test_s_val, 20.0f, 40);
        status_ = STEP1;
        break;
    case STEP1:
        if (cnt++ > 200)
        {
            cnt = 0;
            target += 100.0f;
            if (target > 300.0f)
            {
                status_ = STEP2;
                break;
            }
        }
        break;
    case STEP2:
        if (cnt++ > 200)
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
    case STEP3:
        if (cnt++ > 200)
        {
            cnt = 0;
            target = 800.0f;
            status_ = STEP4;
        }
        break;
    case STEP4:
        if (cnt++ > 200)
        {
            cnt = 0;
            target = -800.0f;
            status_ = STEP5;
        }
        break;
    default:
        break;
    }
    float output;
    output = s_type_interpolation(&test_s_val, target);
    return 0;
}
board_task(test)
#endif

#if 1 //T型测试函数
#define MAX_CYC  1600  //最大加速周期
#define MAX_ACC  300  //最大加速度
#define MIN_SPEED_DIFF (MAX_CYC) //最小速度差
#define MAX_SPEED_DIFF (48888)   //最大速度差
#define ACC_STEP (1) //加加速度

/*
*  最大速度差 = v0 + acc*最大加速周期
*  最大加速周期固定 唯一的变量是acc。所以acc_max = max_diff/最大加速周期
*  
*  最小速度差的确定:当加速度最小是，在

1、速度规划第一参考：必须满足规定响应时间
2、在规定时间内:以最大加速度运行，任然不能满足指定目标
3、以最小加速度运行，提前到达目标值


计算速度差 

速度差/最大加速 = 加速时间  是否大于响应时间，规划失败，则以最大加速进行规划

如果小于等于响应时间 
以响应时间，计算合适的加速度。
若无解(加速度小于)
*/

#include "main.h"
#include "rng.h"
#include "random.h"
#include "stdlib.h"
void t_shape_test(void)
{
    enum
    {
        INIT,
        STEP1,
        STEP2,
        STEP3,
        STEP4,
        STEP5,
    };    
    static linear_in_t test_line1_val;
    static linear_in_t test_line2_val;
    static int32_t linear1_target = 0;
    static int32_t linear2_target = 0;
    static int32_t CMD_deployment_cycle = 0;//指令下发周期
    static random_t rand1;
    static random_t rand2;
    static random_t rand3;

    int32_t xxx;
    int32_t rand_value = 0;

    static uint32_t cnt = 0;
    static uint8_t status_ = INIT;
    switch (status_)
    {
    case INIT://初始化待测试模块
        t_type_interpolation_init(&test_line1_val,MAX_ACC,MAX_CYC);
        t_type_interpolation_init(&test_line2_val,MAX_ACC,MAX_CYC);
        random_init(&rand1,24444,800);
        random_init(&rand2,24444,800);
        random_init(&rand3,80,20);
        status_ = STEP1;
        break;
    case STEP1:
            linear1_target = -400;
            linear2_target = 24444;
            cnt = 0;
            status_ = STEP2;
        break;
    case STEP2:
        // if (cnt++ > (MAX_CYC+200))
        if (cnt++ > (800+200))        
        {
            cnt = 0;
            linear1_target = 400;
            linear2_target = -24444; 
            status_ = IDLE;
            // status_ = STEP5;
            CMD_deployment_cycle = MAX_CYC;
        }
        break;
    case IDLE:
        if (cnt++ < CMD_deployment_cycle )
        {
            break;
        }
        cnt = 0;
        HAL_RNG_GenerateRandomNumber(&hrng, &xxx);    
        rand_value = random_cacle(&rand3,xxx);    
        // CMD_deployment_cycle = MAX_CYC + rand_value;//更新指令周期
        CMD_deployment_cycle = MAX_CYC + 200;//更新指令周期

        status_ = STEP4;
        break;        
    case STEP4:
        HAL_RNG_GenerateRandomNumber(&hrng, &xxx);
        linear1_target = random_cacle(&rand1,xxx);
        HAL_RNG_GenerateRandomNumber(&hrng, &xxx);
        linear2_target = random_cacle(&rand2,xxx);
        status_ = IDLE;
        break;
    default:

        break;
    }
    static int32_t output1;
    static int32_t output2;
    output1 = t_type_interpolation(&test_line1_val, linear1_target);
    output2 = t_type_interpolation(&test_line2_val, linear2_target);
    return 0;    
}
board_task(t_shape_test)
#endif
/*==========================================================================================
 * @brief        线性规划函数
 * @FuncName     
 * @param        linear 
 * @param        new_targe 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
static void linear_shape_path_planning(linear_in_t *linear, int32_t new_targe)
{
	float out = 0;
	float acc = 0;
    float diff = 0;
	while (1)
	{
		switch (linear->s_state)
		{
            case IDLE:
            break;
		case INIT:

            /*是否穿越0点  更新执行器状态*/
            if ((new_targe > 0 && linear->last_targe<0) || (new_targe<0 && linear->last_targe>0))
            {
                linear->actor_state = ZERO_POINT;
                /*计算加速度*/
                diff = (float)new_targe - (float)linear->last_targe;
                acc = ((float)diff/(float)(linear->resp_max_cycle - (linear->resp_max_cycle/8)));

            }else{
                linear->actor_state = RISING;
                /*计算加速度*/
                diff = (float)new_targe - (float)linear->last_targe;
                acc = ((float)diff/(float)linear->resp_max_cycle);
            }
			linear->s_state = TS_STEP1;
			break;
		case TS_STEP1:
            if(fabs(acc)>linear->acc_max)
            {
                acc = acc>0.0f?linear->acc_max:-linear->acc_max;
            }
            if(fabs(acc)<0.01f)
            {
                acc = acc>0.0f?0.01f:-0.01f;
            }
            linear->s_state = S_SUCCESS;
            break;
		case S_SUCCESS:
            linear->actor_Ts = linear->resp_max_cycle;
            linear->acc = acc;
			linear->s_state = IDLE;
            return;
			break;
		default:
			break;	
		}
	}
}
/*==========================================================================================
 * @brief        线性插补算法
 * @FuncName     
 * @param        linear 
 * @param        target 
 * @return       int32_t 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
int32_t t_type_interpolation(linear_in_t *linear,int32_t target)
{
	if (linear->last_targe != target)
	{	//开始规划
		linear->actor_state = IDLE;//执行器进入空闲状态
        linear->s_state = INIT;//规划器进入初始状态
        linear->actor_count = 0;//清零规划器计时器器
        linear_shape_path_planning(linear,target);
        linear->last_targe = target;
	}

    /* 执行器 */
    float out = 0;
	switch (linear->actor_state)
	{
	case IDLE:
        linear->acc = 0.0f;
        linear->actor_count = 0;
        linear->actor_Ts = 0;
        linear->out_ref = 0.0f;
		break;
	case RISING://加速中
		out = linear->V0 + (linear->acc);
        linear->V0 = out;
        /*是否到达目标速度*/
        if ((linear->acc > 0.0f && (out >= (float)linear->last_targe)) \
         || (linear->acc < 0.0f && (out <= (float)linear->last_targe)))
		{
            out = target;//限制速输出
            linear->out_ref = out; 
            linear->acc = 0.0f;
            linear->actor_count = 0;
            linear->actor_Ts = 0;             
			linear->actor_state = OVER;
            break;
		}
		break;

    case ZERO_POINT:
		out = linear->V0 + (linear->acc);
        linear->V0 = out;
        if((-fabs(linear->acc))<out && out<(fabs(linear->acc)))        
        {
            linear->actor_state = WAIT;
        }
        break;
    case WAIT:
        out = 0;
        if(linear->actor_count++<=(linear->actor_Ts/8))
        {
            break;
        }
        linear->actor_count = 0;
        linear->actor_state = RISING;
        break;
    case OVER://加速结束
        out = linear->out_ref;
        break;
	default:
		break;
	}
	return (int32_t)out;
}
/*==========================================================================================
 * @brief        线性插补chushih
 * @FuncName     
 * @param        linear 
 * @param        max_acc 
 * @param        respone 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void t_type_interpolation_init(linear_in_t *linear,uint32_t max_acc,uint32_t respone)
{
    linear->last_targe = 0;
    linear->acc_max = max_acc;
    linear->resp_max_cycle = respone;
}
/*==========================================================================================
 * @brief        线性插补反初始化
 * @FuncName     
 * @param        linear 
 * @version      0.1
--------------------------------------------------------------------------------------------*/
void t_type_interpolation_deinit(linear_in_t *linear)
{
    linear->acc         = 0;
    linear->out_ref     = 0;
    linear->V0          = 0;
    linear->last_targe  = 0;
    linear->actor_count = 0;
    linear->actor_Ts    = 0;
    linear->actor_state = 0;
    linear->s_state     = 0;
}
