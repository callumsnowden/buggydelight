/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include "printf.h"
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t HallPos = 0;

uint8_t step = 0;

uint8_t pwm = 170;

#define PWM_INVERTED

/*
static const uint8_t COMM_TABLE[8][3] = {
		//U, V, W
		{0, 0, 0},
		{0, 0, 1},
		{0, 1, 1},
		{0, 1, 0},
		{1, 1, 0},
		{1, 0, 0},
		{1, 0, 1},
		{1, 1, 1}
};
*/

static const uint8_t COMM_TABLE[8][3] = {
		//U, V, W
		{0, 0, 0},
		{1, 0, 0},
		{1, 1, 0},
		{0, 1, 0},
		{0, 1, 1},
		{0, 0, 1},
		{1, 0, 1},
		{0, 0, 0}
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void UART_putc(void* p, char c);

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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_CAN_Init();
  MX_ADC1_Init();
  HAL_TIMEx_ConfigCommutationEvent_IT(&htim1, TIM_TS_ITR1, TIM_COMMUTATION_TRGI);
  /* USER CODE BEGIN 2 */

  init_printf(NULL, UART_putc);

  DBG_PRINTF("Done peripheral init\r\n");

  TIM1->CCR1 = pwm;
  TIM1->CCR2 = pwm;
  TIM1->CCR3 = pwm;

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

  HAL_TIMEx_HallSensor_Start_IT(&htim2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	  	uint8_t BU = COMM_TABLE[step][0];
	  	uint8_t BV = COMM_TABLE[step][1];
	  	uint8_t BW = COMM_TABLE[step][2];

	  	if(!BU) HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	  	if(!BV) HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	  	if(!BW) HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);

	  	if(BU)
	  	{
	  		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	  	}

	  	if(BV)
	  	{
	  		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	  	}

	  	if(BW)
	  	{
	  		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	  	}

	  	TIM1->CCR1 = pwm;
	  	TIM1->CCR2 = pwm;
	  	TIM1->CCR3 = pwm;

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIMEx_CommutationCallback(TIM_HandleTypeDef *htim)
{
	DBG_PRINTF("Commutation\r\n");

	//Timer commutation callback
	if (htim->Instance == TIM1)
	{
		DBG_PRINTF("Commutation interrupt fired on tim1\r\n");
	}
}

void UART_putc(void* p, char c)
{
	HAL_UART_Transmit(&huart2, &c, 1, 100);
}

void HAL_SYSTICK_Callback()
{

	/*
	step++;

	if(step == 7)
	{
		step = 1;
	}
	*/

	/*
	uint8_t BU = COMM_TABLE[step][0];
	uint8_t BV = COMM_TABLE[step][1];
	uint8_t BW = COMM_TABLE[step][2];

	if(!BU) HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	if(!BV) HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	if(!BW) HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);

	if(BU)
	{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	}

	if(BV)
	{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	}

	if(BW)
	{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	}
	*/

	//step++;

	/*
	TIM1->CCR1 = pwm;
	TIM1->CCR2 = pwm;
	TIM1->CCR3 = pwm;
	*/
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		//DBG_PRINTF("hall timer callback, step %d\r\n", step);


		step++;

		if(step == 7)
		{
			step = 1;
		}


	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
