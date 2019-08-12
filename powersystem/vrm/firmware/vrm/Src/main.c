
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
#include "gpio.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

volatile uint16_t LoopCount = 0;
volatile float vddVoltage = 3.26;
volatile uint16_t ADCRawValues[5] = {0};
volatile uint8_t ADCChannelIndex = 0;

//Voltage divider ratio calculations
//Use R2 / (R1 + R2)
static float _12V_Main_Ratio =  0.15151515151515151515151515151515; //10000 / (56000 + 10000)
static float _12V_Aux_Ratio = 0.15151515151515151515151515151515; //10000 / (56000 + 10000)
static float _80V_Ratio = 0.02629016553067185978578383641675; //2700 / (100000 + 2700)

//Voltages
volatile float Main_12V_Voltage = 0;
volatile float Aux_12V_Voltage = 0;
volatile float Input_Voltage = 0;

//Thresholds
float Voltage_Tolerance = 0.1; //0-1 adjustment (x100 for percent)
float Input_Lower_Threshold = 39; //Lower input voltage limit
float Input_Upper_Threshold = 45; //Upper input voltage limit

//Status flags
volatile uint8_t Main_OK = 0;
volatile uint8_t Aux_OK = 0;
volatile uint8_t System_PSU_OK = 0;
volatile uint8_t CAN_OK = 0;
volatile uint8_t Input_Fault = 0;
volatile uint8_t Main_Fault = 0;
volatile uint8_t Aux_Fault = 0;
volatile uint8_t System_PSU_Fault = 0;
volatile uint8_t Fault = 0;


//Enable flags
uint8_t Main_Enable = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

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
  MX_ADC_Init();
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */

  HAL_ADC_Start_IT(&hadc);
  //HAL_ADC_Start(&hadc);

  hcan.Instance->MCR = 0x60;

  HAL_CAN_Receive_IT(&hcan, CAN_FIFO0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //Check for invalid main output
	  if((Main_12V_Voltage < (12 - (12 * Voltage_Tolerance)) && Main_Enable == 1) || Main_12V_Voltage > (12 + (12 * Voltage_Tolerance)))
	  {
		  Main_Fault = 1;
		  Main_OK = 0;
	  } else {
		 Main_Fault = 0;
		 Main_OK = 1;
	  }

	  //Check for out of range input
	  if(Input_Voltage > Input_Upper_Threshold || Input_Voltage < Input_Lower_Threshold)
	  {
		  Input_Fault = 1;
	  } else if (Input_Voltage < Input_Upper_Threshold || Input_Voltage > Input_Lower_Threshold)
	  {
		  Input_Fault = 0;
	  }

	  //Check for invalid aux output
	  if(Aux_12V_Voltage < (12 - (12 * Voltage_Tolerance)) || Aux_12V_Voltage > (12 + (12 * Voltage_Tolerance)))
	  {
		  Aux_Fault = 1;
		  Aux_OK = 0;
	  } else {
		  Aux_Fault = 0;
		  Aux_OK = 1;
	  }

	  if(Aux_Fault || Input_Fault || Main_Fault)
	  {
		  Fault = 1;
	  } else {
		  Fault = 0;
	  }

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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

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

void HAL_SYSTICK_Callback(void)
{
	if(LoopCount == 1000) LoopCount = 0;

	//Heartbeat indicator toggle
	if(LoopCount % 500 == 0)
	{
		HAL_GPIO_TogglePin(HEARTBEAT_LED_GPIO_Port, HEARTBEAT_LED_Pin);
	}

	//Fault indicators toggle
	if(LoopCount % 100 == 0)
	{
		if(Fault == 1)
		{
			HAL_GPIO_TogglePin(FAULT_LED_GPIO_Port, FAULT_LED_Pin);

			if(Main_OK == 0) HAL_GPIO_TogglePin(MAIN_OK_LED_GPIO_Port, MAIN_OK_LED_Pin);
			if(Aux_OK == 0) HAL_GPIO_TogglePin(AUX_OK_LED_GPIO_Port, AUX_OK_LED_Pin);
		} else {
			HAL_GPIO_WritePin(FAULT_LED_GPIO_Port, FAULT_LED_Pin, 0);
		}

		hcan.pTxMsg->Data[0] = (int)Main_12V_Voltage;
		hcan.pTxMsg->Data[1] = (int)Aux_12V_Voltage;
		hcan.pTxMsg->Data[2] = (int)Input_Voltage;

		if(HAL_CAN_Transmit(&hcan, 10) != HAL_OK)
		{
			Error_Handler();
		}

	}

	//Status indicator setting
	if(Main_OK == 1)
	{
		HAL_GPIO_WritePin(MAIN_OK_LED_GPIO_Port, MAIN_OK_LED_Pin, 0);
	}

	if(Aux_OK == 1)
	{
		HAL_GPIO_WritePin(AUX_OK_LED_GPIO_Port, AUX_OK_LED_Pin, 1);
	}

	LoopCount++;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC))
	{
		ADCRawValues[ADCChannelIndex] = HAL_ADC_GetValue(hadc);
		ADCChannelIndex++;
	}

	if(__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS))
	{
		//ADCChannelIndex = 0;
		//DCBUS_VOLTAGE = (ADC_RAW * (3.3 / 4096.0)) / 0.04347826086956521739130434782609; // calculated from R2 / (R1 + R2)
		Main_12V_Voltage = (ADCRawValues[0] * (vddVoltage / 4096.0)) / _12V_Main_Ratio;
		Aux_12V_Voltage = (ADCRawValues[2] * (vddVoltage / 4096.0)) / _12V_Aux_Ratio;
		Input_Voltage = (ADCRawValues[4] * (vddVoltage / 4096.0)) / _80V_Ratio;

		HAL_ADC_Start_IT(hadc);
		ADCChannelIndex = 0;
	}
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
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
