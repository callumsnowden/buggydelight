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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

int8_t PWM = 0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t sineTable[3][360] = {{50,51,52,53,53,54,55,56,57,58,59,60,60,61,62,63,64,65,65,66,67,68,69,70,70,71,72,73,73,74,75,76,76,77,78,79,79,80,81,81,82,83,83,84,85,85,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,94,94,95,95,95,96,96,96,97,97,97,98,98,98,98,99,99,99,99,99,99,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,99,99,99,99,99,99,98,98,98,98,97,97,97,96,96,96,95,95,95,94,94,93,93,92,92,91,91,90,90,89,89,88,88,87,87,86,85,85,84,83,83,82,81,81,80,79,79,78,77,76,76,75,74,73,73,72,71,70,70,69,68,67,66,65,65,64,63,62,61,60,60,59,58,57,56,55,54,53,53,52,51,50,49,48,47,47,46,45,44,43,42,41,40,40,39,38,37,36,35,35,34,33,32,31,30,30,29,28,27,27,26,25,24,24,23,22,21,21,20,19,19,18,17,17,16,15,15,14,13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,5,4,4,4,3,3,3,2,2,2,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,3,3,3,4,4,4,5,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,15,15,16,17,17,18,19,19,20,21,21,22,23,24,24,25,26,27,27,28,29,30,30,31,32,33,34,35,35,36,37,38,39,40,40,41,42,43,44,45,46,47,47,48,49}, {6,6,5,5,5,4,4,4,3,3,3,2,2,2,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,3,3,3,4,4,4,5,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,15,15,16,17,17,18,19,19,20,21,21,22,23,24,24,25,26,27,27,28,29,30,30,31,32,33,34,35,35,36,37,38,39,40,40,41,42,43,44,45,46,47,47,48,49,50,51,52,53,53,54,55,56,57,58,59,60,60,61,62,63,64,65,65,66,67,68,69,70,70,71,72,73,73,74,75,76,76,77,78,79,79,80,81,81,82,83,83,84,85,85,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,94,94,95,95,95,96,96,96,97,97,97,98,98,98,98,99,99,99,99,99,99,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,99,99,99,99,99,99,98,98,98,98,97,97,97,96,96,96,95,95,95,94,94,93,93,92,92,91,91,90,90,89,89,88,88,87,87,86,85,85,84,83,83,82,81,81,80,79,79,78,77,76,76,75,74,73,73,72,71,70,70,69,68,67,66,65,65,64,63,62,61,60,60,59,58,57,56,55,54,53,53,52,51,50,49,48,47,47,46,45,44,43,42,41,40,40,39,38,37,36,35,35,34,33,32,31,30,30,29,28,27,27,26,25,24,24,23,22,21,21,20,19,19,18,17,17,16,15,15,14,13,13,12,12,11,11,10,10,9,9,8,8,7,7}, {93,92,92,91,91,90,90,89,89,88,88,87,87,86,85,85,84,83,83,82,81,81,80,79,79,78,77,76,76,75,74,73,73,72,71,70,70,69,68,67,66,65,65,64,63,62,61,60,60,59,58,57,56,55,54,53,53,52,51,50,49,48,47,47,46,45,44,43,42,41,40,40,39,38,37,36,35,35,34,33,32,31,30,30,29,28,27,27,26,25,24,24,23,22,21,21,20,19,19,18,17,17,16,15,15,14,13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,5,4,4,4,3,3,3,2,2,2,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,3,3,3,4,4,4,5,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,15,15,16,17,17,18,19,19,20,21,21,22,23,24,24,25,26,27,27,28,29,30,30,31,32,33,34,35,35,36,37,38,39,40,40,41,42,43,44,45,46,47,47,48,49,50,51,52,53,53,54,55,56,57,58,59,60,60,61,62,63,64,65,65,66,67,68,69,70,70,71,72,73,73,74,75,76,76,77,78,79,79,80,81,81,82,83,83,84,85,85,86,87,87,88,88,89,89,90,90,91,91,92,92,93,93,94,94,95,95,95,96,96,96,97,97,97,98,98,98,98,99,99,99,99,99,99,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,99,99,99,99,99,99,98,98,98,98,97,97,97,96,96,96,95,95,95,94,94,93}};

uint16_t sinePos = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
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
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_CAN_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  /*
   * TIM_CHANNEL_1 / CCR1 is U
   * TIM_CHANNEL_2 / CCR2 is V
   * TIM_CHANNEL_3 / CCR3 is W
   */

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

  HAL_GPIO_WritePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin, GPIO_PIN_SET);

  TIM1->CCR1 = 50;
  TIM1->CCR2 = 50;
  TIM1->CCR3 = 50;

  PWM = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  /*
	  if(sinePos > 359) sinePos = 0;
	  TIM1->CCR1 = sineTable[0][sinePos];
	  TIM1->CCR2 = sineTable[1][sinePos];
	  TIM1->CCR3 = sineTable[2][sinePos];
	  sinePos++;
	  HAL_Delay(20);
	  */

	  for(int8_t i = 0; i < 5; i++)
	  {
		  PWM += 10;
		  TIM1->CCR2 = 50 + PWM;
		  TIM1->CCR3 = 50 - PWM;
		  HAL_Delay(100);
	  }
	  HAL_Delay(2000);
	  for(int8_t i = 0; i < 10; i++)
	  {
		  PWM -= 10;
	  	  TIM1->CCR2 = 50 + PWM;
	  	  TIM1->CCR3 = 50 - PWM;
	  	  HAL_Delay(100);
	  }
	  HAL_Delay(2000);
	  for(int8_t i = 0; i < 5; i++)
	  {
		  PWM += 10;
		  TIM1->CCR2 = 50 + PWM;
		  TIM1->CCR3 = 50 - PWM;
		  HAL_Delay(100);
	  }
	  HAL_Delay(2000);


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM1_TRG_COM_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_TRG_COM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);
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
