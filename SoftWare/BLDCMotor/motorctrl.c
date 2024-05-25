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

mc_param_t mc_param = {0};
motordebug_t motordebug = {0};

static void mc_param_init(void);
static void mc_param_deinit(void);

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

void mc_hightfreq_task(float *iabc)
{
    duty_t duty = {0};
    /*获取角度 速度*/
    int32_t raw = ((int32_t*)sensor_user_read(SENSOR_01,EN_SENSORDATA_RAW))[0];
	float theta = 0.0f;
    float next_theta = 0.0f;
    float speed = 0.0f;
    mc_encoder_readspeedangle(&raw,&theta,&speed);
    next_theta = theta + 1.5f * CURRMENT_MEAS_PERIOD * speed;
    motordebug.ele_angle = theta;
    duty = currment_loop(iabc,theta,next_theta);
    motor_set_pwm(duty._a,duty._b,duty._c);
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



motor_t* motor_table[2] = {0};
void motor_register(motor_t *motor)
{
    motor_table[0] = motor;
}
