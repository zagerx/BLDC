/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Motor_Control.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern ADC_HandleTypeDef hadc1;
unsigned char KEY_FLAG = 0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static float theta = 0.0f;
float A1,A2,A3;
#define AD_OFFSET 2116
#define t_PWM 0.0001f
#define R_S 0.01F
#define BETA_ 5.7F
float Ia,Ib,Ic;
static unsigned int cnt = 0;

void PWM_EN(void)
{
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);   
  HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
  HAL_ADCEx_InjectedStart_IT(&hadc1);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
}
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
volatile unsigned int AngleIn17bits = 0;
float a_real_theta;
unsigned char Spi_TxData[4]={0x83,0xff,0xff,0xff};///////03 04 05 寄存器存角度
unsigned char Spi_pRxData[4]={0};
float read_angle(void)
{
  /*2*pi/2^14 = 0.00038349f*/
  float theta;
  //Read in Burst mode
  HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_RESET);///CSN LOW   
  HAL_SPI_TransmitReceive(&hspi1, &Spi_TxData[0], &Spi_pRxData[0],0x03,0xffff);
  HAL_GPIO_WritePin(MT68XX_CSN_GPIO_Port, MT68XX_CSN_Pin, GPIO_PIN_SET);///CSN HIGH

  AngleIn17bits =(((Spi_pRxData[1]&0x00ff)<<8)|(Spi_pRxData[2]&0x00fc))>>2;;////14bits 
  theta = AngleIn17bits * 0.00038349f;
  if(theta > 6.28)
    theta = 6.28;
  if (theta < 0)
  {
    theta = 0;
  }
  
  return (theta);	    
}
float angle_offset = 0;
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  float a1,a2,a3;
  a1 = (float)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1) - AD_OFFSET;
  a2 = (float)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2) - AD_OFFSET;
  a3 = (float)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_3) - AD_OFFSET;

  A1 = a1;A2 = a2;A3 = a3;

	Ia=(float)A1*(float)(3.3f/4096.0f/R_S/BETA_);//0.01413446
	Ib=(float)A2*(float)(3.3f/4096.0f/R_S/BETA_);
	Ic=(float)A3*(float)(3.3f/4096.0f/R_S/BETA_);	
  Ia /= 1.2f;
  Ic /= 1.3f;

  // rtU.Bus_voltage = 24.0f;
  if(cnt>=20)
  {
    rtU.speed_ref = 2500;
  }else{
    rtU.speed_ref = 2000;
  }
  
  rtU.abc[0] = Ia;
  rtU.abc[1] = Ib;
  rtU.abc[2] = Ic;
  HAL_GPIO_WritePin(LED_01_GPIO_Port,LED_01_Pin,GPIO_PIN_SET);
  /*about 12us*/
  a_real_theta = read_angle();
  a_real_theta -= angle_offset;
  HAL_GPIO_WritePin(LED_01_GPIO_Port,LED_01_Pin,GPIO_PIN_RESET);
  a_real_theta *= 2;
  if(a_real_theta >= 6.28)
  {
    a_real_theta -= 6.28;
  }
  rtU.test_angle = a_real_theta;
  if(KEY_FLAG)
  {

    Motor_Control_step();

    float a,b,c;
    a = ((1.0f-(float)rtY.Ta)*_ARR);
    b = ((1.0f-(float)rtY.Tb)*_ARR);
    c = ((1.0f-(float)rtY.Tc)*_ARR);  
    unsigned short max;
    max = max_val_01((uint16_t)a,(uint16_t)b,(uint16_t)c);	
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,(uint16_t)c);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,(uint16_t)b);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,(uint16_t)a);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,(uint16_t)(max + 100));	
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  KEY_FLAG = 1;
  PWM_EN();
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(LED_01_GPIO_Port,LED_01_Pin,GPIO_PIN_SET);
  Motor_Control_initialize();
  angle_offset = read_angle();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // HAL_GPIO_TogglePin(LED_01_GPIO_Port,LED_01_Pin);
    
    cnt++;
    HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 21;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
