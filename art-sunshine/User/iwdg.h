/**
  ******************************************************************************
  * @file    iwdg.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    27-August-2020
  * @brief   This file contains all the functions prototypes  for  sytem  IWDG  
  *			 configure and reload operation.
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
#ifndef __IWDG__H__
#define __IWDG__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_iwdg.h"

/** @addtogroup IWDG_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/
	
/* Exported macro ------------------------------------------------------------*/
#define IWDG_TIMEOUT_500MS	IWDG_Prescaler_64, 0x139	/* idwg timeout is 500ms */
#define IWDG_TIMEOUT_1S		IWDG_Prescaler_64, 0x271	/* idwg timeout is 1s */
#define IWDG_TIMEOUT_2S		IWDG_Prescaler_64, 0x4E2	/* idwg timeout is 2s */
#define IWDG_TIMEOUT_4S		IWDG_Prescaler_64, 0x9C4 	/* idwg timeout is 4s */


/* Exported functions --------------------------------------------------------*/
void IWDG_Config(uint8_t prv, uint16_t reloadcount);
void IWDG_Feed(void);

#ifdef __cplusplus
}
#endif

#endif  /*__IWDG__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
