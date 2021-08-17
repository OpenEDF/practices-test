/**
  ******************************************************************************
  * @file    uart.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file provides all the uart configure and operation 485 functi-
  *			 ons.
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
#include "uart.h"

/** @addtogroup UART_Driver
  * @{
  */
  
/** @defgroup UART 
  * @brief Uart driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
USART_BUFStruct USART_PORTDATA[PORT_NUMS];

/* Private function prototypes -----------------------------------------------*/
static void USARTx_SendByte( USART_TypeDef * USARTx, uint8_t ch);


/* Private functions ---------------------------------------------------------*/
  
/** @defgroup Uart_Private_Functions
  * @{
  */
  
/**
  * @function   USARTx_Config
  * @brief      Configurate the system UART Port.
  * @param[in]  UASRT_Port: USART Port for system define.
  *		        port is set to the USART_485/USART_LTE/USART_DEBUG.
  * @param[in]  baudrate: The usart port baudrate.
  *		        This parameter can be set the 2400, 9600, 115200 etc.
  * @param[in]  parity: parity control, data checks parity.
  *  			This parameter can be set the USART_Parity_No/USART_Parity_Even/USART_Parity_Odd. 
  * @retval     None.
  */   
static void USARTx_Config(USART_Port port, uint32_t baudrate, uint16_t parity)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* CHOOSE AND CONFIG */
	switch (port)
	{
		case USART_485:
		/* config the USART_485  */

		/* RCC Enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		
		/* GPIO Config  */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  ;  
  		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  ;  
  		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

		/* USART Config */
		USART_InitStructure.USART_BaudRate = baudrate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = parity;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART3, &USART_InitStructure);
		
		/* NVIC Config */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/* Enbale USART */
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
		USART_Cmd(USART3, ENABLE);
		break;

		case USART_LTE:
		/* config the USART_LTE  */

		/* RCC Enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
		
		/* GPIO Config  */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  ;  
  		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  ;  
  		GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

		/* USART Config */
		USART_InitStructure.USART_BaudRate = baudrate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = parity;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(UART4, &USART_InitStructure);
		
		/* NVIC Config */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/* Enbale USART */
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
		USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
		USART_Cmd(UART4, ENABLE);

		break;

		case USART_DEBUG:
		/* config the USART_DEBUG  */

		/* RCC Enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		
		/* GPIO Config  */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  ;  
  		GPIO_Init(GPIOB, &GPIO_InitStructure);
 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  ;  
  		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

		/* USART Config */
		USART_InitStructure.USART_BaudRate = baudrate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = parity;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure);
		
		/* NVIC Config */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/* Enbale USART */
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		USART_Cmd(USART1, ENABLE);
		break;

		case USART_LCD:
		/* config the USART_LCD  */

		/* RCC Enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		
		/* GPIO Config  */
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  ;  
  		GPIO_Init(GPIOD, &GPIO_InitStructure);
 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  ;  
  		GPIO_Init(GPIOD, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

		/* USART Config */
		USART_InitStructure.USART_BaudRate = baudrate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = parity;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &USART_InitStructure);
		
		/* NVIC Config */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/* Enbale USART */
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		USART_Cmd(USART2, ENABLE);
		break;

		default: 
		/* error handler */
		/* .... */
		break;

	}
	
}

/**
  * @function   System_USART_Init
  * @brief      Initalizes the system UART.
  * @param[in]  None.
  * @retval     None.
  */  
void System_USART_Init(void)
{
	/* Initalizes the 485 Port */
	USARTx_Config(USART_485, USART_485_BAUDRATE, USART_Parity_No);
	USARTx_485Pin_Config();
	
	/* Initalizes the LTE Port */
	USARTx_Config(USART_LTE, USART_LTE_BAUDRATE, USART_Parity_No);

	/* Initalizes the Debug Port */
	USARTx_Config(USART_DEBUG, USART_DEBUG_BAUDRATE, USART_Parity_No);

	/* Initalizes the LCD Port */
	USARTx_Config(USART_LCD, USART_LCD_BAUDRATE, USART_Parity_No);

	/* Initalize the data buffer */
	for (uint8_t index = USART_485; index < PORT_NUMS; ++index)
	{
		USART_PORTDATA[index].txbuffer = NULL;
		USART_PORTDATA[index].txbuf_length = 0;
		USART_PORTDATA[index].txsent_length = 0;
		USART_PORTDATA[index].rxbuf_count = 0;
		USART_PORTDATA[index].rxtimeout = 0;
	}
}

