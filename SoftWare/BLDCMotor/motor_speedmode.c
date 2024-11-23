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
#include "accdece_speed.h"
static void motor_paraminit(motor_t *motor);
static void motor_paramdeinit(motor_t *motor);
static float constant_accel_decel(float current_target);

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
                motor->speed_handle.tar = constant_accel_decel(motor->currment_handle.pid_debug_target) ;//motor->currment_handle.pid_debug_target;
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
#include <stdio.h>
 
// 定义一些常量
#define MAX_ACCELERATION 10.0f  // 最大加速度，单位：速度/时间（例如，速度/毫秒）
#define MAX_DECELERATION -10.0f  // 最大减速度，单位同上
#define EPSILON 0.01f            // 用于浮点数比较的容差

/*==========================================================================================
 * @brief 匀加减速规划
 * 实现简单的加减速规划。
 * @FuncName     
 * @param        current_target 
 * @return       * float 
 * @version      v0.1
--------------------------------------------------------------------------------------------*/
static float constant_accel_decel(float current_target)
{
    static enum State {
        INIT,
        ACCELERATING,
        DECELERATING,
        IDLE
    } status = INIT;
 
    static float current_output = 0.0f;
    static float last_target = 0.0f;
    static float acceleration = 0.0f;
    static uint16_t accumulator = 0;
 
    // 如果目标值改变，重新规划
    if (last_target != current_target)
    {
        float speed_difference = current_target - current_output;
        speed_difference > 0 ? (acceleration = MAX_ACCELERATION) : (acceleration = MAX_DECELERATION);
        last_target = current_target;
        accumulator = 0;
        status = INIT;
        USER_DEBUG_NORMAL("Start planning\n");
    }
 
    // 状态机逻辑
    switch (status)
    {
        case INIT:
            status = (acceleration > 0) ? ACCELERATING : DECELERATING;
            break;
 
        case ACCELERATING:
            current_output += acceleration;
            accumulator++;
 
            if (current_output >= (current_target - EPSILON))
            {
                current_output = current_target;
                status = IDLE;
                USER_DEBUG_NORMAL("Acceleration Finish\n");
            }else{
                USER_DEBUG_NORMAL("Accelerating, Current Value: %f\n", current_output);
            }
            break;
 
        case DECELERATING:
            current_output += acceleration;
            accumulator++;
 
            if (current_output <= (current_target + EPSILON))
            {
                current_output = current_target;
                status = IDLE;
                USER_DEBUG_NORMAL("Deceleration Finish\n");
            }else{
                USER_DEBUG_NORMAL("Decelerating, Current Value: %f\n", current_output);
            }
            break;
 
        case IDLE:
            break;
 
        default:
            break;
    }
 
    return current_output;
}
  