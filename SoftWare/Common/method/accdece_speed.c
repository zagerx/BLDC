#include "accdece_speed.h"
#include "debuglog.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define MAX_ACCELERATION 10.0f
#define MAX_DECELERATION -10.0f 
#define EPSILON 0.01f
#define TIME_STEP 1.0f  // 1ms

static enum State {
    INIT = 0,
    ACCELERATING,
    CONST_VELOCITY,
    DECELERATING,
    IDLE
} status = INIT;

static float current_output = 0.0f;
static float last_target = 0.0f;
static float acceleration = 0.0f;
static float target_velocity = 0.0f;
static float first_val,sec_val,thr_val;
static uint32_t cout=0;
static uint32_t j = 0;;
float S_constant_accel_decel(float current_target)
{
    if (last_target != current_target)
    {
        // 初始化阶段
        last_target = current_target;
        acceleration = 1.0F;
        cout = 0;
        status = INIT;
        USER_DEBUG_NORMAL("Start planning\n");
    }
    switch (status)
    {
        case INIT:
            status = ACCELERATING;
            break;
 
        case ACCELERATING:
            if (j++<20)
            {
                current_output = 0.5f*acceleration*cout*cout;
                cout++;
                break;
            }
            first_val = current_output;
            j = 0;
            cout=0;
            status = CONST_VELOCITY;
            // break;
 
        case CONST_VELOCITY:
            if (j++<100)
            {
                current_output =first_val+ 20*acceleration*cout;
                cout++;
                break;
            }
            j = 0;
            sec_val = current_output+200.0f;
            status = DECELERATING;
            cout = 20;
            // break;
 
        case DECELERATING:
            if (j++<20)
            {
                current_output = sec_val - ((0.5f*(acceleration))*cout*cout);
                --cout;
                break;
            }
            j = 0;
            status = IDLE;
            break;
 
        case IDLE:
            break;
 
        default:
            break;
    }
 
    return current_output;
}
#include "taskmodule.h"
void test(void) {
    float target = 100.0f;
    static uint32_t cout;
    {
        float output;
        output = S_constant_accel_decel(target);
        USER_DEBUG_NORMAL("%d--->Output: %f\n",cout++, output);
        // USER_DEBUG_NORMAL("test\n");
    }
    return 0;
}  

board_task(test)


