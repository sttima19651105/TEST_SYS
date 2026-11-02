/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define CSDAC_REF_Pin GPIO_PIN_2
#define CSDAC_REF_GPIO_Port GPIOE
#define LDAC_REF_Pin GPIO_PIN_3
#define LDAC_REF_GPIO_Port GPIOE
#define VOLT_neg6V_Pin GPIO_PIN_3
#define VOLT_neg6V_GPIO_Port GPIOF
#define CURR_neg6V_Pin GPIO_PIN_4
#define CURR_neg6V_GPIO_Port GPIOF
#define TEMR_MOD_Pin GPIO_PIN_5
#define TEMR_MOD_GPIO_Port GPIOF
#define TEMR_VP1_Pin GPIO_PIN_6
#define TEMR_VP1_GPIO_Port GPIOF
#define TEMPR_VP2_Pin GPIO_PIN_7
#define TEMPR_VP2_GPIO_Port GPIOF
#define VOLT_MOD_Pin GPIO_PIN_8
#define VOLT_MOD_GPIO_Port GPIOF
#define VOLT_VP1_Pin GPIO_PIN_9
#define VOLT_VP1_GPIO_Port GPIOF
#define VOLT_VP2_Pin GPIO_PIN_10
#define VOLT_VP2_GPIO_Port GPIOF
#define IMP_CURR_MOD_Pin GPIO_PIN_0
#define IMP_CURR_MOD_GPIO_Port GPIOC
#define STAT_CURR_MOD_Pin GPIO_PIN_1
#define STAT_CURR_MOD_GPIO_Port GPIOC
#define IMP_CURR_VP11_Pin GPIO_PIN_2
#define IMP_CURR_VP11_GPIO_Port GPIOC
#define STAT_CURR_VP11_Pin GPIO_PIN_3
#define STAT_CURR_VP11_GPIO_Port GPIOC
#define IMP_CURR_VP12_Pin GPIO_PIN_0
#define IMP_CURR_VP12_GPIO_Port GPIOA
#define STAT_CURR_VP12_Pin GPIO_PIN_1
#define STAT_CURR_VP12_GPIO_Port GPIOA
#define IMP_CURR_VP12A2_Pin GPIO_PIN_2
#define IMP_CURR_VP12A2_GPIO_Port GPIOA
#define STAT_CURR_VP13_Pin GPIO_PIN_3
#define STAT_CURR_VP13_GPIO_Port GPIOA
#define IMP_CURR_VP21_Pin GPIO_PIN_4
#define IMP_CURR_VP21_GPIO_Port GPIOA
#define STAT_CURR_VP21_Pin GPIO_PIN_5
#define STAT_CURR_VP21_GPIO_Port GPIOA
#define IMP_CURR_VP22_Pin GPIO_PIN_6
#define IMP_CURR_VP22_GPIO_Port GPIOA
#define STAT_CURR_VP21A7_Pin GPIO_PIN_7
#define STAT_CURR_VP21A7_GPIO_Port GPIOA
#define IMP_CURR_VP23_Pin GPIO_PIN_4
#define IMP_CURR_VP23_GPIO_Port GPIOC
#define STAT_CURR_VP23_Pin GPIO_PIN_5
#define STAT_CURR_VP23_GPIO_Port GPIOC
#define IMP_POW_PRD_Pin GPIO_PIN_0
#define IMP_POW_PRD_GPIO_Port GPIOB
#define IMP_POW_AFP_Pin GPIO_PIN_1
#define IMP_POW_AFP_GPIO_Port GPIOB
#define VIDEO2_Pin GPIO_PIN_11
#define VIDEO2_GPIO_Port GPIOF
#define Protection_by_KSXN_Pin GPIO_PIN_13
#define Protection_by_KSXN_GPIO_Port GPIOF
#define OLED_C_D_Pin GPIO_PIN_1
#define OLED_C_D_GPIO_Port GPIOG
#define H_VOLT_MOD_DIS_Pin GPIO_PIN_7
#define H_VOLT_MOD_DIS_GPIO_Port GPIOE
#define H_VOLT_VP1_DIS_Pin GPIO_PIN_8
#define H_VOLT_VP1_DIS_GPIO_Port GPIOE
#define H_VOLT_DIS_VP2_Pin GPIO_PIN_9
#define H_VOLT_DIS_VP2_GPIO_Port GPIOE
#define ENABLE_Pin GPIO_PIN_10
#define ENABLE_GPIO_Port GPIOE
#define RESET_OUT_Pin GPIO_PIN_11
#define RESET_OUT_GPIO_Port GPIOE
#define RST_LEVEL_Pin GPIO_PIN_12
#define RST_LEVEL_GPIO_Port GPIOE
#define POW_START_Pin GPIO_PIN_14
#define POW_START_GPIO_Port GPIOE
#define RESET_HI_Pin GPIO_PIN_15
#define RESET_HI_GPIO_Port GPIOE
#define Hand_Start_Tr_Pin GPIO_PIN_15
#define Hand_Start_Tr_GPIO_Port GPIOB
#define Protection_KSHN_Pin GPIO_PIN_9
#define Protection_KSHN_GPIO_Port GPIOD
#define Overheating_Pin GPIO_PIN_10
#define Overheating_GPIO_Port GPIOD
#define Protection_CURR_Pin GPIO_PIN_11
#define Protection_CURR_GPIO_Port GPIOD
#define Contr_50V_Pin GPIO_PIN_12
#define Contr_50V_GPIO_Port GPIOD
#define Contr_n6V_Pin GPIO_PIN_13
#define Contr_n6V_GPIO_Port GPIOD
#define TEST_STROBE_Pin GPIO_PIN_14
#define TEST_STROBE_GPIO_Port GPIOD
#define TEST_VIDEO2_Pin GPIO_PIN_15
#define TEST_VIDEO2_GPIO_Port GPIOD
#define OLED_E_Pin GPIO_PIN_2
#define OLED_E_GPIO_Port GPIOG
#define OLED_RW_Pin GPIO_PIN_3
#define OLED_RW_GPIO_Port GPIOG
#define OLED_D7_Pin GPIO_PIN_4
#define OLED_D7_GPIO_Port GPIOG
#define OLED_D6_Pin GPIO_PIN_5
#define OLED_D6_GPIO_Port GPIOG
#define OLED_D5_Pin GPIO_PIN_6
#define OLED_D5_GPIO_Port GPIOG
#define OLED_D4_Pin GPIO_PIN_7
#define OLED_D4_GPIO_Port GPIOG
#define WORK_Pin GPIO_PIN_15
#define WORK_GPIO_Port GPIOA
#define READY_Pin GPIO_PIN_1
#define READY_GPIO_Port GPIOD
#define FAILURE_Pin GPIO_PIN_2
#define FAILURE_GPIO_Port GPIOD
#define POW_50P_ROC_Pin GPIO_PIN_10
#define POW_50P_ROC_GPIO_Port GPIOG
#define STROBE_Pin GPIO_PIN_12
#define STROBE_GPIO_Port GPIOG
#define Transmission_Pin GPIO_PIN_1
#define Transmission_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
/* USER CODE BEGIN Private defines */
#define  ADC1_CHANELS_NUM   8
#define  ADC2_CHANELS_NUM   8
#define  ADC3_CHANELS_NUM   8

