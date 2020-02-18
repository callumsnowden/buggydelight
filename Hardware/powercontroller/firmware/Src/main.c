
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "adc.h"
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

#include <math.h>
#include <stdint.h>
#include "printf.h"
#include <string.h>

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

volatile uint16_t ADC_RAW = 0;
volatile float DCBUS_VOLTAGE = 0;

enum PowerState {
	PWR_ERROR = -1,
	PWR_OFF,
	PWR_OFF_REQUEST,
	PWR_PRECHARGE,
	PWR_ON,
	PWR_ON_REQUEST
};

uint8_t state = PWR_OFF;

uint8_t c = 'A';

uint8_t EndOfLine = 0;

uint8_t RXBuffer[BUFFER_LENGTH];

uint8_t BufferPos = 0;

uint16_t SystickCounter = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void UART_putc(void* p, char c);
void PowerOn_Safe();
void PowerOn_Unsafe();
void PowerOff();

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_USART1_UART_Init();
  MX_CAN_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */

  //This magic makes CAN bus work when debugging, apparently
  hcan.Instance->MCR = 0x60;

  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);

  //Initialise printf
  init_printf(NULL, UART_putc);

  //Allocate some empty memory for UART
  memset(RXBuffer, 0, BUFFER_LENGTH);

  //Start various bits
  HAL_ADC_Start_IT(&hadc);
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&c, 1);
  HAL_CAN_Receive_IT(&hcan, CAN_FIFO0);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  DBG_PRINTF("Started\r\n");

  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

	  /*
	  HAL_GPIO_WritePin(PRECHARGE_RELAY_GPIO_Port, PRECHARGE_RELAY_Pin, GPIO_PIN_SET);
	  HAL_Delay(1000);
	  HAL_GPIO_WritePin(PRECHARGE_RELAY_GPIO_Port, PRECHARGE_RELAY_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  */

	  if(EndOfLine)
	  {
		  EndOfLine = 0;
		  BufferPos = 0;
		  switch (RXBuffer[0])
		  {
		  case 'i':
			  if(state == PWR_ON)
			  {
				  DBG_PRINTF("Already powered on\r\n");

			  } else {
				  state = PWR_ON_REQUEST;
			  }
			  break;

		  case 'o':
			  state = PWR_OFF_REQUEST;
			  break;

		  case 'u':
			  PowerOn_Unsafe();
			  break;

		  case 'r':
			  state = PWR_OFF;
			  HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
			  break;

		  case '?':
			  DBG_PRINTF("DC Bus voltage: %dV. State: %d\r\n", (uint8_t)DCBUS_VOLTAGE, state);
			  break;

		  default:
			  DBG_PRINTF("Invalid command. 'i' for safe power on, 'o' for power off, 'u' for unsafe power on, 'r' to reset error, '?' to request status report\r\n");
			  break;
		  }

		  memset(RXBuffer, 0, BUFFER_LENGTH);
	  }

	  if(DCBUS_VOLTAGE < NOMINAL_VOLTAGE - (NOMINAL_VOLTAGE * NOMINAL_VOLTAGE_TOLERANCE) && state == PWR_ON)
	  {
		  state = PWR_OFF_REQUEST;
		  DBG_PRINTF("DC Bus voltage error! Current voltage: %dV\r\n", (uint8_t)DCBUS_VOLTAGE);
		  HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
	  }

	  switch (state)
	  {
	  case PWR_ON_REQUEST:
		  PowerOn_Safe();
		  break;

	  case PWR_OFF_REQUEST:
		  PowerOff();
		  break;
	  }

  }

  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

