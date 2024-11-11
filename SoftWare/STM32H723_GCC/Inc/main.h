/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define _ENCODERLINS 16384
#define _ARR 13750
#define LED_01_Pin GPIO_PIN_3
#define LED_01_GPIO_Port GPIOE
#define EBAKE_PWM_EN_Pin GPIO_PIN_7
#define EBAKE_PWM_EN_GPIO_Port GPIOE
#define VCC3V3_OUT_EN_Pin GPIO_PIN_15
#define VCC3V3_OUT_EN_GPIO_Port GPIOE
#define MAIN_POWER_OFF_Pin GPIO_PIN_11
#define MAIN_POWER_OFF_GPIO_Port GPIOD
#define VCC5V_OUT_EN_Pin GPIO_PIN_3
#define VCC5V_OUT_EN_GPIO_Port GPIOD
#define WATCHDOG_IN_Pin GPIO_PIN_3
#define WATCHDOG_IN_GPIO_Port GPIOB
#define TEST_IO_Pin GPIO_PIN_1
#define TEST_IO_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
