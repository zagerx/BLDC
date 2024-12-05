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
    s_in_t *s = (s_in_t *)object;
    float diff;
    float v0, v1, v2, v3;
    float temp_delte;
    float temp_ja;

    diff = new_targe - s->last_targe;
    s->V0 = s->cur_output;
    v0 = s->cur_output;
    temp_ja = s->max_acc;

    /*更新规划时间*/
    uint16_t T2 = s->Tm / 2;
    uint16_t T1 = (s->Tm - T2) / 2;
    if (diff > EPSILON)
    {
        s->Ts[1] = s->Ts[3] = T1; // 加速/减速阶段 时间需要保持一致
        s->Ts[2] = T2;
        s->status = RISING;
    }
    else
    {
        s->Ts[5] = s->Ts[7] = T1; // 加速/减速阶段 时间需要保持一致
        s->Ts[6] = T2;
        s->status = FALLING;
    }

    /*规划更新Ja*/
    while (1)
    {
        if (temp_ja < 0.0f) // 规划失败
        {
            break;
        }
        temp_ja -= 0.01f;
        s->Ja = (s->status == RISING) ? temp_ja : -temp_ja;
        v1 = v0 + (0.5f) * (s->Ja) * (T1) * (T1);
        v2 = v1 + (T1) * (s->Ja) * (T2);
        v3 = v2 + T1 * s->Ja * T1 - 0.5f * s->Ja * T1 * T1;
        temp_delte = (s->status == RISING) ? (v3 - new_targe) : -(v3 - new_targe);
        if (temp_delte < EPSILON)
        {
            break;
        }
    }

    s->cout = 0;
    s->tau = 0;

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
    static float target = 0.0f;
    static uint8_t flag = 0;
    static uint8_t cnt = 0;
    static uint8_t status_ = INIT;
    switch (status_)
    {
    case INIT:
        /* code */
        target = 100.0f;
        linear_interpolation_init(&test_line_val, 10.0f);
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
    linear_interpolation(&test_line_val, target);
    return 0;
}

board_task(test)

    /*==========================================================================================
     * @brief
     * @FuncName
     * @param        Object
     * @param        new_targe
     * @return       float
     * @version      0.1
    --------------------------------------------------------------------------------------------*/
    float linear_interpolation(void *Object, float new_targe)
{
    enum
    {
        INIT = 1,
        RASING,  // 上升插补
        FALLING, // 下降插补
        IDLE
    };
    linear_in_t *linear = (linear_in_t *)Object;

    if (linear->last_targe != new_targe) // 路径规划
    {
        float diff = new_targe - linear->last_targe;
        diff > 0 ? (linear->acc = linear->max_acc) : (linear->acc = -linear->max_acc);
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
        }
        else
        {
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
        }
        else
        {
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

void linear_interpolation_init(void *Object, float max_acc)
{
    linear_in_t *linear = (linear_in_t *)Object;
    linear->max_acc = max_acc;
}

void linear_interpolation_deinit(void *Object)
{
    linear_in_t *linear = (linear_in_t *)Object;
    linear->max_acc = 0.0f;
}
