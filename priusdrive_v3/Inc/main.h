/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

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
#define HEARTBEAT_LED_Pin GPIO_PIN_13
#define HEARTBEAT_LED_GPIO_Port GPIOC
#define OK_LED_Pin GPIO_PIN_14
#define OK_LED_GPIO_Port GPIOC
#define DIAG_LED_0_Pin GPIO_PIN_15
#define DIAG_LED_0_GPIO_Port GPIOC
#define DIAG_LED_1_Pin GPIO_PIN_0
#define DIAG_LED_1_GPIO_Port GPIOD
#define BRAKE_IN_Pin GPIO_PIN_1
#define BRAKE_IN_GPIO_Port GPIOD
#define THROTTLE_0_Pin GPIO_PIN_0
#define THROTTLE_0_GPIO_Port GPIOC
#define THROTTLE_1_Pin GPIO_PIN_1
#define THROTTLE_1_GPIO_Port GPIOC
#define COOLANT_IN_TEMP_Pin GPIO_PIN_2
#define COOLANT_IN_TEMP_GPIO_Port GPIOC
#define COOLANT_OUT_TEMP_Pin GPIO_PIN_3
#define COOLANT_OUT_TEMP_GPIO_Port GPIOC
#define HALL_U_Pin GPIO_PIN_0
#define HALL_U_GPIO_Port GPIOA
#define HALL_V_Pin GPIO_PIN_1
#define HALL_V_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define CONTROL_CURRENT_SENSE_Pin GPIO_PIN_4
#define CONTROL_CURRENT_SENSE_GPIO_Port GPIOA
#define MOTOR_CURRENT_SENSE_Pin GPIO_PIN_5
#define MOTOR_CURRENT_SENSE_GPIO_Port GPIOA
#define PUMP_PWM_Pin GPIO_PIN_6
#define PUMP_PWM_GPIO_Port GPIOA
#define FAN_PWM_Pin GPIO_PIN_7
#define FAN_PWM_GPIO_Port GPIOA
#define DC_LINK_VOLTAGE_Pin GPIO_PIN_4
#define DC_LINK_VOLTAGE_GPIO_Port GPIOC
#define SYSTEM_VOLTAGE_Pin GPIO_PIN_5
#define SYSTEM_VOLTAGE_GPIO_Port GPIOC
#define DRIVE_TEMPERATURE_Pin GPIO_PIN_0
#define DRIVE_TEMPERATURE_GPIO_Port GPIOB
#define MOTOR_TEMPERATURE_Pin GPIO_PIN_1
#define MOTOR_TEMPERATURE_GPIO_Port GPIOB
#define GPIO_0_Pin GPIO_PIN_2
#define GPIO_0_GPIO_Port GPIOB
#define HALL_W_Pin GPIO_PIN_10
#define HALL_W_GPIO_Port GPIOB
#define GPIO_1_Pin GPIO_PIN_11
#define GPIO_1_GPIO_Port GPIOB
#define ESTOP_IN_Pin GPIO_PIN_12
#define ESTOP_IN_GPIO_Port GPIOB
#define FAULT_OUT_Pin GPIO_PIN_13
#define FAULT_OUT_GPIO_Port GPIOB
#define GPIO_2_Pin GPIO_PIN_14
#define GPIO_2_GPIO_Port GPIOB
#define GPIO_3_Pin GPIO_PIN_15
#define GPIO_3_GPIO_Port GPIOB
#define GPIO_10_Pin GPIO_PIN_6
#define GPIO_10_GPIO_Port GPIOC
#define GPIO_9_Pin GPIO_PIN_7
#define GPIO_9_GPIO_Port GPIOC
#define GPIO_8_Pin GPIO_PIN_8
#define GPIO_8_GPIO_Port GPIOC
#define MOTOR_ENABLE_Pin GPIO_PIN_9
#define MOTOR_ENABLE_GPIO_Port GPIOC
#define MOT_PWM_U_Pin GPIO_PIN_8
#define MOT_PWM_U_GPIO_Port GPIOA
#define MOT_PWM_V_Pin GPIO_PIN_9
#define MOT_PWM_V_GPIO_Port GPIOA
#define MOT_PWM_W_Pin GPIO_PIN_10
#define MOT_PWM_W_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_15
#define FLASH_CS_GPIO_Port GPIOA
#define GPIO_7_Pin GPIO_PIN_10
#define GPIO_7_GPIO_Port GPIOC
#define GPIO_6_Pin GPIO_PIN_11
#define GPIO_6_GPIO_Port GPIOC
#define GPIO_5_Pin GPIO_PIN_12
#define GPIO_5_GPIO_Port GPIOC
#define GPIO_4_Pin GPIO_PIN_2
#define GPIO_4_GPIO_Port GPIOD
#define FLASH_SCK_Pin GPIO_PIN_3
#define FLASH_SCK_GPIO_Port GPIOB
#define FLASH_MISO_Pin GPIO_PIN_4
#define FLASH_MISO_GPIO_Port GPIOB
#define FLASH_MOSI_Pin GPIO_PIN_5
#define FLASH_MOSI_GPIO_Port GPIOB
#define BOOST_PWM_Pin GPIO_PIN_6
#define BOOST_PWM_GPIO_Port GPIOB
#define BOOST_ENABLE_Pin GPIO_PIN_7
#define BOOST_ENABLE_GPIO_Port GPIOB
#define BOOST_FAIL_Pin GPIO_PIN_8
#define BOOST_FAIL_GPIO_Port GPIOB
#define MOTOR_FAIL_Pin GPIO_PIN_9
#define MOTOR_FAIL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
