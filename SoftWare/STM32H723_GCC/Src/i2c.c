/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */
#include "_common.h"

static uint8_t i2cx_RxBuf[2] = {0};


void user_i2cirq_cb(I2C_HandleTypeDef *hi2c);

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c2;

/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x60404E72;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */
  /* USER CODE END I2C2_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
    PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C1235CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();

    /* I2C2 interrupt Init */
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_SetPriority(I2C2_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

    /* I2C2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
uint16_t ID_TEST = 0;

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  ID_TEST = (uint16_t)i2cx_RxBuf[0]<<8 | i2cx_RxBuf[1];
  USER_DEBUG_NORMAL("i2x Rx cb 0x%04x\r\n",ID_TEST);
  // HAL_I2C_Mem_Read_IT(&hi2c2,0x0080,0xFF00,I2C_MEMADD_SIZE_16BIT,&i2cx_RxBuf[0],2);

}
void i2c2_init(void)
{
  uint8_t pData[2];
  uint8_t buf[1];
  buf[0] = 0xFF;
  HAL_I2C_Mem_Read_IT(&hi2c2,0x0080,0xFF00,I2C_MEMADD_SIZE_16BIT,&i2cx_RxBuf[0],2);
  // HAL_Delay(1);
  // HAL_I2C_Mem_Read_IT(&hi2c2,0x0080,0x0000,I2C_MEMADD_SIZE_16BIT,&i2cx_RxBuf[0],2);
  // HAL_Delay(1);
  // HAL_I2C_Mem_Read_IT(&hi2c2,0x0080,0x0500,I2C_MEMADD_SIZE_16BIT,&i2cx_RxBuf[0],2);  
  // HAL_Delay(1);
  // HAL_I2C_Mem_Read_IT(&hi2c2,0x0080,0x0000,I2C_MEMADD_SIZE_16BIT,&i2cx_RxBuf[0],2);
  return;
  // USER_DEBUG_NORMAL("I2C2 CR1 0x%08x  ",READ_REG((I2C_HandleTypeDef *)(&hi2c2)->Instance->CR1));
  // USER_DEBUG_NORMAL("I2C2 CR2 0x%08x  ",READ_REG((I2C_HandleTypeDef *)(&hi2c2)->Instance->CR2));
  // USER_DEBUG_NORMAL("I2C2 ISR 0x%08x  ",READ_REG((I2C_HandleTypeDef *)(&hi2c2)->Instance->ISR));
  // USER_DEBUG_NORMAL("I2C2 ICR 0x%08x  \r\n",READ_REG((I2C_HandleTypeDef *)(&hi2c2)->Instance->ICR));
  USER_DEBUG_NORMAL("I2C2 CR1 0x%08x  ",READ_REG((I2C_HandleTypeDef *)(&hi2c2)->Instance->CR1));  
  USER_DEBUG_NORMAL("I2C2 CR2 0x%08X  ",READ_REG((I2C_HandleTypeDef *)(&hi2c2)->Instance->CR2));
  USER_DEBUG_NORMAL("I2C2 ISR 0x%08x  ",READ_REG((I2C_HandleTypeDef *)(&hi2c2)->Instance->ISR));
  USER_DEBUG_NORMAL("I2C2 ICR 0x%08x  \r\n",READ_REG((I2C_HandleTypeDef *)(&hi2c2)->Instance->ICR));
}

void i2c2_read(uint16_t DevAddress, uint8_t register_addr, uint8_t *pData, uint16_t Size)
{
  uint8_t buf[1];
  buf[0] = register_addr;
  HAL_I2C_Master_Transmit(&hi2c2,DevAddress, buf,1, 0xFF);
  HAL_I2C_Master_Receive(&hi2c2,DevAddress,&pData[0],2,0xFF);
}
void i2c2_write(uint16_t DevAddress, uint8_t register_addr,uint8_t *pData, uint16_t Size)
{
  static uint8_t SentTable[3];
  SentTable[0] = register_addr;
  SentTable[1] = (pData[0]);
  SentTable[2] = (pData[1]);
  HAL_I2C_Master_Transmit(&hi2c2, 0x80, SentTable, sizeof(SentTable), 0xFF);
}
/* USER CODE END 1 */
