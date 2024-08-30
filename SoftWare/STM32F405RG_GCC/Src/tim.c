/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim1;

/* TIM1 init function */
void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = _PSC-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED3;
  htim1.Init.Period = _ARR;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 2;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 100;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspInit 0 */

  /* USER CODE END TIM1_MspInit 0 */
    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /* TIM1 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  /* USER CODE BEGIN TIM1_MspInit 1 */

  /* USER CODE END TIM1_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspPostInit 0 */

  /* USER CODE END TIM1_MspPostInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM1 GPIO Configuration
    PB13     ------> TIM1_CH1N
    PB14     ------> TIM1_CH2N
    PB15     ------> TIM1_CH3N
    PA8     ------> TIM1_CH1
    PA9     ------> TIM1_CH2
    PA10     ------> TIM1_CH3
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM1_MspPostInit 1 */

  /* USER CODE END TIM1_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /* TIM1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
static unsigned short max_val_01(unsigned short a,unsigned short b,unsigned short c)
{
	short max;
	if(a>b)
	{
		max = a;
	}else{
		max = b;
	}
	if(c>max)
	{
		max = c;
	}
	return max;
}
static unsigned short min_val_01(unsigned short a,unsigned short b,unsigned short c)
{
  unsigned short min;
  if(a<b)
  {
    min = a;
  }else{
    min = b;
  }
  if (c < min)
  {
    min = c;
  }
  return min;
}

uint16_t a,b,c;
void tim_set_pwm(float _a,float _b,float _c)
{
  a = (uint16_t)((_a)*(_ARR));
  b = (uint16_t)((_b)*(_ARR));
  c = (uint16_t)((_c)*(_ARR));
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,a);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,b);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,c);

  //   unsigned short max = 0;
  //   max = max_val_01((uint16_t)a,(uint16_t)b,(uint16_t)c);
  // __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,(uint16_t)(_ARR>>1));  
}

void tim_pwm_enable(void)
{
  
    HAL_TIM_Base_Start_IT(&htim1);

    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3); 
}
void tim_pwm_disable(void)
{
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_3); 

    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_4);      
}

void tim_tigger_adc(void)
{
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,(uint16_t)(_ARR>>1));  
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
}




// extern ADC_HandleTypeDef hadc2;
// extern ADC_HandleTypeDef hadc3;
// #include "debuglog.h"
// #include "motorctrl.h"
// #define  CURRENT_SENSE_MIN_VOLT           0.3f
// #define  CURRENT_SENSE_MAX_VOLT           3.0f
// #define  CURRENT_ADC_LOWER_BOUND          (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MIN_VOLT / 3.3f)
// #define  CURRENT_ADC_UPPER_BOUND          (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MAX_VOLT / 3.3f)
// #define  SHUNT_RESISTANCE                 0.001f    //采样电阻，如果是0.5mΩ=0.0005f,1mΩ=0.001f
// #define  PHASE_CURRENT_GAIN               20.0f     //电流采样运放倍数，20倍

// static void _convert_current(uint16_t* adc_buf,float *i_abc)
// {
// 	if (adc_buf[1] < CURRENT_ADC_LOWER_BOUND || adc_buf[1] > CURRENT_ADC_UPPER_BOUND ||\
// 		adc_buf[2] < CURRENT_ADC_LOWER_BOUND || adc_buf[2] > CURRENT_ADC_UPPER_BOUND)
// 	{
// 		i_abc[1] = 0.0f;
// 		i_abc[2] = 0.0f;
// 	}else{
// 		i_abc[1]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[1] - (1 << 11)) * (1/PHASE_CURRENT_GAIN)) * (1/SHUNT_RESISTANCE);          //shunt_conductance_ = 1/0.001采样电阻;
// 		i_abc[2]  = ((3.3f / (float)(1 << 12)) * (float)((int)adc_buf[2] - (1 << 11)) * (1/PHASE_CURRENT_GAIN)) * (1/SHUNT_RESISTANCE);
// 		i_abc[0] = -i_abc[1] - i_abc[2];    // i_abc[0] = -i_abc[1]-i_abc[2]
// 	}
// 		i_abc[1]  += 0.2f;
// 		i_abc[2]  -= 0.2f;
// 		i_abc[0]  -= 0.0f;
//     return;
// }


// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
//   static unsigned short adc_vale[3];

//   uint8_t counting_down = TIM1->CR1 & TIM_CR1_DIR;

// 	if(!counting_down)   
// 	{
//     adc_vale[0] = (uint16_t)0;
//     adc_vale[1] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_1);
//     adc_vale[2] = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc3,ADC_INJECTED_RANK_1);
//     float iabc[3] = {0.0f};
//     _convert_current(adc_vale,iabc);    
//     mc_hightfreq_task(iabc);
// 	}
// }


/* USER CODE END 1 */
