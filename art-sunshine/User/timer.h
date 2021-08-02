/**
  ******************************************************************************
  * @file    timer.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file contains all the functions prototypes for timer configure
  *			 and operation.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2020 Macro</center></h2>
  *
  * Licensed emmmmmmmmmm.....
  * 
  * I agree that my code agrees to all  the  restrictions  and  free  of charge 
  * stipulated by the GNU General Public License, because I believe that GNU is  
  * full of energy, appeal, and magic. No one needs my consent to use this code,  
  * if you are using it I found a bug in the  process. I  hope  you can give me
  * feedback. I am very happy to learn from people who are better than me.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER__H__
#define __TIMER__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_tim.h"

/** @addtogroup Timer_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* TIM8 PWM channelx */
typedef enum {
	TIM8_PWM_CH1 = 0,	/* TIM8 PWM channel */
	TIM8_PWM_CH2,
	TIM8_PWM_CH3,
	TIM8_PWM_CH4,
}TIM8_PWM_CHx_m;
	
/* Exported constants --------------------------------------------------------*/
/* 
 * variable: @system global timer counter
 */
extern volatile uint32_t timer_counter;		/* global timer nums */
extern volatile uint32_t seconds;
	
/* Exported macro ------------------------------------------------------------*/
/* 
 * macro: @choose the basic timer
 */
#define BASIC_TIM 	    			TIM6	/* selected timer */
#define INDEPENT_TIMER				TIM2	/* Independent timer */
#define MOTOR_CONTRL_TIMER			TIM8	/* motor control timer */

#define MOTOR_A_PWM_PIN				GPIO_Pin_5			/* motor A control */
#define MOTOR_A_PWM_PORT			GPIOI
#define MOTOR_A_PWM_CLOCK			RCC_AHB1Periph_GPIOI
#define MOTOR_A_PWM_AF_SOURSE		GPIO_PinSource5
#define MOTOR_A_PWM_AF_TIMER		GPIO_AF_TIM8

#define MOTOR_B_PWM_PIN				GPIO_Pin_6			/* motor B control */
#define MOTOR_B_PWM_PORT			GPIOI
#define MOTOR_B_PWM_CLOCK			RCC_AHB1Periph_GPIOI
#define MOTOR_B_PWM_AF_SOURSE		GPIO_PinSource6
#define MOTOR_B_PWM_AF_TIMER		GPIO_AF_TIM8

#define MOTOR_C_PWM_PIN				GPIO_Pin_7			/* motor C control */
#define MOTOR_C_PWM_PORT			GPIOI
#define MOTOR_C_PWM_CLOCK			RCC_AHB1Periph_GPIOI
#define MOTOR_C_PWM_AF_SOURSE		GPIO_PinSource7
#define MOTOR_C_PWM_AF_TIMER		GPIO_AF_TIM8

#define MOTOR_D_PWM_PIN				GPIO_Pin_2			/* motor D control */
#define MOTOR_D_PWM_PORT			GPIOI
#define MOTOR_D_PWM_CLOCK			RCC_AHB1Periph_GPIOI
#define MOTOR_D_PWM_AF_SOURSE		GPIO_PinSource2
#define MOTOR_D_PWM_AF_TIMER		GPIO_AF_TIM8

/* Exported functions --------------------------------------------------------*/
void TIM6_Configuration(void);
void TIM2_Configuration(void);
void TIM8_Init_CHx_PWM_Output(TIM8_PWM_CHx_m CHx, uint16_t pwm_period, uint16_t pwm_pluse);
void Timer_Delayms(uint32_t millisecond);

#ifdef __cplusplus
}
#endif

#endif  /*__TIMER__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
