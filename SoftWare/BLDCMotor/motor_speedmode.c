#include "motor_speedmode.h"
#include "mc_protocol.h"
#include "motorctrl_common.h"
#include "string.h"
#include "mc_encoder.h"
#include "mc_speedloop.h"
#include "board.h"
#include "motorctrl_cfg.h"
#include "debuglog.h"
#include "mc_smo.h"
#include "math.h"
static void motor_paraminit(motor_t *motor);
static void motor_paramdeinit(motor_t *motor);

fsm_rt_t motor_speedmode(fsm_cb_t *pthis)
{
    enum{
        READY = USER,
        CALIBRATE,
        IDLE,
        RUN,
        INIT,
    };
    motor_t *motor;
    motor = (motor_t*)pthis->pdata;    
    switch (pthis->chState)
    {
    case ENTER:
        USER_DEBUG_NORMAL("entry speed mode\n");
        motor_paraminit(motor);//编码器初始化，获取初始位置
        motor->encoder_handle.runflag = 1;
        pthis->chState = READY;
    case READY:
        if (motor->curmode != M_SET_START)
        {
            break;
        }
        motor->enable();
        pthis->chState = RUN;
        break;    
    case RUN:
        if (motor->curmode == M_SET_STOP)
        {
            pthis->chState = EXIT;
        }else{
            if (motor->currment_handle.pid_debug_target!=0.0f)
            {                
                motor->encoder_handle.runflag = 1;
                motor->speed_handle.tar = motor->currment_handle.pid_debug_target;
                motor->speed_handle.real = motor->encoder_handle.speed;
                motor->currment_handle.iq_tar = speed_loop(&(motor->speed_handle));
                motor->currment_handle.id_tar = 0.0f;
            }
        }
        break;
    case EXIT:
        USER_DEBUG_NORMAL("exit speed mode\n");
        motor->disable();
        motor->currment_handle.pid_debug_target = 0.0f;
        motor_paramdeinit(motor);
        pthis->chState = ENTER;
        break;
    default:
        break;
    }
    return 0;
}

