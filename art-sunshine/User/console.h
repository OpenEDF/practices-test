/**
  ******************************************************************************
  * @file    console.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file contains  all  the  functions  prototypes  for  control 
  *			 console and operation.
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
#ifndef __CONSOLE__H__
#define __CONSOLE__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/** @addtogroup Console_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* 
 * enum: @console control Working Status
*/
typedef enum {
	normal = 0,			/* console control is normal */
	fault = !normal,	/* console control is fault */
} CONSOLE_Status;

/* Exported constants --------------------------------------------------------*/
	
/* Exported macro ------------------------------------------------------------*/
#define CONSOLE_NUMS	6		/* system console nums */

/* Exported functions --------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif  /*__CONSOLE__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
