/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim1;

extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim4;

extern TIM_HandleTypeDef htim8;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);
void MX_TIM8_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
void tim8_tigger_adc(void);
void tim8_pwm_disable(void);
void tim8_pwm_enable(void);
void tim8_set_pwm(float _a,float _b,float _c);

void tim3_abzencoder_enable(void);          
uint32_t tim3_abzencoder_getcount(void);    
void tim3_abzencoder_setcount(uint32_t cnt);
void tim3_abzencoder_disable(void);  

void tim1_tigger_adc(void);
void tim1_set_pwm(float _a,float _b,float _c);
void tim1_pwm_enable(void);
void tim1_pwm_disable(void);
void tim4_abzencoder_enable(void);          
uint32_t tim4_abzencoder_getcount(void);    
void tim4_abzencoder_setcount(uint32_t cnt);
void tim4_abzencoder_disable(void);  
void timx_enable(void);
void timx_disable(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

