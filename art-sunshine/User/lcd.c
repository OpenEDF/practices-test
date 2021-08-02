/**
  ******************************************************************************
  * @file    lcd.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    13-October-2020
  * @brief   This file provides all the operation and configure functions for 
  *          system lcd show control.
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
#include "lcd.h"

/** @addtogroup LCD_Driver
  * @{
  */
  
/** @defgroup LCD control
  * @brief LCD control driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SemaphoreHandle_t xLCDBinarySemaphore = NULL;


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static FunStatus Check_Result(void);

/** @defgroup LCD_Functions
  * @{
  */
  
/**
  * @function   Send_DataTo_LCD
  * @brief      Send the data and command to LCD Device via uart.
  * @param[in]  data: show data.        
  * @retval     FunStatus: Function execution results.
  * 			@R_OK: Indicates successful execution.
  *				@R_ERROR: Indicates failed execution.
  */  
FunStatus Send_DataTo_LCD(char *data)
{
	char *txbuffer;
	uint8_t templen;
	/* allocated the memory */
	if (NULL == (txbuffer = pvPortMalloc(LCD_MAXDATA_SZIE)))
	{
		PDEBUG("No more memory to be allocate.\n");
		return R_ERROR;
	}

	templen = strlen(data);
	/* check parameters */
	if ((templen > 100) || (data == NULL))
	{
		vPortFree(txbuffer);
		return R_ERROR;
	}
		
	/* Frames the data */
	memcpy(txbuffer, data, templen);
	strcat(txbuffer, "\r\n");
	
	/* Send to queue */
	if(xQueueSendToBack(xSerialTxQueue, &txbuffer, pdMS_TO_TICKS(100)) != pdPASS)
	{
		PDEBUG("LCD Queue send message failed.\n");
		return R_ERROR;
	}
	
	/* Check the communcation result */
	if (Check_Result() == R_ERROR)
		return R_ERROR;
	
	/* Return */
	return R_OK;
}

/**
  * @function   LCD_Init
  * @brief      Initalizate the lcd device.
  * @param[in]  None.        
  * @retval     None.
  */  
void LCD_Init(void)
{
	/* create the binary semphore */
	xLCDBinarySemaphore = xSemaphoreCreateBinary();
	/* check cerated */
	if (xLCDBinarySemaphore == NULL)
	{
		PDEBUG("The lcd xLCDBinarySemaphore is Create Failed.\n");
	}
	else
	{
		PDEBUG("xLCDBinarySemaphore is Created.\n");
	}

	/* send the command set device */
	Send_DataTo_LCD("CLR(0);"); /* background */
	Send_DataTo_LCD("DIR(1);"); /* directly */
	Send_DataTo_LCD("BL(0);");  /* lighting */
	Send_DataTo_LCD("SBC(0);"); /* under color */

	/* show the 'Loading...' */
	Send_DataTo_LCD("DCV32(120,80,'Loading...',2");

	/*
		+++++++++++++++++++++++++++
	    +                	      +
	    +     	 Loading...		  +
	    +        				  +
	    +++++++++++++++++++++++++++
	*/

}

/**
  * @function   Check_Result
  * @brief      check the communcation result from the LCD device.
  * @param[in]  None.        
  * @retval     FunStatus: Function execution results.
  * 			@R_OK: Indicates successful execution.
  *				@R_ERROR: Indicates failed execution.
  */ 
static FunStatus Check_Result(void)
{
	USART_BUFStruct *uartPtr;
	uartPtr = &USART_PORTDATA[USART_LCD];	
	FunStatus ret = R_ERROR;
	uint8_t loop_times;

	/* Tack the Binary semaphore */
	loop_times = 3;
	while (loop_times--)
	{
		if (xSemaphoreTake(xLCDBinarySemaphore, pdMS_TO_TICKS(100)) != pdPASS)
		{
			PDEBUG("LCD Receiver message failed.\n");
			continue;
		}
		else
		{
			/* check the result from receiver buffer */
			if (NULL != strstr((const char *)uartPtr->rxbuffer, "OK")) 
				ret = R_OK;
		}
	}
	
	/* give the Binary semaphore and clean the buffer*/
	xSemaphoreGive(xLCDBinarySemaphore);
	memset(uartPtr->rxbuffer, 0x00, uartPtr->rxbuf_count);
	uartPtr->rxbuf_count = 0;

	/* return */
	return ret;
}

/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/
