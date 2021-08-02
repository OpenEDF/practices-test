/**
  ******************************************************************************
  * @file    led.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file provides all the operation and configure functions for 
  *          system status led control.
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

/* Includes ------------------------------------------------------------------*/
#include "led.h"

/** @addtogroup LED_Driver
  * @{
  */
  
/** @defgroup LED control
  * @brief LED control driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
  
/** @defgroup LED_Functions
  * @{
  */
  
/**
  * @function   Config_GPIO_OUT
  * @brief      Configurate the GOIP to output.
  * @param[in]  GPIOx: GPIO Port.
  *		        where x can be (A..K) to select the GPIO peripheral for STM32F405xx/407xx devices.
  * @param[in]  GPIO_Pin: GPIO Pin.
  *		        This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval     None.
  */  
void Config_GPIO_OUT(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Configure the GPIO struct */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	/* Initalizes the GPIO Pin work. */
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/**
  * @function   Config_GPIO_IN
  * @brief      Configurate the GPIO to input.
  * @param[in]  GPIOx: GPIO Port.
  *		        where x can be (A..K) to select the GPIO peripheral for STM32F405xx/407xx devices.
  * @param[in]  GPIO_Pin: GPIO Pin.
  *		        This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @retval     None.
  */  
void Config_GPIO_IN(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Configure the GPIO struct */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/* Initalizes the GPIO Pin work. */
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}


/**
  * @function   System_LED_Init
  * @brief      Configurate the system LED.
  * @param[in]  None.
  * @retval     None.
  */ 
void System_LED_Init(void)
{
	/* Enable the GPIO Port Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* configure the LED */
	Config_GPIO_OUT(SYSTEM_STATUS_LED_PORT, SYSTEM_STATUS_LED);
	Config_GPIO_OUT(TASK_STATUS_LED_PORT, TASK_STATUS_LED);
	Config_GPIO_OUT(COMMUN_STATUS_LED_PORT, COMMUN_STATUS_LED);

	/* Initalizes LED Status */
	STATUS_LED_OFF();
	TASK_LED_OFF();
	COMMUN_LED_OFF();
}

/**
  * @function   System_Beep_Init
  * @brief      Configurate the system Beep Pin.
  * @param[in]  None.
  * @retval     None.
  */
void System_Beep_Init(void)
{
	/* Enable the GPIO Port Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);

	/* configure the LED */
	Config_GPIO_OUT(SYSTEM_WARN_BEEP_PORT, SYSTEM_WARN_BEEP_PIN);

	/* Init Status */
	WARN_BEEP_ON();
	simple_delay(0x8FFFFF);
	WARN_BEEP_OFF();
}


/**
  * @function   System_Status_Show
  * @brief      Configure the LED blink frequency to indicate system working status.
  * @param[in]  System_Mode: system work mode.
  *		        The sysytem mode can be set the follow:
  *				@NORMAL_MODE    blink 1000ms on and 1000ms off.
  *				@EXCEPTION_MODE blink 500ms on and 500ms off.
  *				@SLEEP_MODE		blink 500ms on and 5000ms off.
  *				@CLEAR_MODE		LED turn off.
  * @retval     None.
  */ 
void System_Status_Show(System_Mode mode)
{
	/* set the led bilnk frequency */
	switch (mode)
	{
		case NORMAL_MODE:	/* normal mode  */
			if ((timer_counter % 2000) == 0)
				STATUS_LED_TOGGLE();
		break;

		case EXCEPTION_MODE:	/* exception mode  */
			if ((timer_counter % 200) == 0)
				STATUS_LED_TOGGLE();
		break;

		case SLEEP_MODE:	/* exception mode  */
			if ((timer_counter % 500) == 0)
				STATUS_LED_OFF();
			else if ((timer_counter % 5000) == 0)
				STATUS_LED_ON();
		break;

		case CLEAR_MODE:		/* clear mode  */
		default:	
			STATUS_LED_OFF();
		break;
	}
}


/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/

