/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "string.h"
#include "device.h"
#include <stdint.h>
static uint8_t sg_uartreceive_buff[125];
/* USER CODE END 0 */

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;

/* USART3 init function */

void MX_USART3_UART_Init(void)
{

	/* USER CODE BEGIN USART3_Init 0 */

	/* USER CODE END USART3_Init 0 */

	/* USER CODE BEGIN USART3_Init 1 */

	/* USER CODE END USART3_Init 1 */
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart3, (uint8_t *)sg_uartreceive_buff, sizeof(sg_uartreceive_buff));
	/* USER CODE END USART3_Init 2 */
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	if (uartHandle->Instance == USART3) {
		/* USER CODE BEGIN USART3_MspInit 0 */

		/* USER CODE END USART3_MspInit 0 */

		/** Initializes the peripherals clocks
		 */
		PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
		PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
			Error_Handler();
		}

		/* USART3 clock enable */
		__HAL_RCC_USART3_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**USART3 GPIO Configuration
		PB10     ------> USART3_TX
		PB11     ------> USART3_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USART3 DMA Init */
		/* USART3_RX Init */
		hdma_usart3_rx.Instance = DMA1_Channel1;
		hdma_usart3_rx.Init.Request = DMA_REQUEST_USART3_RX;
		hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart3_rx.Init.Mode = DMA_NORMAL;
		hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
		if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK) {
			Error_Handler();
		}

		__HAL_LINKDMA(uartHandle, hdmarx, hdma_usart3_rx);

		/* USART3 interrupt Init */
		HAL_NVIC_SetPriority(USART3_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
		/* USER CODE BEGIN USART3_MspInit 1 */

		/* USER CODE END USART3_MspInit 1 */
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

	if (uartHandle->Instance == USART3) {
		/* USER CODE BEGIN USART3_MspDeInit 0 */

		/* USER CODE END USART3_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART3_CLK_DISABLE();

		/**USART3 GPIO Configuration
		PB10     ------> USART3_TX
		PB11     ------> USART3_RX
		*/
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10 | GPIO_PIN_11);

		/* USART3 DMA DeInit */
		HAL_DMA_DeInit(uartHandle->hdmarx);

		/* USART3 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART3_IRQn);
		/* USER CODE BEGIN USART3_MspDeInit 1 */

		/* USER CODE END USART3_MspDeInit 1 */
	}
}

/* USER CODE BEGIN 1 */
#include "stdio.h"
int _write(int file, char *data, int len)
{
	HAL_StatusTypeDef status = HAL_UART_Transmit(&huart3, (uint8_t *)data, len, 1000);
	return (status == HAL_OK ? len : 0);
}
// void _bsp_protransmit(unsigned char* pdata,unsigned short len)
// {
//     static unsigned char sg_uartsend_buf[125];
//     memcpy(sg_uartsend_buf,pdata,len);
//     HAL_UART_Transmit_DMA(&huart3,sg_uartsend_buf,len);
// }
void process_data(uint8_t *data, uint16_t len);

