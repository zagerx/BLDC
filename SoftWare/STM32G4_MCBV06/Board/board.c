#include "board.h"
/************************传感器*****************************/
#if 0
#include "sensor.h"
// #include "as5047.h"
#undef NULL
#define NULL 0

static sensor_t sg_sensor_as5047 = {
        .pf_read = as5047_readangle,
        .pf_write = NULL,
        .pf_init = as5047_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};
#endif

/*--------------Actuator--------------------*/
#include "actuator.h"
#include "gpio.h"
#include "debuglog.h"
int led_actutor(actuator_t *pthis)
{
    pthis->cnt++;
    switch (pthis->state)
    {
    case ACT_IDLE :
        if (pthis->cnt > 20)
        {
            pthis->cnt = 0;
            HAL_GPIO_TogglePin(LED01_GPIO_Port,LED01_Pin);
        }
        break;
    case ACT_START:
        break;

    default:
        break;
    }
    return 0;
}

static actuator_t sg_led_actor = {
    .state = ACT_IDLE,
    .pFunc = led_actutor};

void board_init(void)
{
    actuator_mapinit();
    actuator_resgiter((actuator_t*)&sg_led_actor,EVENT_01,0);
}

void board_deinit(void)
{

}
