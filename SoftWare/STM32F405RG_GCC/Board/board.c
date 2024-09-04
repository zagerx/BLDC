#include "board.h"
#include "initmodule.h"
#include "sensor.h"
#include "as5047.h"

#undef NULL
#define NULL 0

static sensor_t sg_sensor_as5047 = {
        .pf_read = as5047_readangle,
        .pf_write = NULL,
        .pf_init = as5047_init,
        .cycle = 0,
        .status = EN_SENSOR_INIT
};
void user_board_init(void)
{
    sensor_register(&sg_sensor_as5047,SENSOR_01);
}
void board_deinit(void)
{
    
}





#include "debuglog.h"
#include "motorctrl.h"
#include "adc.h"
#define  CURRENT_SENSE_MIN_VOLT           0.3f
#define  CURRENT_SENSE_MAX_VOLT           3.0f
#define  CURRENT_ADC_LOWER_BOUND          (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MIN_VOLT / 3.3f)
#define  CURRENT_ADC_UPPER_BOUND          (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MAX_VOLT / 3.3f)
#define  SHUNT_RESISTANCE                 0.001f    //采样电阻，如果是0.5mΩ=0.0005f,1mΩ=0.001f
#define  PHASE_CURRENT_GAIN               20.0f     //电流采样运放倍数，20倍

static void _convert_current(uint16_t* adc_buf,float *i_abc)
{
	if (adc_buf[1] < CURRENT_ADC_LOWER_BOUND || adc_buf[1] > CURRENT_ADC_UPPER_BOUND ||\
		adc_buf[2] < CURRENT_ADC_LOWER_BOUND || adc_buf[2] > CURRENT_ADC_UPPER_BOUND)
	{
		i_abc[1] = 0.0f;
		i_abc[2] = 0.0f;
	}else{
		i_abc[1]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[1] - (1 << 11)) * (1/PHASE_CURRENT_GAIN)) * (1/SHUNT_RESISTANCE);          //shunt_conductance_ = 1/0.001采样电阻;
		i_abc[2]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[2] - (1 << 11)) * (1/PHASE_CURRENT_GAIN)) * (1/SHUNT_RESISTANCE);
		i_abc[0] = -i_abc[1] - i_abc[2];
	}
		i_abc[1]  += 0.2f;
		i_abc[2]  -= 0.2f;
		i_abc[0]  -= 0.0f;
    return;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 unsigned short adc_vale[3];

  uint8_t counting_down = TIM1->CR1 & TIM_CR1_DIR;

	if(!counting_down)
	{
        adc_vale[0] = (uint16_t)0;
        adc_vale[1] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_1);
        adc_vale[2] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc3,ADC_INJECTED_RANK_1);
        float iabc[3] = {0.0f};
        _convert_current(adc_vale,iabc);
        mc_hightfreq_task(iabc);
	}
}


void user_softresetsystem(void)
{
	HAL_NVIC_SystemReset();
}



board_init(user_board_init)
