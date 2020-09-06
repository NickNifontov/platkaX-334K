/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32f3xx_hal.h"

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
#define PIN_PF0_Pin GPIO_PIN_0
#define PIN_PF0_GPIO_Port GPIOF
#define PIN_PF1_Pin GPIO_PIN_1
#define PIN_PF1_GPIO_Port GPIOF
#define PIN_OCV_ADC1_IN1_Pin GPIO_PIN_0
#define PIN_OCV_ADC1_IN1_GPIO_Port GPIOA
#define PIN_RAW70_ADC1_IN2_Pin GPIO_PIN_1
#define PIN_RAW70_ADC1_IN2_GPIO_Port GPIOA
#define PIN_BUZZER_TIM15_Pin GPIO_PIN_2
#define PIN_BUZZER_TIM15_GPIO_Port GPIOA
#define PIN_RAW30_ADC1_IN4_Pin GPIO_PIN_3
#define PIN_RAW30_ADC1_IN4_GPIO_Port GPIOA
#define PIN_OCI_ADC2_IN1_Pin GPIO_PIN_4
#define PIN_OCI_ADC2_IN1_GPIO_Port GPIOA
#define PIN_TEMP_ADC2_IN2_Pin GPIO_PIN_5
#define PIN_TEMP_ADC2_IN2_GPIO_Port GPIOA
#define V16_ADC2_IN3_Pin GPIO_PIN_6
#define V16_ADC2_IN3_GPIO_Port GPIOA
#define PIN_OCV_COMP2_ADC2_4_Pin GPIO_PIN_7
#define PIN_OCV_COMP2_ADC2_4_GPIO_Port GPIOA
#define PIN_OCI_COMP4_ADC1_11_Pin GPIO_PIN_0
#define PIN_OCI_COMP4_ADC1_11_GPIO_Port GPIOB
#define PIN_DRV_Pin GPIO_PIN_1
#define PIN_DRV_GPIO_Port GPIOB
#define PIN_PWMB1_Pin GPIO_PIN_8
#define PIN_PWMB1_GPIO_Port GPIOA
#define PIN_PWMB2_Pin GPIO_PIN_9
#define PIN_PWMB2_GPIO_Port GPIOA
#define PIN_PWMA1_Pin GPIO_PIN_10
#define PIN_PWMA1_GPIO_Port GPIOA
#define PIN_PWMA2_Pin GPIO_PIN_11
#define PIN_PWMA2_GPIO_Port GPIOA
#define PIN_PEREK_Pin GPIO_PIN_12
#define PIN_PEREK_GPIO_Port GPIOA
#define PIN_PEREK_EXTI_IRQn EXTI15_10_IRQn
#define PIN_DIO_Pin GPIO_PIN_13
#define PIN_DIO_GPIO_Port GPIOA
#define PIN_CLK_Pin GPIO_PIN_14
#define PIN_CLK_GPIO_Port GPIOA
#define PIN_LEDI_TIM2CH1_Pin GPIO_PIN_15
#define PIN_LEDI_TIM2CH1_GPIO_Port GPIOA
#define PIN_LEDV_TIM2CH2_Pin GPIO_PIN_3
#define PIN_LEDV_TIM2CH2_GPIO_Port GPIOB
#define PIN_COOLER_TIM16_Pin GPIO_PIN_4
#define PIN_COOLER_TIM16_GPIO_Port GPIOB
#define PIN_IGLA_TIM17_Pin GPIO_PIN_5
#define PIN_IGLA_TIM17_GPIO_Port GPIOB
#define PIN_TX_Pin GPIO_PIN_6
#define PIN_TX_GPIO_Port GPIOB
#define PIN_RX_Pin GPIO_PIN_7
#define PIN_RX_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
