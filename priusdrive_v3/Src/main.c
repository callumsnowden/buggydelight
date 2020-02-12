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


/*
 * ADC PIN ASSIGNMENTS
 *
 * IN4  / 0: Control current sense
 * IN5  / 1: Motor current sense
 * IN8  / 2: Drive temperature
 * IN9  / 3: Motor temperature
 * IN10 / 4: Throttle 0
 * IN11 / 5: Throttle 1
 * IN12 / 6: Coolant in temperature
 * IN13 / 7: Coolant out temperature
 * IN14 / 8: DC Link voltage
 * IN15 / 9: System voltage
 */

/*
 * Timer Assignments
 *
 * TIM1: Motor PWM
 * TIM2: Hall effects input
 * TIM3: Cooling PWM
 * TIM4: Boost converter PWM
 */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "iwdg.h"
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

#define COMMAND_ADDR 0x01
#define CONFIG_ADDR 0x02
//#define MULTI_THROTTLE_MODE

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

volatile uint16_t ThrottleMin = 700;
volatile uint16_t ThrottleMax = 3450;

volatile uint8_t ForwardReverseSelect = 0;
volatile uint8_t InverterEnable = 0;

volatile uint8_t ThrottleCommand = 0;
volatile uint8_t LastThrottleCommand = 0;
volatile uint8_t CANThrottleCommand = 0;

volatile uint8_t ThrottleDisagreement = 0;
volatile uint8_t ThrottleRampStep = 1;
volatile uint8_t ThrottleRampTime = 5;
volatile uint8_t ThrottleRunning = 0;

volatile uint8_t MaxFanSpeed = 0;
volatile uint8_t MaxPumpSpeed = 0;
volatile uint8_t MaxMotorCurrent = 0;
volatile uint8_t MaxMotorTemp = 0;
volatile uint8_t MaxCoolantTemp = 0;

volatile uint8_t MaxBoostVoltage = 0;
volatile uint8_t BoostCommand = 0;

volatile uint16_t ThrottleAveraging[10] = {0};
volatile uint16_t DCLinkAveraging[10] = {0};

static float DCLinkRatio = 0.0262901655;
volatile float DCLinkVoltage = 0;

volatile uint8_t Error = 0;

volatile uint16_t LoopCount = 0;


/*
 * Array values
 * 0: CONTROL_CURRENT_SENSE
 * 1: MOTOR_CURRENT_SENSE
 * 2: DRIVE_TEMPERATURE
 * 3: MOTOR_TEMPERATURE
 * 4: THROTTLE_0
 * 5: THROTTLE_1
 * 6: COOLANT_IN_TEMP
 * 7: COOLANT_OUT_TEMP
 * 8: DC_LINK_VOLTAGE
 * 9: SYSTEM_VOLTAGE
 *
 */

volatile uint16_t ADCRawValues[10];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

void AddValue(uint16_t newValue, uint16_t array[], uint8_t len);
uint16_t CalculateArrayAverage(uint16_t array[], uint8_t len);
uint8_t ScaleToPercentage(uint16_t input, uint16_t in_min, uint16_t in_max);
void CheckSafeties();

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_CAN_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  hcan.Instance->MCR = 0x60;

  __enable_irq();

  HAL_CAN_Receive_IT(&hcan, CAN_FIFO0);
  //HAL_ADC_Start_IT(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADCRawValues, 10);

  /*
   * TIM_CHANNEL_1 / CCR1 is U
   * TIM_CHANNEL_2 / CCR2 is V
   * TIM_CHANNEL_3 / CCR3 is W
   */

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

  HAL_GPIO_WritePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(BOOST_ENABLE_GPIO_Port, BOOST_ENABLE_Pin, GPIO_PIN_RESET);

  TIM1->CCR2 = 100;
  TIM1->CCR3 = 100;
  TIM4->CCR1 = 0;

  HAL_GPIO_WritePin(GPIOC, OK_LED_Pin, GPIO_PIN_SET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(Error == 0)
	  {
		  HAL_GPIO_WritePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOD, DIAG_LED_1_Pin, GPIO_PIN_RESET);

		  //Set throttle
		  if(ForwardReverseSelect == 1)
		  {
			  //Check to see if throttle has been released to decay throttle
			  if(ThrottleCommand < LastThrottleCommand && LoopCount % ThrottleRampTime == 0)
			  {
				  TIM1->CCR2 = 100 + ThrottleCommand - ThrottleRampStep;
				  TIM1->CCR3 = 100 - ThrottleCommand - ThrottleRampStep;
				  LastThrottleCommand = LastThrottleCommand - ThrottleRampStep;
			  } else if(ThrottleCommand > LastThrottleCommand){
				  TIM1->CCR2 = 100 + ThrottleCommand;
				  TIM1->CCR3 = 100 - ThrottleCommand;
				  LastThrottleCommand = ThrottleCommand;
			  }
		  } else if(ForwardReverseSelect == 0)
		  {
			  //Check to see if throttle has been released to decay throttle
			  if(ThrottleCommand < LastThrottleCommand && LoopCount % ThrottleRampTime == 0)
			  {
				  TIM1->CCR2 = 100 - ThrottleCommand - ThrottleRampStep;
				  TIM1->CCR3 = 100 + ThrottleCommand - ThrottleRampStep;
				  LastThrottleCommand = LastThrottleCommand - ThrottleRampStep;
			  } else if(ThrottleCommand > LastThrottleCommand){
				  TIM1->CCR2 = 100 - ThrottleCommand;
				  TIM1->CCR3 = 100 + ThrottleCommand;
				  LastThrottleCommand = ThrottleCommand;
			  }
		  }

		  if(ThrottleCommand > 15)
		  {
			  ThrottleRunning = 1;
			  TIM4->CCR1 = BoostCommand;
		  }
		  if(ThrottleCommand < 14)
		  {
			  ThrottleRunning = 0;
			  TIM4->CCR1 = 0;
		  }
	  }

	  if(Error == 1)
	  {
		  TIM1->CCR2 = 0;
		  TIM1->CCR3 = 0;
		  TIM4->CCR1 = 0;

		  HAL_GPIO_WritePin(GPIOD, DIAG_LED_1_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin, GPIO_PIN_RESET);
	  }

	  if(LoopCount == 1000)
	  {
		  Error = 0;
		  LoopCount = 0;
	  }

	  if(LoopCount % 100 == 0)
	  {
		  hcan.pTxMsg->DLC = 3;
		  hcan.pTxMsg->Data[0] = (int)LoopCount / 100;
		  hcan.pTxMsg->Data[1] = ThrottleCommand; //ScaleToPercentage(ADCRawValues[4], ThrottleMin, ThrottleMax);
		  hcan.pTxMsg->Data[2] = (int)DCLinkVoltage;
		  HAL_CAN_Transmit(&hcan, 10);
		  HAL_GPIO_TogglePin(GPIOC, HEARTBEAT_LED_Pin);

	  }

	  LoopCount++;

	  HAL_Delay(1);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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

