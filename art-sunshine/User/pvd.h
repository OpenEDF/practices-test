/**
  ******************************************************************************
  * @file    pvd.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    27-August-2020
  * @brief   This file contains all the functions prototypes  for  sytem  pvd  
  *			 configure.
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
#ifndef __PVD__H__
#define __PVD__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_pwr.h"

/** @addtogroup IWDG_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/
	
/* Exported macro ------------------------------------------------------------*/
/* @PVD level selection */
#define PVD_LEVEL_2_0		((uint32_t)0x00000000)
#define PVD_LEVEL_2_1		((uint32_t)0x00000020) 	
#define PVD_LEVEL_2_3		((uint32_t)0x00000040)
#define PVD_LEVEL_2_5		((uint32_t)0x00000060)
#define PVD_LEVEL_2_6		((uint32_t)0x00000080)
#define PVD_LEVEL_2_7		((uint32_t)0x000000A0)
#define PVD_LEVEL_2_8		((uint32_t)0x000000C0)
#define PVD_LEVEL_2_9		((uint32_t)0x000000E0)

/* Exported functions --------------------------------------------------------*/
void PVD_Config(uint32_t pvdlevel);

#ifdef __cplusplus
}
#endif

#endif  /*__PVD__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/


