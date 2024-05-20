/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#define BIN1_Pin GPIO_PIN_3
#define BIN1_GPIO_Port GPIOA
#define BIN2_Pin GPIO_PIN_4
#define BIN2_GPIO_Port GPIOA
#define AIN1_Pin GPIO_PIN_0
#define AIN1_GPIO_Port GPIOB
#define AIN2_Pin GPIO_PIN_1
#define AIN2_GPIO_Port GPIOB
#define BOTTON_LEFT_Pin GPIO_PIN_12
#define BOTTON_LEFT_GPIO_Port GPIOB
#define BOTTON_MID_Pin GPIO_PIN_13
#define BOTTON_MID_GPIO_Port GPIOB
#define BOTTON_RIGHT_Pin GPIO_PIN_14
#define BOTTON_RIGHT_GPIO_Port GPIOB
#define TOP_UP_Pin GPIO_PIN_11
#define TOP_UP_GPIO_Port GPIOA
#define TOP_DOWN_Pin GPIO_PIN_15
#define TOP_DOWN_GPIO_Port GPIOA
#define TOP_MID_Pin GPIO_PIN_3
#define TOP_MID_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
