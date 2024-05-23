#include "board.h"

/*************************电机控制******************************/
#include "motorctrl.h"
#include "tim.h"
static motor_t motor_01 = {
    .name = "motor01",
    .enble = tim_pwm_enable,
    .disable = tim_pwm_disable,
    .setpwm = tim_set_pwm,
}






/************************传感器*****************************/
#include "sensor.h"
#include "mt6816.h"
#undef NULL
#define NULL 0
static sensor_t sensor_mt6816 = {
        .pf_read = mt6816_read,
        .pf_write = NULL,
        .pf_init = mt6816_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};


void board_init(void)
{
    sensor_register(&sensor_mt6816,ANGLE_SENSOR);
}

void board_deinit(void)
{

}
