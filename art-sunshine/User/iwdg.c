/**
  ******************************************************************************
  * @file    iwdg.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    27-August-2020
  * @brief   This file provides all the operation and configure functions for 
  *          system IWDG config.
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
#include "iwdg.h" 

/** @addtogroup Flash_Driver
  * @{
  */
  
/** @defgroup Flash control
  * @brief IWDG control driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
  
/** @defgroup Flash_Functions
  * @{
  */
  
/**
  * @function	IWDG_Config
  * @brief		Config the system iwdg.
  * @param[in]	prv: Prescaler divider.
  * @param[in]	reloadcount: Watchdog counter reload value.
  * @retval     None.
  */  
void IWDG_Config(uint8_t prv, uint16_t reloadcount)
{
	/* Enables or disables write access to IWDG_PR and IWDG_RLR registers. */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* set the Prescaler value. */
	IWDG_SetPrescaler(prv);
	
	/* set the counter reload value. */
	IWDG_SetReload(reloadcount);

	/* reloade */
	IWDG_ReloadCounter();

	/* enable IWDG */
	IWDG_Enable();
}

/**
  * @function	IWDG_Feed
  * @brief		Reloads IWDG counter and restart watchdog.
  * @param[in]	None.
  * @retval     None.
  */  
void IWDG_Feed(void)
{
	/* reload and down count*/
	IWDG_ReloadCounter();
}

/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/