/**
  * @function   USARTx_SendByte
  * @brief      send a byte data to uart port.
  * @param[in]  USARTx: The UART Port is to send data.
  *				where x can be 1, 2, 3, 4, 5, 6 to select the USART or UART Peripher.
  * @param[in]  ch: the byte data will be send.
  * @retval     None.
  */ 
static void USARTx_SendByte( USART_TypeDef * USARTx, uint8_t ch)
{
	/* send a byte char to USART1 */
	USART_SendData(USARTx, ch);	
	/* wait the send finish */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
}


/**
  * @function   USARTx_Send_Data
  * @brief      send data byte by byte via uart.
  * @param[in]  USARTx: The UART Port is to send data.
  *				where x can be 1, 2, 3, 4, 5, 6 to select the USART or UART Peripher.
  * @param[in]  str: the pointer data will be send.
  * @param[in]  count: the length for data.
  * @retval     None.
  */  
void USARTx_Send_Data(USART_TypeDef* USARTx, uint8_t *str, uint16_t count)
{
	/* send all data */
	uint16_t index = 0;
	do	/* send string */
	{
		USARTx_SendByte(USARTx, *(str + index));
		index++;
	}
	while (index < count);
}

/**
  * @function   USARTx_Send_String
  * @brief      send string via uart.
  * @param[in]  USARTx: The UART Port is to send data.
  *				where x can be 1, 2, 3, 4, 5, 6 to select the USART or UART Peripher.
  * @param[in]  str: the pointer data will be send.
  * @retval     None.
  */ 
void USARTx_Send_String(USART_TypeDef* USARTx, char *str)
{
	uint16_t index = 0;
	do	/* send string */
	{
		USARTx_SendByte(USARTx, *(str + index));
		index++;
	}
	while (*(str + index) != '\0');
}

/**
  * @function   USARTx_485Pin_Config
  * @brief      Configurate the 485 input/output Pin.
  * @param[in]  None.
  * @retval     None.
  */ 
static void USARTx_485Pin_Config(void)
{
	/* Enable the RCC */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	
	/* Config the GPIO */
	Config_GPIO_OUT(RS485_SWITCH_PORT, GPIO_Pin_8);

	/* Set the Receiver Mode */
	RS485_SWITCH_RX();
}

/**
  * @function   fputc
  * @brief      Retargets the C library printf function to the UART.
  * @param[in]  ch and f. default.
  * @retval     None.
  */ 
int fputc(int ch, FILE *f)
{
	/* send the data to uart port buffer */
	USART_SendData(SYSTEM_DEBUG_PORT, (uint8_t)ch);
	while (USART_GetFlagStatus(SYSTEM_DEBUG_PORT, USART_FLAG_TXE) == RESET);

	return (ch);
}

/**
  * @function   fgetc
  * @brief      Retargets the C library scanf and getchar function to the UART.
  * @param[in]  f. default.
  * @retval     None.
  */ 
