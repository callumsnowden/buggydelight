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
#include "stm32f0xx_hal.h"

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
#define _12V_MAIN_LEVEL_Pin GPIO_PIN_0
#define _12V_MAIN_LEVEL_GPIO_Port GPIOA
#define _12V_MAIN_CURRENT_Pin GPIO_PIN_1
#define _12V_MAIN_CURRENT_GPIO_Port GPIOA
#define _12V_AUX_LEVEL_Pin GPIO_PIN_2
#define _12V_AUX_LEVEL_GPIO_Port GPIOA
#define _12V_AUX_CURRENT_Pin GPIO_PIN_3
#define _12V_AUX_CURRENT_GPIO_Port GPIOA
#define _80V_LEVEL_Pin GPIO_PIN_4
#define _80V_LEVEL_GPIO_Port GPIOA
#define HEARTBEAT_LED_Pin GPIO_PIN_7
#define HEARTBEAT_LED_GPIO_Port GPIOA
#define CAN_LED_Pin GPIO_PIN_4
#define CAN_LED_GPIO_Port GPIOC
#define MAIN_OK_LED_Pin GPIO_PIN_5
#define MAIN_OK_LED_GPIO_Port GPIOC
#define AUX_OK_LED_Pin GPIO_PIN_0
#define AUX_OK_LED_GPIO_Port GPIOB
#define FAULT_LED_Pin GPIO_PIN_1
#define FAULT_LED_GPIO_Port GPIOB
#define ESTOP_IN_Pin GPIO_PIN_2
#define ESTOP_IN_GPIO_Port GPIOB
#define _12V_MAIN_EN_Pin GPIO_PIN_8
#define _12V_MAIN_EN_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