void AddValue(uint16_t newValue, uint16_t array[], uint8_t len)
{
	for(uint8_t i = len - 1; i > 0; i--)
	{
		array[i] = array[i-1];
	}

	array[0] = newValue;
}

uint16_t CalculateArrayAverage(uint16_t array[], uint8_t len)
{
	uint32_t average = 0;

	for(uint8_t i = 0; i < len; i++)
	{
		average += array[i];
	}

	return average / len;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	// Check safeties
	CheckSafeties();

	// Average throttle 0 ADC readings
	AddValue(ADCRawValues[4], ThrottleAveraging, 10);

	// Average DC link ADC readings
	AddValue(ADCRawValues[8], DCLinkAveraging, 10);
	DCLinkVoltage = (CalculateArrayAverage(DCLinkAveraging, 10) * (3.32 / 4096.0)) / DCLinkRatio;

	// Calculate DC link voltage


	// Calculate throttle percentage
	ThrottleCommand = ScaleToPercentage(CalculateArrayAverage(ThrottleAveraging, 10), ThrottleMin, ThrottleMax);

	HAL_GPIO_TogglePin(GPIOC, DIAG_LED_0_Pin);
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
	uint8_t CAN_ID = hcan->pRxMsg->StdId;

	switch (CAN_ID) {
	case COMMAND_ADDR:
		if(hcan->pRxMsg->RTR == CAN_RTR_DATA)
		{
			//CAN Set command
		}

		if(hcan->pRxMsg->RTR == CAN_RTR_REMOTE)
		{
			//CAN Request command
		}
		break;

	case CONFIG_ADDR:
		if(hcan->pRxMsg->RTR == CAN_RTR_DATA)
		{
			//CAN Set command
			ThrottleRampTime = hcan->pRxMsg->Data[0];
			BoostCommand = hcan->pRxMsg->Data[1];
		}

		if(hcan->pRxMsg->RTR == CAN_RTR_REMOTE)
		{
			//CAN Request command
		}
		break;
	}


	if(hcan->pRxMsg->StdId == 0x01 && hcan->pRxMsg->IDE == CAN_ID_STD)
	{
		TIM1->CCR2 = 100 + hcan->pRxMsg->Data[0];
		TIM1->CCR3 = 100 - hcan->pRxMsg->Data[0];
	}

	HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
}

uint8_t ScaleToPercentage(uint16_t input, uint16_t in_min, uint16_t in_max)
{
	uint8_t result = (100 * (input - in_min) / (in_max - in_min));
	if(result >  100) result = 100;
	if(result < 0) result = 0;
	return result;
}

void CheckSafeties()
{
	//Check throttle bounds
	if(ADCRawValues[4] < ThrottleMin - 100 || ADCRawValues[4] > ThrottleMax + 100)
	{
		Error = 1;
	} else {
		Error = 0;
	}

#ifdef MULTI_THROTTLE_MODE
	if(ADCRawValues[5] < ThrottleMin - 100 || ADCRawValues[5] > ThrottleMax + 100)
	{
		Error = 1;
	} else {
		Error = 0;
	}
#endif

	//Check boost voltage limits

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

	HAL_GPIO_WritePin(GPIOC, OK_LED_Pin, GPIO_PIN_RESET);

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
