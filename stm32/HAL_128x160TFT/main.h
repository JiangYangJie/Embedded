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
#define RST_Pin GPIO_PIN_3
#define RST_GPIO_Port GPIOA
#define AD_Pin GPIO_PIN_4
#define AD_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_6
#define CS_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
#define LCD_RST1  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,GPIO_PIN_SET);
#define LCD_RST0  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,GPIO_PIN_RESET);
//   LCD_DC
#define LCD_DC1  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_SET);//a0//GPIOB->BSRR=GPIO_BSRR_BS_12
#define LCD_DC0  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET);//GPIOB->BSRR=GPIO_BSRR_BR_12//

//  LCD_CS
#define LCD_CS1  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET);// GPIOB->BSRR=GPIO_BSRR_BS_10//
#define LCD_CS0  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_RESET);// GPIOB->BSRR=GPIO_BSRR_BR_10//
void lcd7735_senddata(unsigned char data);
void lcd7735_send16bData(unsigned char msb,unsigned char lsb);
void spi2_8b_init(void);
void spi2_16b_init(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