int fgetc(FILE *f)
{
	/* receiver the data from the uart port buffer */
	while (USART_GetFlagStatus(SYSTEM_DEBUG_PORT, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(SYSTEM_DEBUG_PORT);
}

/**
  * @function   USARTx_Send_Data
  * @brief      send data byte by byte via uart.
  * @param[in]  port: The uart port.
  * @param[in]  USARTx: The UART Port is to send data.
  *				where x can be 1, 2, 3, 4, 5, 6 to select the USART or UART Peripher.
  * @param[in]  timeout: uart recevier data timeout time.
  * @retval     None.
  */  
void USARTx_IRQHandler(USART_Port port, USART_TypeDef* USARTx, uint16_t timeout)
{
	USART_BUFStruct *portstr = &USART_PORTDATA[port];
	uint8_t tempch;
	UBaseType_t uxSavedInterruptStatus;
	
	uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
	/* Receiver the data */
	if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
	{
		COMMUN_LED_ON();
		/* directy receiver data */
		tempch = USART_ReceiveData(USARTx);
		if (portstr->rxbuf_count < (RECEIVER_BUFFER_SIZE - 5))
		{
			portstr->rxbuffer[portstr->rxbuf_count++] = tempch;
			portstr->rxtimeout = timeout;
		}
	}
	/* Send the data */
	if (USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
	{
		COMMUN_LED_ON();
		/* write one byte to the transmit data register */
		USART_SendData(USARTx, portstr->txbuffer[portstr->txsent_length++]);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

		if (portstr->txsent_length == portstr->txbuf_length)
		{
			/* wait the data send finish */
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);

			/* Disable the TX Interrupt */
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
			portstr->txbuf_length = 0;
			portstr->txsent_length = 0;
			COMMUN_LED_OFF();
			
			/* turn off 485 RX */
			if (port == USART_485)
			{
				RS485_SWITCH_RX();
			}
		}
	}
	taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}

/**
  * @function   TimerOneFrame_Data
  * @brief      Use a timer to detemine a complete data frame and send it to
  *				Specified the UART Port.
  * @param[in]  None.
  * @retval     None.
  */
void TimerOneFrame_Data(void)
{
	USART_BUFStruct *uartPtr;
	USART_Port uart_no;
	
	BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
	
	/* Polling operations for each UART port */
	for (uart_no = USART_485; uart_no < PORT_NUMS; uart_no++)
	{
		uartPtr = &USART_PORTDATA[uart_no];
		/* check the timer time */
		if (uartPtr->rxtimeout != 0)
		{
			uartPtr->rxtimeout--;

			if ((uartPtr->rxtimeout == 0) && (uartPtr->rxbuf_count > 0))
			{
				COMMUN_LED_OFF();	/* turn off the communcation LED */
				/* send the data to corresponding queue */
				if (uart_no == USART_485)
				{
					xSemaphoreGiveFromISR(wind_device.xSensoeBinarySemaphore, &pxHigherPriorityTaskWoken);
				}
				
				if (uart_no == USART_LTE)
				{
					/* accroding of the first string check the data property */
					if ((uartPtr->rxbuffer[6] == 'R') && (uartPtr->rxbuffer[9] == 'V'))
					{
						/* Aliyun active publish message. */
						xSemaphoreGiveFromISR(LTE_infomation.LtePublishSemaphore, &pxHigherPriorityTaskWoken);
					}
					else
					{
						/* LTE moudles active respone message. */
						xSemaphoreGiveFromISR(LTE_infomation.LteResponeSemaphore, &pxHigherPriorityTaskWoken);
					}
				}

				if (uart_no == USART_LCD)
				{
					xSemaphoreGiveFromISR(xLCDBinarySemaphore, &pxHigherPriorityTaskWoken);
				}	
			}
		}
	}
}

/**
  * @function   USARTSend_Task
  * @brief      Send the data via UART.
  * @param[in]  pvParameters: default.
  * @retval     None.
  */ 
void USARTSend_Task(void *pvParameters)
{
	PORT_BUF_FORMAT *portBuffer;
	USART_BUFStruct *portStr;
	uint16_t memsize;
	PDEBUG("\r[OK] USARTSend_Task is created.\n");

	/* loop */
	while (TRUE)
	{
		/* receiver the data from Queue xSerialTxQueue */
		if (xQueueReceive(xSerialTxQueue, &portBuffer, pdMS_TO_TICKS(10)) == pdPASS)
		{
			/* check the data is vaild */
			if (portBuffer->port >= PORT_NUMS)
			{
				vPortFree(portBuffer);
 				continue;
			}

			portStr = &USART_PORTDATA[portBuffer->port];
			memsize = calmemsize(portBuffer->length);
	
			/* dynamic allocated memory store the data */
			if (NULL != (portStr->txbuffer = pvPortMalloc(memsize)))
			{
				/* copy the data to uart struct */
				portStr->txbuf_length = portBuffer->length;
				memcpy(portStr->txbuffer, portBuffer->buffer, portStr->txbuf_length);

				/* send the data to uart */
				switch (portBuffer->port)
				{
					case USART_485:
						RS485_SWITCH_TX();
						USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
					break;

					case USART_LTE:
						USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
					break;

					case USART_DEBUG:
						USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
					break;

					case USART_LCD:
						USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
					break;

					default:
						/* ... never */
					break;
				}
			}
			/* free memory */
			vPortFree(portBuffer);
			vPortFree(portStr->txbuffer);
		}
		
		/* Opeartion the IWDG */
		IWDG_Feed();
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/
