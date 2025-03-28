/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

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
#define PSC 1
#define _ENCODERLINS 8192
#define _ARR 4200
#define HALL_U1_Pin GPIO_PIN_14
#define HALL_U1_GPIO_Port GPIOC
#define POWER_12V_OFF_Pin GPIO_PIN_3
#define POWER_12V_OFF_GPIO_Port GPIOC
#define SF_DOG_Pin GPIO_PIN_2
#define SF_DOG_GPIO_Port GPIOA
#define POWER_12V_EN_Pin GPIO_PIN_2
#define POWER_12V_EN_GPIO_Port GPIOB
#define LED01_Pin GPIO_PIN_7
#define LED01_GPIO_Port GPIOE
#define HALL_V1_Pin GPIO_PIN_14
#define HALL_V1_GPIO_Port GPIOE
#define HALL_W1_Pin GPIO_PIN_15
#define HALL_W1_GPIO_Port GPIOE
#define VCC5V_OUT_EN_Pin GPIO_PIN_10
#define VCC5V_OUT_EN_GPIO_Port GPIOB
#define SPI3_CS_Pin GPIO_PIN_15
#define SPI3_CS_GPIO_Port GPIOA
#define POWER_MAIN_OFF_Pin GPIO_PIN_7
#define POWER_MAIN_OFF_GPIO_Port GPIOD
#define POWER_3V3_EN_Pin GPIO_PIN_9
#define POWER_3V3_EN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
