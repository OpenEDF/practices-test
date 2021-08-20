/**
  ******************************************************************************
  * @file    key.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file contains all the functions prototypes for key configure
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
#ifndef __KEY__H__
#define __KEY__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_gpio.h"
#include "led.h"

/** @addtogroup Key_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
	
/* Exported macro ------------------------------------------------------------*/
/* GPIO E0 IS KEY EXIT, TODO: GPIO G0 */
#define KEY_GPIO_Port			GPIOE			/* The system key GPIO Port */
#define KEY_GPIO_Pin			GPIO_Pin_0		/* The system key GPIO Pin */
#define KEY_GPIO_CLK			RCC_AHB1Periph_GPIOE
#define KEY_EXIT_PORTSOURCE		EXTI_PortSourceGPIOE
#define KEY_EXIT_PINSOURCEE		EXTI_PinSource0
#define KEY_EXIT_LINE			EXTI_Line0
#define KEY_EXIT_IRQ			EXTI0_IRQn

/* Exported functions --------------------------------------------------------*/
void EXTI_KEY_Config(void);

#ifdef __cplusplus
}
#endif

#endif  /*__KEY__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
