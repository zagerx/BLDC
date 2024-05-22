#include "./motorctrl.h"
#include "debuglog.h"
#include "motorctrl_common.h"
#include "mc_protocol.h"
#include "board.h"
#include "string.h"
#include "mc_torquemode.h"
#include "mc_currmentmode.h"
#include "mc_utils.h"
#include "mc_angle.h"
#define  CURRMENT_MEAS_PERIOD             0.000125f
enum{
    MOTOR_INIT,
    MOTOR_START, 
    MOTOR_RUNING,
    MOTOR_STOP,
};

static void mc_param_init(void);
static void mc_param_deinit(void);
static void trigger_software_reset(void);
static void motor_enable(void);
static void motor_disable(void);
static void motor_set_pwm(duty_t temp);

mc_param_t mc_param = {0};
motordebug_t motordebug = {0};

void motortctrl_process(void)
{
    static uint16_t _state = MOTOR_INIT;

    motorprotocol_process();
    switch (_state)
    {
    case MOTOR_INIT:
        if(strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_START].cmd)))
        {
            break;
        }
        mc_param_init();
        _state = MOTOR_START;
    case MOTOR_START:
        motor_enable();
        _state = MOTOR_RUNING;
        motorprotocol_transmit(sg_commandmap[CMD_SET_START].res_cmd,strlen(sg_commandmap[CMD_SET_START].res_cmd));
        break;

    case MOTOR_RUNING:
        {
            dq_t idq = mc_update_torque(0.05f);//更新iq限制值
            motordebug.id_targe = idq.d;
            motordebug.iq_targe = idq.q;
        }
        if(strcmp(motordebug.cur_cmd,(sg_commandmap[CMD_SET_START].cmd)))
        {
            motorprotocol_transmit(sg_commandmap[CMD_SET_STOP].res_cmd,strlen(sg_commandmap[CMD_SET_STOP].res_cmd));
            _state = MOTOR_STOP;
        }
        break;

    case MOTOR_STOP:
        motor_disable();
        mc_param_deinit();
        _state = MOTOR_INIT;
        break;
    default:
        break;
    }
}

void _mc_param_init(mc_param_t *motor_x)
{
    /*电流环 参数初始化*/

    /*速度环 参数初始化*/

}

void mc_hightfreq_task(uint16_t a,uint16_t b,uint16_t c)
{
    duty_t duty = {0};
    /*获取角度 速度*/
    int32_t raw = ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_RAW))[0];
	float theta = 0.0f;
    float next_theta = 0.0f;
    float speed = 0.0f;
    mc_encoder_readspeedangle(&raw,&theta,&speed);
    next_theta = theta + 1.5f * CURRMENT_MEAS_PERIOD * speed;

    duty = currment_loop(a,b,c,theta,next_theta);
    motor_set_pwm(duty);
}


static void mc_param_init(void)
{
    pid_init(&(mc_param.daxis_pi),0.0273f,166.2507f * 0.000125f,1.0f,D_MAX_VAL,D_MIN_VAL);
    pid_init(&(mc_param.qaxis_pi),0.0273f,166.2507f * 0.000125f,1.0f,D_MAX_VAL,D_MIN_VAL);


    // pid_init(&(mc_param.speedloop_pi),1.0f,0.0f,1.0f,D_MAX_VAL,D_MIN_VAL);
    // lowfilter_init(&(mc_param.elefitler[0]),80);
    // lowfilter_init(&(mc_param.elefitler[1]),80);
    // lowfilter_init(&(mc_param.elefitler[2]),80);
}

static void mc_param_deinit(void)
{
    memset(&mc_param,0,sizeof(mc_param));
    memset(&motordebug,0,sizeof(motordebug));
}

static void motor_enable(void)
{
#ifdef BOARD_STM32H723
    gpio_setmotor_power();
#endif
    /*pwm 使能*/
    tim_pwm_enable();
    tim_tigger_adc();
    adc_start();
}
static void motor_enable_noirq(void)
{
#ifdef BOARD_STM32H723
    gpio_setmotor_power();
#endif
    /*pwm 使能*/
    tim_pwm_enable();
}
static void motor_disable(void)
{
#ifdef BOARD_STM32H723
    gpio_setmotor_powerdown();
#endif    
    tim_pwm_disable();
    adc_stop();
}
static void motor_set_pwm(duty_t temp)
{
    tim_set_pwm(temp._a ,temp._b,temp._c);
}
static void trigger_software_reset(void)
{
    HAL_NVIC_SystemReset();
}
