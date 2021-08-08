/**
  ******************************************************************************
  * @file    lcd.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    13-October-2020
  * @brief   This file contains all  the  functions  prototypes  for  sytem lcd  
  *			 configure and operation.
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
#ifndef __LCD__H__
#define __LCD__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "uart.h"


/** @addtogroup LCD_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
extern SemaphoreHandle_t xLCDBinarySemaphore;	

/* Exported define -----------------------------------------------------------*/
#define LCD_MAXDATA_SZIE 64

/* Exported functions --------------------------------------------------------*/
FunStatus Send_DataTo_LCD(char *data);
void LCD_Init(void);
void lcd_uart_tx_str(char *str);


#ifdef __cplusplus
}
#endif

#endif  /*__LCD__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
