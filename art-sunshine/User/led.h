/**
  ******************************************************************************
  * @file    led.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file contains all the functions prototypes for sytem status  
  *			 led configure and operation.
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
#ifndef __LED__H__
#define __LED__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "stm32f4xx_gpio.h"
#include "timer.h"

/** @addtogroup LED_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
	
/* Exported macro ------------------------------------------------------------*/

/* 
 * macro: @system LED GPIO define.
 */
/* System LED is GPIOH9/GPIOE5/GPIOE6 */
#define SYSTEM_STATUS_LED      GPIO_Pin_9		/* D1 GPIO Pin is  GPIO_Pin_9 */
#define SYSTEM_STATUS_LED_PORT GPIOH	    	/* D1 GPIO Port is GPIOH */

#define TASK_STATUS_LED        GPIO_Pin_5	    /* D2 GPIO Pin is  GPIO_Pin_5 */
#define TASK_STATUS_LED_PORT   GPIOE	        /* D2 GPIO Port is GPIOE */

#define COMMUN_STATUS_LED      GPIO_Pin_6   	/* D3 GPIO Pin is  GPIO_Pin_6 */
#define COMMUN_STATUS_LED_PORT GPIOE			/* D3 GPIO Pin is  GPIOE */

#define SYSTEM_WARN_BEEP_PIN   GPIO_Pin_10		/* Beep GPIO Pin */
#define SYSTEM_WARN_BEEP_PORT  GPIOI			/* Beep GPIO Port */

#define SYSTEM_MOTOR_AB_LOCK_PIN  	GPIO_Pin_15		/* Motor AB Lock GPIO Port */
#define SYSTEM_MOTOR_AB_LOCK_PORT  	GPIOF			/* Motor AB Lock GPIO Pin */

#define SYSTEM_MOTOR_CD_LOCK_PIN  	GPIO_Pin_14		/* Motor CD Lock GPIO Port */
#define SYSTEM_MOTOR_CD_LOCK_PORT  	GPIOF			/* Motor CD Lock GPIO Pin */

/* 
 * macro: @system control LED Status.
 */
#define STATUS_LED_ON()		GPIO_SetBits(SYSTEM_STATUS_LED_PORT, SYSTEM_STATUS_LED)
#define STATUS_LED_OFF()	GPIO_ResetBits(SYSTEM_STATUS_LED_PORT, SYSTEM_STATUS_LED)
#define STATUS_LED_TOGGLE()	GPIO_ToggleBits(SYSTEM_STATUS_LED_PORT, SYSTEM_STATUS_LED)

/* 
 * macro: @Task runing LED Status.
 */
#define TASK_LED_ON()		GPIO_SetBits(TASK_STATUS_LED_PORT, TASK_STATUS_LED)
#define TASK_LED_OFF()		GPIO_ResetBits(TASK_STATUS_LED_PORT, TASK_STATUS_LED)
#define TASK_LED_TOGGLE()	GPIO_ToggleBits(TASK_STATUS_LED_PORT, TASK_STATUS_LED)

/* 
 * macro: @Task communcation LED Status.
 */
#define COMMUN_LED_ON()		GPIO_SetBits(COMMUN_STATUS_LED_PORT, COMMUN_STATUS_LED)
#define COMMUN_LED_OFF()	GPIO_ResetBits(COMMUN_STATUS_LED_PORT, COMMUN_STATUS_LED)
#define COMMUN_LED_TOGGLE()	GPIO_ToggleBits(COMMUN_STATUS_LED_PORT, COMMUN_STATUS_LED)

/* 
 * macro: @System Warning Beep Status.
 */
#define WARN_BEEP_ON()			GPIO_SetBits(SYSTEM_WARN_BEEP_PORT, SYSTEM_WARN_BEEP_PIN)
#define WARN_BEEP_OFF()			GPIO_ResetBits(SYSTEM_WARN_BEEP_PORT, SYSTEM_WARN_BEEP_PIN)
#define WARN_BEEP_TOGGLE()		GPIO_ToggleBits(SYSTEM_WARN_BEEP_PORT, SYSTEM_WARN_BEEP_PIN)

/* 
 * macro: @System Motor AB and CD Lock control.
 */
#define SYSTEM_MOTOR_AB_LOCK_ON()			GPIO_SetBits(SYSTEM_MOTOR_AB_LOCK_PORT, SYSTEM_MOTOR_AB_LOCK_PIN)
#define SYSTEM_MOTOR_AB_LOCK_OFF()			GPIO_ResetBits(SYSTEM_MOTOR_AB_LOCK_PORT, SYSTEM_MOTOR_AB_LOCK_PIN)
#define SYSTEM_MOTOR_AB_LOCK_OFFTOGGLE()	GPIO_ToggleBits(SYSTEM_MOTOR_AB_LOCK_PORT, SYSTEM_MOTOR_AB_LOCK_PIN)

#define SYSTEM_MOTOR_CD_LOCK_ON()			GPIO_SetBits(SYSTEM_MOTOR_CD_LOCK_PORT, SYSTEM_MOTOR_CD_LOCK_PIN)
#define SYSTEM_MOTOR_CD_LOCK_OFF()			GPIO_ResetBits(SYSTEM_MOTOR_CD_LOCK_PORT, SYSTEM_MOTOR_CD_LOCK_PIN)
#define SYSTEM_MOTOR_CD_LOCK_OFFTOGGLE()	GPIO_ToggleBits(SYSTEM_MOTOR_CD_LOCK_PORT, SYSTEM_MOTOR_CD_LOCK_PIN)

/* Exported functions --------------------------------------------------------*/
void Config_GPIO_OUT(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Config_GPIO_IN(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void System_LED_Init(void);
void System_Beep_Init(void);
void System_Motor_Lock_Init(void);
void System_Status_Show(System_Mode mode);

#ifdef __cplusplus
}
#endif

#endif  /*__LED__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