void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
	if (USART3 == huart->Instance) {
		if (RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE)) {
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			HAL_UART_DMAStop(huart);
			unsigned short data_length =
				sizeof(sg_uartreceive_buff) - __HAL_DMA_GET_COUNTER(huart->hdmarx);
			process_data(sg_uartreceive_buff, data_length);
			// memset(sg_uartreceive_buff, 0, 125);

			// data_length = 0;
			HAL_UART_Receive_DMA(huart, (uint8_t *)sg_uartreceive_buff,
					     sizeof(sg_uartreceive_buff));
		}
	}
}
#include <string.h>
#include <stdlib.h>
#include "foc_parameters.h"
void debug_update_foc_data(float *input, enum foc_parameters_index flag);
#if 0
void process_data(uint8_t *data, uint16_t len)
{
	if (data[0] == 0) {
		return;
	}
	char buf[64];
	memcpy(buf, data, len);
	buf[len] = '\0';

	// 找冒号
	char *p = strchr(buf, ':');
	if (!p) {
		return;
	}

	*p = '\0';             // 将命令名与参数分开
	char *cmd = buf;       // 命令名
	char *val_str = p + 1; // 参数字符串
	float value = atof(val_str);

	// ========== 命令判断 ==========
	float input[2];
	if (strcmp(cmd, "D_Kp") == 0) {
		input[0] = value;
		input[1] = 20.00f;
		debug_update_foc_data(input, INDEX_D_PI);
		return;
	}
	if (strcmp(cmd, "D_Ki") == 0) {
		input[0] = 0.1f; // 需要根据调试好的Kp写入
		input[1] = value;
		debug_update_foc_data(input, INDEX_D_PI);
		return;
	}

	if (strcmp(cmd, "Q_Kp") == 0) {
		printf("IQ Command, value = %.3f\r\n", value);
		return;
	}
	if (strcmp(cmd, "Q_Ki") == 0) {
		printf("IQ Command, value = %.3f\r\n", value);
		return;
	}
	if (strcmp(cmd, "SpeedKp") == 0) {
		input[0] = value;
		input[1] = 1.01f;
		debug_update_foc_data(input, INDEX_VELOCITY_PI);
		return;
	}
	if (strcmp(cmd, "SpeedKi") == 0) {
		// input[0] = ;
		input[1] = value;
		debug_update_foc_data(input, INDEX_VELOCITY_PI);
		return;
	}
	if (strcmp(cmd, "set_dq_Ref") == 0) {
		input[0] = value;
		debug_update_foc_data(input, INDEX_ID_REF);
		HAL_GPIO_TogglePin(LED02_GPIO_Port, LED02_Pin);
		return;
	}

	if (strcmp(cmd, "set_speed_Ref") == 0) {
		input[0] = value;
		debug_update_foc_data(input, INDEX_VELOCITY_REG);
		HAL_GPIO_TogglePin(LED02_GPIO_Port, LED02_Pin);
		return;
	}
}
#else
// 命令映射表结构
typedef struct {
    const char *cmd_name;
    uint8_t min_params;  // 最少需要的参数个数
    enum foc_parameters_index data_index;
} command_map_t;

// 命令表定义
static const command_map_t cmd_map[] = {
    {"D_PI", 2, INDEX_D_PI},
    {"D_ref", 2, 	INDEX_ID_REF},

    {"Velocity_PI", 2, INDEX_VELOCITY_PI},
    {"Valocity_tar", 2, INDEX_VELOCITY_REG},
};
void process_data(uint8_t *data, uint16_t len)
{
    if (data[0] == 0 || len == 0 || len > 255) {
        return;
    }
    
    char buf[256];
    uint16_t copy_len = (len < sizeof(buf) - 1) ? len : sizeof(buf) - 1;
    memcpy(buf, (char *)data, copy_len);
    buf[copy_len] = '\0';

    // 找冒号
    char *colon_pos = strchr(buf, ':');
    if (!colon_pos) {
        return;
    }

    *colon_pos = '\0';
    char *cmd = buf;
    char *params_str = colon_pos + 1;

    // ========== 高效参数解析 ==========
    float params[10];
    uint8_t param_count = 0;
    char *ptr = params_str;
    
    while (*ptr != '\0' && param_count < 10) {
        // 跳过空格
        while (*ptr == ' ') ptr++;
        if (*ptr == '\0') break;
        
        // 解析浮点数
        char *end;
        params[param_count] = strtof(ptr, &end);
        
        if (end == ptr) {
            // 转换失败，跳过这个字段
            while (*end != ',' && *end != '\0') end++;
        } else {
            param_count++;
        }
        
        // 移动到下一个参数
        ptr = end;
        if (*ptr == ',') {
            ptr++;
        } else {
            break;
        }
    }

    if (param_count == 0) {
        return;
    }

    for (size_t i = 0; i < sizeof(cmd_map) / sizeof(cmd_map[0]); i++) {
        if (strcmp(cmd, cmd_map[i].cmd_name) == 0) {
            if (param_count >= cmd_map[i].min_params) {
                float input[10];
                uint8_t copy_count = (param_count < 10) ? param_count : 10;
                
                for (int j = 0; j < copy_count; j++) {
                    input[j] = params[j];
                }
                
                debug_update_foc_data(input, cmd_map[i].data_index);
            }
            return;
        }
    }
}
#endif
/* USER CODE END 1 */
