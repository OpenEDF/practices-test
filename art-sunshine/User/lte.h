 /**
  ******************************************************************************
  * @file    lte.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file contains  all  the  functions  prototypes  for  the  lte   
  *          operation and configure.
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
#ifndef __LTE__H__
#define __LTE__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "rtc.h"

/** @addtogroup LTE_Driver
  * @{
  */
/* Exported macro ------------------------------------------------------------*/
#define IDENTIFIER_SIZE			8		/* system identifier nums */
#define LTEMAXJOINNETWORK_TIME	(60 * 1000)  /* LTE moudles max tiime for join the network */
#define USERCMD_MAXSIZE			128		/* user command maximum size */

/* Exported types ------------------------------------------------------------*/
/* 
 * enum: @LTE Network Status
 */
typedef enum {
	online = 0,			/* LTE moudle is online */
	offline = !online,	/* LTE moudle is offline */
} LTE_Status;

/* 
 * enum: @Server Control Status
 */
typedef enum {
	control = 0,			/* LTE moudle is online */
	de_control = !control,	/* LTE moudle is offline */
} Server_Ctl;
	
#include "uart.h"
/* 
 * enum: @four operated syntax for Extended syntax
 */
typedef enum {
	Test_cmd = 0,		/* AT+<x>=?	command returns a list of parameters and range. */
	Read_cmd,			/* AT+<x>? command return the current value of paramters. */
	Set_cmd,			/* AT+<x>=<...>set the parameters value. */
	Execute_cmd,		/* AT+<x>  perfprms operations */
} Extend_Type;

/* 
 * enum: @Server Control Status
 */
typedef enum {
	Cmd_data = 0,				/* Command is cpmprise AT command and data  */
	Only_data = !Cmd_data,		/* Command is cpmprise just plain data */
} Data_Type;

/* 
 * struct: @Aliyun iot device model information.
 */
 typedef struct {
 	char *ProductKey;		/* Aliyun iot Product Key */
	char *DeviceName;		/* Aliyun iot Device Name */
 	char *DeviceSecret;		/* Aliyun iot Device Secret */
	char *HostName;			/* Aliyun iot host name */
	char *ServerPort;		/* Aliyun iot server Port<1 - 65536> */
	char *SystemTopic;		/* system publish  topic */
	char *parame_ID[IDENTIFIER_SIZE];	/* system report information identifier */
 } Aliyun_model;

/* 
 * struct: @LTE Moudle parameters and operation struct.
 */
typedef struct {
	Aliyun_model device_info;	/* Aliyun iot device information */
	SemaphoreHandle_t LteResponeSemaphore;	/* lte response Binary Semaphore */
	SemaphoreHandle_t LtePublishSemaphore;	/* lte actived published Binary Semaphore */
} LTE_Module;

/* Exported constants --------------------------------------------------------*/
extern QueueHandle_t xUSARTLTERxQueue;		/* LTE usart queue */
extern LTE_Module LTE_infomation;

/* Exported functions --------------------------------------------------------*/
void LTE_DeInit(void);
static PORT_BUF_FORMAT *LTE_DataPacking(Data_Type type, const char *data);
static FunStatus LTE_DataUnPack(Extend_Type cmd_type, PORT_BUF_FORMAT * rxackbuffer);
FunStatus LTE_TxRxDataProcess(Extend_Type cmd_type, Data_Type type, const char *databuf, PORT_BUF_FORMAT* *ReceiverData);
void LTE_ConnetNetwork(void);
FunStatus LTE_SendMessage(void);
FunStatus LTE_ATE0(void);
FunStatus LTE_ATTest(void);
FunStatus LTE_QueryPIN(void);
FunStatus LTE_QuerySignal(void);
FunStatus LTE_QueryNetStatus(void);
FunStatus LTE_SetAttachPS(void);
FunStatus LTE_QueryPDPAddr(void);
FunStatus LTE_SetMQTT(void);
FunStatus LTE_OpenConMQTTNet(void);
FunStatus LTE_SendData(void);
RTC_Type LTE_QueryRTC(void);
FunStatus LTE_CloseMQTT(void);
void LTEReceiver_Task(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif  /*__LTE__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