static void motor_paraminit(motor_t *motor)
{
    mc_encoder_init(&(motor->encoder_handle));
#ifdef MOTOR_CURMENLOOP_DEBUG
    /*从FLASH指定位置读取PID参数数据*/
    motor_Romparam_t temp;
    motor->read(&temp,sizeof(motor_Romparam_t));
    /*初始化PID参数*/
    pid_init(&(motor->currment_handle.d_pid),temp.fbuf[0],temp.fbuf[1],1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    pid_init(&(motor->currment_handle.q_pid),temp.fbuf[0],temp.fbuf[1],1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    USER_DEBUG_NORMAL("D_Axis Kp:%.04f Ki:%.04f\n",motor->currment_handle.d_pid.kp,motor->currment_handle.d_pid.ki);  
#elif MOTOR_CURMENLOOP_TEST
    pid_init(&(motor->currment_handle.d_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    pid_init(&(motor->currment_handle.q_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);   
#elif MOTOR_SPEEDLOOP_DEBUG
    pid_init(&(motor->currment_handle.d_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    pid_init(&(motor->currment_handle.q_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);  

    motor_Romparam_t temp;
    motor->read((uint8_t *)&temp,sizeof(motor_Romparam_t));   
    pid_init(&(motor->speed_handle.pid),temp.fbuf[0],temp.fbuf[1],1.0,SPEED_OUT_MAX,SPEED_OUT_MIN);    
    lowfilter_init(&(motor->encoder_handle.speedfilter),15.0f);        
    USER_DEBUG_NORMAL("Currment  :   Kp:%.4f Ki:%.4f\n",motor->currment_handle.q_pid.kp,motor->currment_handle.q_pid.ki);  
    USER_DEBUG_NORMAL("Spedd loop:   Kp:%.4f,Ki:%.4f\n",motor->speed_handle.pid.kp,motor->speed_handle.pid.ki);
#endif


    pid_init(&(motor->currment_handle.d_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);
    pid_init(&(motor->currment_handle.q_pid),CURRMENTLOOP_KP,CURRMENTLOOP_KI,1.0,CIRCLE_OUT_MAX,CIRCLE_OUT_MIN);  
    pid_init(&(motor->speed_handle.pid),SPEEDLOOP_KP,SPEEDLOOP_KI,1.0,SPEED_OUT_MAX,SPEED_OUT_MIN);    
    lowfilter_init(&(motor->encoder_handle.speedfilter),15.0f);        
    USER_DEBUG_NORMAL("Currment  :   Kp:%.4f Ki:%.4f\n",CURRMENTLOOP_KP,CURRMENTLOOP_KI);  
    USER_DEBUG_NORMAL("Spedd loop:   Kp:%.4f,Ki:%.4f\n",SPEEDLOOP_KP,CURRMENTLOOP_KI);

}



static void motor_paramdeinit(motor_t *motor)
{
    // memset(motor,0,sizeof(motor_t));
    // motor_func_register(motor);
}



#include <stdint.h>
#include <stdbool.h>

// 定义一些常量
#define MAX_ACCEL 100.0f  // 最大加速度，单位：速度/时间（例如，速度/毫秒）
#define MAX_DECEL 100.0f  // 最大减速度，单位同上
#define TIME_STEP 1.0f    // 时间步长，单位：毫秒

// 状态枚举
typedef enum {
    ACCELERATING,
    CONSTANT_SPEED,
    DECELERATING,
    IDLE
} MotionState;

// 梯形加减速函数
float trapezoidal_accel_decel(float target_speed, float current_speed, float *last_target_speed) {
    static float accumulated_time = 0.0f;  // 累计时间，用于计算速度变化
    static MotionState state = IDLE;      // 当前运动状态
    static float acceleration = 0.0f;     // 当前加速度
    static float deceleration = 0.0f;     // 当前减速度
    static float target_reached_time = 0.0f; // 预计达到目标速度的时间
    static float max_speed = 0.0f;         // 梯形速度曲线的最大速度

    // 如果目标速度变化了，重新计算梯形曲线参数
    if (target_speed != *last_target_speed) {
        float speed_diff = target_speed - current_speed;
        float accel_time = fabs(speed_diff) / MAX_ACCEL;
        float decel_time = fabs(speed_diff) / MAX_DECEL;
        float total_time = accel_time + decel_time;
        max_speed = current_speed + (speed_diff > 0 ? MAX_ACCEL : -MAX_ACCEL) * accel_time;

        // 如果加速和减速时间之和大于0，则更新状态和时间参数
        if (total_time > 0) {
            target_reached_time = accumulated_time + total_time;
            acceleration = (speed_diff > 0 ? MAX_ACCEL : -MAX_ACCEL);
            deceleration = (speed_diff > 0 ? -MAX_DECEL : MAX_DECEL);

            // 根据当前速度与目标速度的关系设置初始状态
            if (current_speed < target_speed) {
                state = ACCELERATING;
            } else {
                state = DECELERATING;
                acceleration = deceleration; // 在减速开始时，加速度实际上是负值的减速度
            }
        } else {
            // 如果目标速度与当前速度相同，则无需移动
            state = IDLE;
        }

        // 更新最后的目标速度值
        *last_target_speed = target_speed;
    }

    // 根据当前状态更新速度和累计时间
    switch (state) {
        case ACCELERATING:
            if (accumulated_time < (target_reached_time - fabs(target_speed - max_speed) / MAX_DECEL)) {
                current_speed += acceleration * TIME_STEP;
            } else {
                // 切换到恒速阶段（如果需要的话，可以省略此阶段，直接跳到减速）
                // 但为了完整性，这里保留它
                current_speed = max_speed;
                state = CONSTANT_SPEED; // 实际上，为了简化，我们可以直接跳到减速阶段
                // 注意：为了简化代码，我们没有实现恒速阶段，而是直接准备减速
            }
            break;

        case CONSTANT_SPEED:
            // 此处可以添加恒速阶段的逻辑，但为简化起见，我们直接跳过到减速阶段
            state = DECELERATING; // 强制进入减速阶段（如果需要恒速，请移除此行）
            // 注意：为了保持示例的简洁性，我们省略了恒速阶段
            // 在实际应用中，您可能希望在此处添加一些逻辑来处理恒速阶段
            break;

        case DECELERATING:
            if (accumulated_time < target_reached_time) {
                if ((current_speed - deceleration * TIME_STEP) > target_speed) {
                    current_speed -= deceleration * TIME_STEP;
                } else {
                    current_speed = target_speed;
                    state = IDLE;
                }
            } else {
                current_speed = target_speed;
                state = IDLE;
            }
            break;

        case IDLE:
            // 无需更新速度，保持在目标速度
            break;
    }

    // 更新累计时间
    accumulated_time += TIME_STEP;

    // 返回更新后的速度
    return current_speed;
}

// 注意：这个函数需要外部调用者提供当前速度（可能是从传感器读取的）和上次的目标速度（用于检测变化）
// 调用示例：
// float current_speed = 0.0f; // 初始速度
// float last_target_speed = 0.0f; // 上次的目标速度，用于比较是否变化
// float new_target_speed = 500.0f; // 新的目标速度
// current_speed = trapezoidal_accel_decel(new_target_speed, current_speed, &last_target_speed);
