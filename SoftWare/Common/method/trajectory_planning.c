#include "trajectory_planning.h"
#include "debuglog.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "string.h"
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
    static linear_in_t test_line_val;
    static linear_in_t test_line2_val;

    static float target = 0.0f;
    static int16_t linear_target = 0;
    
    static uint8_t flag = 0;
    static uint8_t cnt = 0;
    static uint8_t status_ = INIT;
    switch (status_)
    {
    case INIT:
        /* code */
        target = 100.0f;
        linear_target = 8000;
        // linear_interpolation_init(&test_line_val, 10.0f);
        s_type_interpolation_init(&test_s_val, 20.0f, 40);
        t_type_interpolation_init(&test_line_val,100,30);
        t_type_interpolation_init(&test_line2_val,100,30);

        status_ = STEP1;
        break;
    case STEP1:
        if (cnt++ > 200)
        {
            cnt = 0;
            target += 100.0f;
            linear_target += 3000;
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
            linear_target -= 3000;

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
            linear_target = 8000;

            status_ = STEP4;
        }
        break;
    case STEP4:
        if (cnt++ > 200)
        {
            cnt = 0;
            target = -800.0f;
            linear_target = -8000;

            status_ = STEP5;
        }
        break;
    default:
        break;
    }
    float output;
    output = s_type_interpolation(&test_s_val, target);
    // linear_interpolation(&test_line_val, target);
    t_type_interpolation(&test_line_val, linear_target);
    t_type_interpolation(&test_line2_val, linear_target+4000);

    return 0;
}

board_task(test)
#define ACC_STEP (2) 
static void linear_shape_path_planning(linear_in_t *linear, float new_targe)
{
	int32_t out = 0;
	int32_t count = 0;
	int32_t acc = 0;
    int16_t diff = 0;
	while (1)
	{
		switch (linear->s_state)
		{
            case IDLE:
            break;
		case INIT:
            out = linear->out_ref;
            acc = 0;
            diff = new_targe - linear->last_targe;
			linear->s_state = TS_STEP1;
			break;
		case TS_STEP1:
			//计算末端速度
			out +=  acc*(count++);
			if (count >= linear->resp_max_cycle)//超过响应时间
			{
                count = 0;
				linear->s_state = TS_STEP2;
                break;
			}
            //判断是否规划成功
            bool is_successful = (diff > 0 && out >= new_targe) || (diff <= 0 && out <= new_targe);
            if (is_successful) {               
                linear->s_state = S_SUCCESS;
            }
            break;
		case TS_STEP2:
            acc += (diff > 0) ? ACC_STEP : -ACC_STEP;
            if (abs(acc) > linear->acc_max) {
                linear->s_state = FAIL; // 规划失败
            } else {
                out = linear->out_ref; // 更新输出为参考值
                linear->s_state = TS_STEP1; // 重新规划
            }            
			break;
		case S_SUCCESS:
            linear->actor_Ts = count;
            linear->acc = acc;
            linear->actor_state = RISING;
			linear->s_state = IDLE;
            return;
			break;
		case FAIL:
            linear->actor_Ts = linear->resp_max_cycle;
            linear->acc = acc;
            linear->actor_state = RISING;
			linear->s_state = IDLE;
            return;
			break;
		default:
			break;	
		}
	}
}
int16_t t_type_interpolation(linear_in_t *linear,int16_t target)
{
	if (linear->last_targe != target)
	{	//开始规划
		linear->actor_state = IDLE;//执行器进入空闲状态
        linear->s_state = INIT;//规划器进入初始状态
        linear->actor_count = 0;//清零规划器计时器器
        linear_shape_path_planning(linear,target);
        linear->last_targe = target;
	}

	switch (linear->actor_state)
	{
	case IDLE:
        linear->acc = 0;
        linear->actor_count = 0;
        linear->actor_Ts = 0;
		break;
	case RISING:
		linear->out_ref = linear->out_ref + (linear->acc) * (linear->actor_count++);
		if (linear->actor_count >= linear->actor_Ts)//到达目标速度
		{
			linear->actor_state = IDLE;
            break;
		}
		break;
	default:
		break;
	}
    /*限制执行器的输出*/
    if ((linear->acc > 0 && (linear->out_ref > target)) || (linear->acc <= 0 && (linear->out_ref < target))) {
        linear->out_ref = target;
    }
    
	return linear->out_ref;
}

void t_type_interpolation_init(linear_in_t *linear,uint16_t max_acc,uint16_t respone)
{
    linear->acc_max = max_acc;
    linear->resp_max_cycle = respone;
}