void PowerOn_Safe()
{
	if(state == PWR_ON_REQUEST)
	{
		uint32_t startTick = HAL_GetTick();
		DBG_PRINTF("Requested safe power on\r\n");
		state = PWR_PRECHARGE;

		HAL_GPIO_WritePin(ORANGE_LED_GPIO_Port, ORANGE_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(NEGATIVE_RELAY_GPIO_Port, NEGATIVE_RELAY_Pin, GPIO_PIN_SET);
		HAL_Delay(RELAY_DELAY);
		HAL_GPIO_WritePin(PRECHARGE_RELAY_GPIO_Port, PRECHARGE_RELAY_Pin, GPIO_PIN_SET);
		HAL_Delay(RELAY_DELAY);

		while(DCBUS_VOLTAGE < NOMINAL_VOLTAGE - (NOMINAL_VOLTAGE * NOMINAL_VOLTAGE_TOLERANCE))
		{
			//Do nothing besides check the timeout
			if(HAL_GetTick() > startTick + PRECHARGE_TIMEOUT)
			{
				HAL_GPIO_WritePin(ORANGE_LED_GPIO_Port, ORANGE_LED_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(PRECHARGE_RELAY_GPIO_Port, PRECHARGE_RELAY_Pin, GPIO_PIN_RESET);
				HAL_Delay(RELAY_DELAY);
				HAL_GPIO_WritePin(NEGATIVE_RELAY_GPIO_Port, NEGATIVE_RELAY_Pin, GPIO_PIN_RESET);
				DBG_PRINTF("Precharge failed in specified timeout!\r\n");
				state = PWR_ERROR;
				break;
			}
		}

		if(DCBUS_VOLTAGE > NOMINAL_VOLTAGE - (NOMINAL_VOLTAGE * NOMINAL_VOLTAGE_TOLERANCE))
		{
			HAL_GPIO_WritePin(POSITIVE_RELAY_GPIO_Port, POSITIVE_RELAY_Pin, GPIO_PIN_SET);
			HAL_Delay(RELAY_DELAY);
			HAL_GPIO_WritePin(PRECHARGE_RELAY_GPIO_Port, PRECHARGE_RELAY_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(ORANGE_LED_GPIO_Port, ORANGE_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
			DBG_PRINTF("Precharge done in %d ticks. DC Bus voltage: %dV\r\n", (uint8_t)(HAL_GetTick() - startTick), (uint8_t)DCBUS_VOLTAGE);
			state = PWR_ON;
		} else {

		}
	} else {
		DBG_PRINTF("State not off! Current state: %d\r\n", state);
	}
}

void PowerOn_Unsafe()
{
	if(state == PWR_OFF)
	{
		DBG_PRINTF("Requested unsafe power on\r\n");
		HAL_GPIO_WritePin(NEGATIVE_RELAY_GPIO_Port, NEGATIVE_RELAY_Pin, GPIO_PIN_SET);
		HAL_Delay(RELAY_DELAY);
		HAL_GPIO_WritePin(POSITIVE_RELAY_GPIO_Port, POSITIVE_RELAY_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
		DBG_PRINTF("Unsafe power on complete\r\n");
		state = PWR_ON;
	} else {
		DBG_PRINTF("State not off! Current state: %d\r\n", state);
	}
}

void PowerOff()
{
	/*
	if(state == PWR_PRECHARGE)
	{
		DBG_PRINTF("Requested precharge abort! DC Bus voltage: %dV\r\n", (uint8_t)DCBUS_VOLTAGE);
		HAL_GPIO_WritePin(PRECHARGE_RELAY_GPIO_Port, PRECHARGE_RELAY_Pin, GPIO_PIN_RESET);
		HAL_Delay(RELAY_DELAY);
		HAL_GPIO_WritePin(NEGATIVE_RELAY_GPIO_Port, NEGATIVE_RELAY_Pin, GPIO_PIN_RESET);
		DBG_PRINTF("Precharge abort complete\r\n");
		state = PWR_OFF;
	} else {
		DBG_PRINTF("State not off! Current state: %d\r\n", state);
	}
	*/

	if(state == PWR_OFF_REQUEST)
	{
		DBG_PRINTF("Requested power off. DC Bus voltage: %dV\r\n", (uint8_t)DCBUS_VOLTAGE);
		HAL_GPIO_WritePin(PRECHARGE_RELAY_GPIO_Port, PRECHARGE_RELAY_Pin, GPIO_PIN_RESET);
		HAL_Delay(RELAY_DELAY);
		HAL_GPIO_WritePin(POSITIVE_RELAY_GPIO_Port, POSITIVE_RELAY_Pin, GPIO_PIN_RESET);
		HAL_Delay(RELAY_DELAY);
		HAL_GPIO_WritePin(NEGATIVE_RELAY_GPIO_Port, NEGATIVE_RELAY_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
		DBG_PRINTF("Power off complete\r\n");
		state = PWR_OFF;
	} else {
		DBG_PRINTF("State not off! Current state: %d\r\n", state);
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	//Convert ADC reading into DC bus voltage
    ADC_RAW = (float)HAL_ADC_GetValue(hadc);
    DCBUS_VOLTAGE = (ADC_RAW * (3.3 / 4096.0)) / 0.04347826086956521739130434782609; // calculated from R2 / (R1 + R2)
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{

    HAL_UART_Receive_IT(&huart1, &c, 1);
    RXBuffer[BufferPos++] = c;

    if(c == '\n' || c == '\r')
    {
        EndOfLine = 1;
        RXBuffer[BufferPos++] = '\0';
    }

}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *CanHandle)
{
	if(CanHandle->pRxMsg->StdId == CAN_ADDRESS && CanHandle->pRxMsg->IDE == CAN_ID_STD && CanHandle->pRxMsg->DLC == 1)
	{
		if(CanHandle->pRxMsg->Data[0] == 1)
		{
			if(state == PWR_ON)
			{
				DBG_PRINTF("CANBUS: Already powered on\r\n");
			} else {
				DBG_PRINTF("CANBUS: Request to power on\r\n");
				state = PWR_ON_REQUEST;
			}
		}

		if(CanHandle->pRxMsg->Data[0] == 0)
		{
			DBG_PRINTF("CANBUS: Request to power off\r\n");
			state = PWR_OFF_REQUEST;
		}
	}

	HAL_CAN_Receive_IT(&hcan, CAN_FIFO0);
}

void ADC_IRQHandler()
{
    HAL_ADC_IRQHandler(&hadc);
}

void UART_putc(void* p, char c)
{
	HAL_UART_Transmit(&huart1, &c, 1, 100);
}

void HAL_SYSTICK_Callback()
{
	if(SystickCounter == 1000)
	{
		SystickCounter = 0;
	}

	if(SystickCounter % 500 == 0)
	{
		if(state != PWR_ON)
		{
			HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
		}
	}

	if(SystickCounter % 100 == 0)
	{
		hcan.pTxMsg->Data[0] = (uint8_t)ceil(DCBUS_VOLTAGE);
		hcan.pTxMsg->Data[1] = state;

		if(HAL_CAN_Transmit(&hcan, 10) != HAL_OK)
		{
			Error_Handler();
		}
	}

	SystickCounter++;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: DBG_PRINTF("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
