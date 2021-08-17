/**
  ******************************************************************************
  * @file    uart.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file contains all the functions prototypes for the uart port  
  *          read-write and configure.
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
#ifndef __UART__H__
#define __UART__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_usart.h"
#include "common.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lte.h"
#include "sensor.h"
#include "iwdg.h"
#include "lcd.h"


/** @addtogroup Sensor_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* 
 * enum: @USART Port
 */
typedef enum {
	USART_485 = 0,		/* wind speed sensor and control console */
	USART_LTE,			/* 4G LTE moudle */
	USART_DEBUG,		/* System Debug */
	USART_LCD,			/* Uart LCD moudles */
	PORT_NUMS,			/* System ports nums */
} USART_Port;

/* 
 * macro: @usart receiver buffer size.
 */
#define RECEIVER_BUFFER_SIZE	128	

/* 
 * struct: @USART transmit and received data struct
 */
typedef struct {
	uint8_t *txbuffer;			/* pointer for send data */
	uint16_t txbuf_length;		/* length of send data */
	uint16_t txsent_length;		/* length of data sent */
	uint8_t rxbuffer[RECEIVER_BUFFER_SIZE];			/* pointer for receiver data */
	uint16_t rxbuf_count;		/* count of receiver data */
	uint8_t  rxtimeout;			/* timeout of receiver data time */
} USART_BUFStruct;

/* Exported macro ------------------------------------------------------------*/
/* 
 * macro: @USART Port Baudrate
 */
#define USART_DEBUG_BAUDRATE	115200	/* debug port baudrate */
#define USART_485_BAUDRATE		9600    /* 485 port baudrate */
#define USART_LTE_BAUDRATE		115200	/* lte moudle baudrate */
#define USART_LCD_BAUDRATE		115200	/* lcd moudle baudrate */

/* 
 * macro: @DEBUG_PORT
 */
#define SYSTEM_DEBUG_PORT	    USART1	/* debug port is USART1 */

/* 
 * macro: @USART 485 Switch Pin.
 */
#define RS485_SWITCH_PORT		GPIOH		/* 485 txrx control pin */
#define RS485_SWITCH_PIN 		GPIO_Pin_8
#define RS485_SWITCH_TX()		GPIO_SetBits(RS485_SWITCH_PORT, RS485_SWITCH_PIN)
#define RS485_SWITCH_RX()		GPIO_ResetBits(RS485_SWITCH_PORT, RS485_SWITCH_PIN)

/* 
 * macro: @uart receiver timer timeout time.
 */
#define DEBUGUSART_TIMEOUT	50		/* Debug uart receiver timeout */
#define LETUSART_TIMEOUT	500		/* LTE uart receiver timeout */
#define RS485USART_TIMEOUT	100		/* RS485 uart receiver timeout */
#define LCDUSART_TIMEOUT	100		/* lcd receiver timeout */

/* Exported constants --------------------------------------------------------*/
/* UART Port data struct */	
extern USART_BUFStruct USART_PORTDATA[PORT_NUMS];
extern QueueHandle_t xSerialTxQueue;
extern SemaphoreHandle_t xSerial485Mutex;

/* Exported functions --------------------------------------------------------*/
static void USARTx_Config(USART_Port port, uint32_t baudrate, uint16_t parity);
void System_USART_Init(void);
void USARTx_Send_Data(USART_TypeDef* USARTx, uint8_t *str, uint16_t count);
void USARTx_Send_String(USART_TypeDef* USARTx, char *str);
static void USARTx_485Pin_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
void USARTx_IRQHandler(USART_Port port, USART_TypeDef* USARTx, uint16_t timeout);
void TimerOneFrame_Data(void);
void USARTSend_Task(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif  /*__UART__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/

