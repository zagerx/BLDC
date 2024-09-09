#include "board.h"
#include "initmodule.h"
#include "debuglog.h"
/************************传感器*****************************/
#include "sensor.h"
#undef NULL
#define NULL 0

#include "as5047.h"
static sensor_t sg_sensor_as5047 = {
        .pf_read = as5047_readangle,
        .pf_write = NULL,
        .pf_init = as5047_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};
#include "mt6816.h"
static sensor_t sg_sensor_mt6816 = {
        .pf_read = mt6816_read,
        .pf_write = NULL,
        .pf_init = mt6816_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};
#include "adc.h"
static sensor_t sg_sensor_vbus = {
        .pf_read = adc_readvbus,
        .pf_write = NULL,
        .pf_init = adc_vbusinit,
        .cycle = 2,
        .status = EN_SENSOR_INIT
};
void user_board_init(void)
{
    sensor_register(&sg_sensor_as5047,SENSOR_01);
    sensor_register(&sg_sensor_vbus,SENSOR_02);
}
void board_deinit(void)
{

}

#include "usart.h"
#include "string.h"
void _bsp_protransmit(unsigned char* pdata,unsigned short len)
{
    static unsigned char sg_uartsend_buf[125];
    memcpy(sg_uartsend_buf,pdata,len);
    HAL_UART_Transmit_DMA(&huart1,sg_uartsend_buf,len);
}

#include "tim.h"
void motor_enable(void)
{
    tim_pwm_enable();
    tim_tigger_adc();
    adc_start();
}
void motor_disable(void)
{
    tim_pwm_disable();
    adc_stop();
}
void motor_set_pwm(float _a,float _b,float _c)
{
    tim_set_pwm(_a ,_b,_c);
}

#include "motorctrl.h"
#define  CURRENT_SENSE_MIN_VOLT           0.3f
#define  CURRENT_SENSE_MAX_VOLT           3.0f
#define  CURRENT_ADC_LOWER_BOUND          (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MIN_VOLT / 3.3f)
#define  CURRENT_ADC_UPPER_BOUND          (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MAX_VOLT / 3.3f)

static void _convert_current(uint16_t* adc_buf,float *i_abc)
{
#if 0    
	if (adc_buf[1] < CURRENT_ADC_LOWER_BOUND || adc_buf[1] > CURRENT_ADC_UPPER_BOUND ||\
		adc_buf[2] < CURRENT_ADC_LOWER_BOUND || adc_buf[2] > CURRENT_ADC_UPPER_BOUND)
	{
		i_abc[1] = 0.0f;
		i_abc[2] = 0.0f;
	}else{
		i_abc[1]  = -((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[1] - (1 << 11)) * (1/5.7f)) * (1/0.025f);          //shunt_conductance_ = 1/0.001采样电阻;
		i_abc[2]  = -((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[2] - (1 << 11)) * (1/5.7f)) * (1/0.025f);
	}
  i_abc[0] = (-i_abc[1] - i_abc[2]);

  i_abc[0] = i_abc[0] - 1.0f;
    return;
#else
    i_abc[0]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[0] - (1 << 11)) * (1/5.7f)) * (1/0.025f); 
    i_abc[1]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[1] - (1 << 11)) * (1/5.7f)) * (1/0.025f);          //shunt_conductance_ = 1/0.001采样电阻;
    i_abc[2]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[2] - (1 << 11)) * (1/5.7f)) * (1/0.025f);

#endif

}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  unsigned short adc_vale[3];
  float iabc[3]; 
	{
    adc_vale[1] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1);
    adc_vale[2] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2);
    adc_vale[0] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_3);
    _convert_current((uint16_t*)adc_vale,iabc);


    
    iabc[0] = -iabc[0];
    iabc[1] = -iabc[1];
    iabc[2] = -iabc[2];
    mc_hightfreq_task(iabc);
	}
}



void user_softresetsystem(void)
{
	HAL_NVIC_SystemReset();
}
board_init(user_board_init)