#define   NB_ADC1_rd   5

#define   SIZE_BUFF_NMEA   200
#define  start_SYMBOL   '$'
#define  stop_SYMBOL    '*'
#define  comma_SYMBOL    ','
#define   BITS_NB   64

//#define RS_1  GPIOG->BSRR = GPIO_BSRR_BS8
//#define RS_0  GPIOG->BSRR = GPIO_BSRR_BR8
//#define EN_1  GPIOG->BSRR = GPIO_BSRR_BS6
//#define EN_0  GPIOG->BSRR = GPIO_BSRR_BR6
//#define RW_0  GPIOG->BSRR = GPIO_BSRR_BR7 // Always 0 for write mode


//#define	N6V_S GPIOD->BSRR = GPIO_BSRR_BR13
//#define	N6V_R GPIOD->BSRR = GPIO_BSRR_BS13

//#define	C50V_S GPIOD->BSRR = GPIO_BSRR_BR12
//#define	C50V_R GPIOD->BSRR = GPIO_BSRR_BS12

//#define	P_CURR_S GPIOD->BSRR = GPIO_BSRR_BR11
//#define	P_CURR_R GPIOD->BSRR = GPIO_BSRR_BS11

//#define	Overheating_S GPIOD->BSRR = GPIO_BSRR_BR10
//#define	Overheating_R GPIOD->BSRR = GPIO_BSRR_BS10

//#define	P_KSHN_S GPIOD->BSRR = GPIO_BSRR_BR9
//#define	P_KSHN_R GPIOD->BSRR = GPIO_BSRR_BS9

//#define	POW_LOW_S GPIOD->BSRR = GPIO_BSRR_BR8
//#define	POW_LOW_R GPIOD->BSRR = GPIO_BSRR_BS8


extern volatile  uint8_t  curent_NB_bits;

extern volatile  uint8_t COMMAND_BUFF_NMEA[SIZE_BUFF_NMEA];
extern   int COMMAND_BUFF_Index;
extern volatile int COMMAND_READY_Flag;


extern  ADC_HandleTypeDef hadc1;
extern  ADC_HandleTypeDef hadc2;
extern  ADC_HandleTypeDef hadc3;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
