/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define HV_CURRENT_Pin GPIO_PIN_0
#define HV_CURRENT_GPIO_Port GPIOC
#define THROTTLE_Pin GPIO_PIN_1
#define THROTTLE_GPIO_Port GPIOC
#define MOTOR_TEMP_Pin GPIO_PIN_2
#define MOTOR_TEMP_GPIO_Port GPIOC
#define INVERTER_TEMP_Pin GPIO_PIN_3
#define INVERTER_TEMP_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define DCLINK_VOLTAGE_Pin GPIO_PIN_4
#define DCLINK_VOLTAGE_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define BOOST_PWM_Pin GPIO_PIN_6
#define BOOST_PWM_GPIO_Port GPIOA
#define DCBUS_VOLTAGE_Pin GPIO_PIN_7
#define DCBUS_VOLTAGE_GPIO_Port GPIOA
#define INVERTER_SHUTDOWN_Pin GPIO_PIN_4
#define INVERTER_SHUTDOWN_GPIO_Port GPIOC
#define INVERTER_FAIL_Pin GPIO_PIN_5
#define INVERTER_FAIL_GPIO_Port GPIOC
#define LV_CURRENT_Pin GPIO_PIN_0
#define LV_CURRENT_GPIO_Port GPIOB
#define BOOST_SHUTDOWN_Pin GPIO_PIN_1
#define BOOST_SHUTDOWN_GPIO_Port GPIOB
#define BOOST_FAIL_Pin GPIO_PIN_2
#define BOOST_FAIL_GPIO_Port GPIOB
#define FAULT_OUT_Pin GPIO_PIN_11
#define FAULT_OUT_GPIO_Port GPIOB
#define COMMS_LED_Pin GPIO_PIN_13
#define COMMS_LED_GPIO_Port GPIOB
#define HALL_U_Pin GPIO_PIN_6
#define HALL_U_GPIO_Port GPIOC
#define HALL_U_EXTI_IRQn EXTI9_5_IRQn
#define HALL_V_Pin GPIO_PIN_7
#define HALL_V_GPIO_Port GPIOC
#define HALL_V_EXTI_IRQn EXTI9_5_IRQn
#define HALL_W_Pin GPIO_PIN_8
#define HALL_W_GPIO_Port GPIOC
#define HALL_W_EXTI_IRQn EXTI9_5_IRQn
#define MOT_U_Pin GPIO_PIN_8
#define MOT_U_GPIO_Port GPIOA
#define MOT_V_Pin GPIO_PIN_9
#define MOT_V_GPIO_Port GPIOA
#define MOT_W_Pin GPIO_PIN_10
#define MOT_W_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

uint16_t GPIOC_Data = 0;

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
#define DBG_PRINTF  (void)tfp_printf("%10d: ", HAL_GetTick());(void)tfp_printf

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
