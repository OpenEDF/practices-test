/**
  ******************************************************************************
  * @file    lte.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file provides all the LTE configure and operation functions.
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
#include "lte.h"
/** @addtogroup LTE_Driver
  * @{
  */
  
/** @defgroup LTE 
  * @brief LTE driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
QueueHandle_t xUSARTLTERxQueue;
LTE_Module LTE_infomation;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
  
/** @defgroup LTE_Private_Functions
  * @{
  */
/**
* @function  LTEReceiver_Task
* @brief	 LTE Moudle passive receiver data from seriver.
* @param[in] pvParameters: default.
* @retval    None.
*/
void LTEReceiver_Task(void *pvParameters)
{
	USART_BUFStruct *usartbuffer;
	char cmdbuffer[USERCMD_MAXSIZE];

	/* Receiver the data from the Server */
	while(TRUE)
	{
		if ((xSemaphoreTake(LTE_infomation.LtePublishSemaphore ,portMAX_DELAY) == pdTRUE))
		{
			PDEBUG("The LTE moudles Receiver the Data.\n");			

			/* Handler command from the server Active control */
			usartbuffer = &USART_PORTDATA[USART_LTE];
			usartbuffer->rxbuffer[usartbuffer->rxbuf_count++] = '\0';
			PDEBUG("LTE Receiver Command: %s\n", usartbuffer->rxbuffer);

			/* copy the command to array */
			if (usartbuffer->rxbuf_count < USERCMD_MAXSIZE)
			{
				memcpy(cmdbuffer, usartbuffer->rxbuffer, usartbuffer->rxbuf_count);

				/* clear the uasr buffer ready receiver the new message */
				memset(usartbuffer->rxbuffer, 0x00, usartbuffer->rxbuf_count);
				usartbuffer->rxbuf_count = 0;
			}

			/* Handing the specific commands */
			if (strstr(cmdbuffer, "Enter") != NULL)
			{
				/* Receiver the User command: Enter the exception mode */
				Art_Sunshine_Info.mode = EXCEPTION_MODE;
				Art_Sunshine_Info.serctl_status = control;
				PDEBUG("Set the exception mode for user command");
			}
			else if (strstr(cmdbuffer, "Exit") != NULL)
			{
				/* Receiver the User command: Exit the exception mode */
				Art_Sunshine_Info.mode = NORMAL_MODE;
				Art_Sunshine_Info.serctl_status = de_control;
				PDEBUG("Set the normal mode for user command");
			}
			else
			{
				/* command is invaild */
				PDEBUG("The LTE moudles Received invaild Data.\n");
			}	
		}

		/* given back the Sempher */
		memset(cmdbuffer, 0x00, USERCMD_MAXSIZE);
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

/**
  * @function  LTE_DeInit
  * @brief	   LTE moudles initalized and set the set the default parameters.
  * @param[in] None.
  * @retval    None
  */
void LTE_DeInit(void)
{
	/* set the Aliyun device information */
	LTE_infomation.device_info.ProductKey = "\"a1iNmsdxpkR\"";
	LTE_infomation.device_info.DeviceName = "\"Environment_device\"";
	LTE_infomation.device_info.DeviceSecret = "\"273c68e5b71ee5e5d77c3eb85e39d077\"";
	LTE_infomation.device_info.HostName = "\"iot-as-mqtt.cn-shanghai.aliyuncs.com\"";
	LTE_infomation.device_info.ServerPort = "1883";
	LTE_infomation.device_info.SystemTopic = "\"/sys/a1iNmsdxpkR/Environment_device/thing/event/property/post\"";
	LTE_infomation.device_info.parame_ID[0] = "WindSpeed:";
	LTE_infomation.device_info.parame_ID[1] = "DevicePower:";

	/* create the Semaphore */
	LTE_infomation.LteResponeSemaphore = xSemaphoreCreateBinary();
	if (LTE_infomation.LteResponeSemaphore == NULL)
	{
		PDEBUG("The LTE LteResponeSemaphore is Create Failed.\n");
	}
	else
	{
		PDEBUG("The LTE LteResponeSemaphore is Created.\n");
	}

	LTE_infomation.LtePublishSemaphore = xSemaphoreCreateBinary();
	if (LTE_infomation.LtePublishSemaphore == NULL)
	{
		PDEBUG("The LTE LtePublishSemaphore is Create Failed.\n");
	}
	else
	{
		PDEBUG("The LTE LtePublishSemaphore is Created.\n");
	}
}

/**
  * @function  LTE_DataPacking
  * @brief	   Packing a complete frame data for AT command.
  * @param[in] type: command comprise type.
  *			   @Cmd_data: AT + data.
  *			   @Only_data: just plain data.

  * @param[in] data: Data appended to the AT command.
  * @retval    a complete frame data.
  */
static PORT_BUF_FORMAT *LTE_DataPacking(Data_Type type, const char *data)
{
	PORT_BUF_FORMAT *frame_data;
	uint16_t memsize, length;
	
	/* calculater the data length and memory */
	length = strlen(data);
	memsize =calmemsize(length + 16);
	
	/* dynamic allocated memory */
	if ((frame_data = pvPortMalloc(memsize)) == NULL)
	{
		PDEBUG("No more memeory to be allocated.\n");
		return NULL;
	}

	/* according the command type packing the frame data */
	frame_data->port = USART_LTE;
	
	/* check data type */
	if (type == Cmd_data)
	{	
		strcpy((char *)frame_data->buffer, data);
		frame_data->length = length;
		frame_data->buffer[frame_data->length++] = 0x0D;
		frame_data->buffer[frame_data->length++] = 0x0A;
	}
	else
	{
		strcpy((char *)frame_data->buffer, "{params:");
		frame_data->length = 9;
		/* copy the use data */
		strcpy((char *)frame_data->buffer + frame_data->length, data);
		frame_data->length += length;
		strcpy((char *)frame_data->buffer + frame_data->length, "}");
		frame_data->length++;
		frame_data->buffer[frame_data->length++] = 0x1A;
		frame_data->buffer[frame_data->length++] = 0x0D;
		frame_data->buffer[frame_data->length++] = 0x0A;
	}
	return frame_data;
}

/**
  * @function  LTE_DataUnPack
  * @brief	   LTE moudles unpackage data.
  * @param[in] PORT_BUF_FORMAT: data buffer ibe unpackage.
  * @param[in] cmd_type: AT Command type.
  *			   detail information context@Extend_Type.
  * @retval    Function Operation result.
  *			   @R_OK: buffer is unpackage successful.
  *			   @R_ERROR: buffer is unpackage failed.
  */
static FunStatus LTE_DataUnPack(Extend_Type cmd_type, PORT_BUF_FORMAT * rxackbuffer)
{
	FunStatus status;
	status = R_ERROR;
	
	/* check the buffer vaild */
	if (rxackbuffer == NULL)
	{
		PDEBUG("LTE UART Receiver buffer is no any data.\n");
		return R_ERROR;
	}

	/* Find the OK character for each command type */
	if (NULL == strstr((char *)rxackbuffer->buffer, "OK"))
	{
		PDEBUG("LTE UART Receiver buffer is no OK.\n");
		return R_ERROR;
	}

	/* check the command  */
	switch (cmd_type)
	{
		case Test_cmd:		/* only OK is indiacte Successfuly*/
		case Set_cmd:
			status = R_OK;
		break;
		
		case Read_cmd:		/* meassage contant value of parameters */
		case Execute_cmd:
			if (rxackbuffer->length > 4)
				status = R_OK;
			else
				status = R_ERROR;
		break;

		default:			/* other is error */
			status = R_ERROR;
		break;
	}
	return status;
}

/**
  * @function  LTE_TxRxDataProcess
  * @brief	   LTE moudles unpackage data.
  * @param[in] cmd_type: AT command data type.
  * @param[in] databuf: data buffer is be send.
  * @param[in][out] receiverdata: LTE response data pointer.
  * @retval   Function Operation result.
  *			  @R_OK: buffer is unpackage successful.
  *			  @R_ERROR: buffer is unpackage failed.
  */
FunStatus LTE_TxRxDataProcess(Extend_Type cmd_type, Data_Type type, const char *databuf, PORT_BUF_FORMAT * *ReceiverData)
{
	PORT_BUF_FORMAT *txuartbuffer, *rxuartbuffer;
	USART_BUFStruct *uartPtr;
	uint8_t loop_times;
	uint16_t memsize;
	
	/* framing a cpmplete data */
	txuartbuffer = LTE_DataPacking(type, databuf);
	txuartbuffer->buffer[txuartbuffer->length++] = '\0';
	PDEBUG("LTE SEND DATA: %s", txuartbuffer->buffer);
	
	/* send the data to queue  */
	if(xQueueSendToBack(xSerialTxQueue, &txuartbuffer, pdMS_TO_TICKS(100)) != pdPASS)
	{
		PDEBUG("UART_LTE Queue SEND message failed.\n");
		return R_ERROR;
	}

	/* Delay 100ms */
	vTaskDelay(pdMS_TO_TICKS(500));
	loop_times = 3;
	/* receiver the data frame */
	while(loop_times--)
	{
		if (xSemaphoreTake(LTE_infomation.LteResponeSemaphore, pdMS_TO_TICKS(100)) != pdPASS)
		{
			PDEBUG("UART_LTE Queue RECEIVER message failed.\n");
			continue;
		}
		else
		{
			/* Read the data from UART struct */
			uartPtr = &USART_PORTDATA[USART_LTE];
			memsize = calmemsize(uartPtr->rxbuf_count + 16);

			/* Check for special characters ">" 0x3E ASCII */
			if (uartPtr->rxbuffer[2] == 0x3E)
			{
				/* Jump out */
				uartPtr->rxbuffer[uartPtr->rxbuf_count++] = '\0';
				PDEBUG("LTE ACK DATA: %s", uartPtr->rxbuffer);
				memset(uartPtr->rxbuffer, 0x00, uartPtr->rxbuf_count);
				uartPtr->rxbuf_count = 0;
				return R_OK;
			}

			/* dynamic allocated memory store the data */
			if (NULL != (rxuartbuffer = pvPortMalloc(memsize)))
			{
				/* copy the data to buffer */
				rxuartbuffer->port = USART_LTE;
				rxuartbuffer->length = uartPtr->rxbuf_count;
				memcpy(rxuartbuffer->buffer, uartPtr->rxbuffer, rxuartbuffer->length);
				rxuartbuffer->buffer[uartPtr->rxbuf_count++] = '\0';
				PDEBUG("LTE ACK DATA: %s", rxuartbuffer->buffer);

				/* clear and set 0 to uart[LTE] buffer */
				memset(uartPtr->rxbuffer, 0x00, uartPtr->rxbuf_count);
				uartPtr->rxbuf_count = 0;
			}
		}
		/* first check data according the cmd_type */
		if (R_ERROR == LTE_DataUnPack(cmd_type, rxuartbuffer))
		{
			vPortFree(rxuartbuffer);
			xSemaphoreGive(LTE_infomation.LteResponeSemaphore);
			PDEBUG("LTE UART Receiver data Error.\n");
			return R_ERROR;
		}

		/* according the ReceiverData Handler data */
		if (ReceiverData != NULL)
		{
			*ReceiverData = rxuartbuffer;
			xSemaphoreGive(LTE_infomation.LteResponeSemaphore);
			return  R_OK;
		}
		else
		{
			/* free memory */
			vPortFree(rxuartbuffer);
			xSemaphoreGive(LTE_infomation.LteResponeSemaphore);
			return  R_OK;
		}	
	}
	return R_ERROR;
}

/**
  * @function  LTE_ConnetNetwork
  * @brief	   LTE moudles connect the network and Registered device.
  * @param[in] None.
  * @retval    None.
  */
void LTE_ConnetNetwork(void)
{
	uint32_t start_time, end_time;

	start_time = timer_counter;
	end_time = timer_counter + LTEMAXJOINNETWORK_TIME;
	
	/* SET THE PARAMETERS AND JOIN THE NETWORK */
	while(start_time < end_time)
	{
		/* Queries the AT */
		if (LTE_ATTest() != R_OK)
		{
			Art_Sunshine_Info.lte_status = offline; 
			vTaskDelay(pdMS_TO_TICKS(1000));
			continue;
		}
		vTaskDelay(pdMS_TO_TICKS(500));
		
		/* Queries PIN */
		if ( LTE_QueryPIN() != R_OK)
		{
			Art_Sunshine_Info.lte_status = offline; 
			vTaskDelay(pdMS_TO_TICKS(1000));
			continue;
		}
		vTaskDelay(pdMS_TO_TICKS(500));
		
		/* Queries CSQ */
		if ( LTE_QuerySignal() != R_OK)
		{
			Art_Sunshine_Info.lte_status = offline; 
			vTaskDelay(pdMS_TO_TICKS(1000));
			continue;
		}
		vTaskDelay(pdMS_TO_TICKS(500));
		
		/* Write the CGATT and Queries */
		if (LTE_QueryNetStatus() != R_OK)
		{
			Art_Sunshine_Info.lte_status = offline; 
			vTaskDelay(pdMS_TO_TICKS(1000));
			continue;
		}
		vTaskDelay(pdMS_TO_TICKS(500));
		
		/* Attachment the PS */
		if (LTE_SetAttachPS() != R_OK)
		{
			Art_Sunshine_Info.lte_status = offline; 
			vTaskDelay(pdMS_TO_TICKS(1000));
			continue;
		}
		vTaskDelay(pdMS_TO_TICKS(500));

		/* Get the IP Address */
		if (LTE_QueryPDPAddr() != R_OK)
		{
			Art_Sunshine_Info.lte_status = offline; 
			vTaskDelay(pdMS_TO_TICKS(500));
			continue;
		}
		vTaskDelay(pdMS_TO_TICKS(1000));

		/* Config the MQTT information */
		if (LTE_SetMQTT() != R_OK)
		{
			LTE_CloseMQTT();
			Art_Sunshine_Info.lte_status = offline; 
			vTaskDelay(pdMS_TO_TICKS(1000));
			continue;
		}
		vTaskDelay(pdMS_TO_TICKS(500));

		/* open and connection the MQTT Network */
		if (LTE_OpenConMQTTNet() != R_OK)
		{
			Art_Sunshine_Info.lte_status = offline; 
			vTaskDelay(pdMS_TO_TICKS(1000));
			continue;
		}
		/* active exit the while loop */
		break;
	}
	
	/* check the time judge wheather connection */	
	if(start_time < end_time)	
	{
		Art_Sunshine_Info.lte_status = online; 
		
		/* show to the lcd */
		Send_DataTo_LCD("CIRF(300,132,8,2);");
		PDEBUG("LTE Device connection successful!\n");
	}
	else
	{
		Art_Sunshine_Info.lte_status = offline; 
		/* show to the lcd */
		Send_DataTo_LCD("CIRF(300,132,8,1);");
		
		PDEBUG("LTE Device connection Failure!\n");
		PDEBUG("Please Check it!\n");

		/* Reset until the device is connected to the network. */
		NVIC_SystemReset();
	}	
}

/**
  * @function  LTE_SendMessage
  * @brief	   LTE moudles send data to server.
  * @param[in] message: message will be send.
  * @retval   Function Operation result.
  *			  @R_OK: data send successful.
  *			  @R_ERROR: data send failure.
  */
FunStatus LTE_SendMessage(void)
{
	FunStatus result;
	result = LTE_SendData();

	/* check and update status */
	if (result != R_OK)
	{
		Art_Sunshine_Info.lte_status = offline;
		PDEBUG(" LTE Device is not connected to the network.\n");
	}

	return result;
}

/**
  * @function  LTE_ATE0
  * @brief	   LTE moudles send ATE0 command.
  * @param[in] None.
  * @retval   Function Operation result.
  *			  @R_OK: data send successful.
  *			  @R_ERROR: data send failure.
  */
FunStatus LTE_ATE0(void)
{
	FunStatus result;
	result = R_ERROR;

	/* send the AT */
	result = LTE_TxRxDataProcess(Test_cmd, Cmd_data, "ATE0", NULL);

	/* return the result */
	return result;
}


/**
  * @function  LTE_ATTest
  * @brief	   LTE moudles send AT test command to server.
  * @param[in] None.
  * @retval   Function Operation result.
  *			  @R_OK: data send successful.
  *			  @R_ERROR: data send failure.
  */
FunStatus LTE_ATTest(void)
{
	FunStatus result;
	result = R_ERROR;

	/* send the AT */
	result = LTE_TxRxDataProcess(Test_cmd, Cmd_data, "ATE0", NULL);
	vTaskDelay(pdMS_TO_TICKS(500));
	result = LTE_TxRxDataProcess(Test_cmd, Cmd_data, "AT", NULL);

	/* return the result */
	return result;
}

/**
  * @function  LTE_QueryPIN
  * @brief	   LTE moudles send AT+CPIN command to server.
  * @param[in] None.
  * @retval   Function Operation result.
  *			  @R_OK: data send successful.
  *			  @R_ERROR: data send failure.
  */
FunStatus LTE_QueryPIN(void)
{
	FunStatus result;
	PORT_BUF_FORMAT *ackbuffer;
	result = R_ERROR;

	/* send the AT and set Echo */
	result = LTE_TxRxDataProcess(Read_cmd, Cmd_data, "AT+CPIN?", &ackbuffer);
	if (result == R_ERROR)
		return result;
	
	/* find the READY */
	if (NULL == strstr((char *)ackbuffer->buffer, "READY"))
	{
		PDEBUG("AT+CPIN Cmmand ACK ERROR.\n");
		result = R_ERROR;
	}
	else
		result = R_OK;
	/* return the result */
	return result;
}

/**
  * @function  LTE_QuerySignal
  * @brief	   LTE moudles send AT+CSQ command to server.
  * @param[in] None.
  * @retval   Function Operation result.
  *			  @R_OK: data send successful.
  *			  @R_ERROR: data send failure.
  */
FunStatus LTE_QuerySignal(void)
{
	FunStatus result;
	PORT_BUF_FORMAT *ackbuffer;
	result = R_ERROR;
	char ch[4] = {0x0A, 0x0A, 0x0A, 0x0A};
	uint8_t rssi;

	/* send the AT and set Echo */
	result = LTE_TxRxDataProcess(Execute_cmd, Cmd_data, "AT+CSQ", &ackbuffer);
	if (result == R_ERROR)
		goto LABEL;

	/* find the rssi */
	Extractstr_fromstr((char *)ackbuffer->buffer, "+CSQ:", ",", ch);
	if (ch[3] == 0x0A)
	{
		rssi = ((ch[1] - 48) * 10) + (ch[2] - 48);
	}
	else if (ch[2] == 0x0A)
	{
		rssi = (ch[1] - 48);
	}
	else if (ch[0] == 0x0A)
	{
		PDEBUG("The LTE Rssi Value Error.\n");
	}

	/* check rssi */
	if (rssi > 191)
		result = R_ERROR;
	else
		result = R_OK;
	
LABEL:
	/* return the result */
	vPortFree(ackbuffer);
	return result;
}

/**
  * @function  LTE_QueryNetStatus
  * @brief	   LTE moudles send AT+CREG? command to server.
  * @param[in] None.
  * @retval   Function Operation result.
  *			  @R_OK: data send successful.
  *			  @R_ERROR: data send failure.
  */
FunStatus LTE_QueryNetStatus(void)
{
	FunStatus result;
	PORT_BUF_FORMAT *ackbuffer;
	result = R_ERROR;

	/* send the AT Command */
	result = LTE_TxRxDataProcess(Test_cmd, Cmd_data, "AT+CREG?", &ackbuffer);
	if (result == R_ERROR)
		goto LABEL;

	/* check the network status */
	if ((ackbuffer->buffer[11] - 48) == 1)
		result = R_OK;
	else
		result = R_ERROR;
	
LABEL:
	/* return the result */
	vPortFree(ackbuffer);
	return result;
}

/**
  * @function  LTE_QueryNetStatus
  * @brief	   LTE moudles send AT+CGATT=1 command to server.
  * @param[in] None.
  * @retval   Function Operation result.
  *			  @R_OK: data send successful.
  *			  @R_ERROR: data send failure.
  */
FunStatus LTE_SetAttachPS(void)
{
	FunStatus result;
	PORT_BUF_FORMAT *ackbuffer;
	result = R_ERROR;

	/* send the AT Command */
	result = LTE_TxRxDataProcess(Set_cmd, Cmd_data, "AT+CGATT=1", NULL);
	if (result == R_ERROR)
		return result;

	vTaskDelay(pdMS_TO_TICKS(1000));
	result = LTE_TxRxDataProcess(Read_cmd, Cmd_data, "AT+CGATT?", &ackbuffer);
	if (result == R_ERROR)
		goto LABEL;
	
	/* check the PS status */
	if ((ackbuffer->buffer[10] - 48) == 1)
		result = R_OK;
	else
		result = R_ERROR;	
LABEL:
	/* return the result */
	vPortFree(ackbuffer);
	return result;
}

/**
  * @function  LTE_QueryPDPAddr
  * @brief	   LTE moudles send AT+CGPADDR command to server.
  * @param[in] None.
  * @retval   Function Operation result.
  *			  @R_OK: data send successful.
  *			  @R_ERROR: data send failure.
  */
FunStatus LTE_QueryPDPAddr(void)
{
	FunStatus result;
	result = R_ERROR;

	/* send the AT Command */
	result = LTE_TxRxDataProcess(Execute_cmd, Cmd_data, "AT+CGPADDR", NULL);
	
	/* return the result */
	return result;
}

/**
  * @function  LTE_SetMQTT
  * @brief	   LTE moudles send "recv/mode",0,0,1 data to server.
  * @param[in] None.
  * @retval   Function Operation result.
  *			  @R_OK: data send successful.
  *			  @R_ERROR: data send failure.
  */
FunStatus LTE_SetMQTT(void)
{
	FunStatus result;
	result = R_ERROR;
	uint8_t length;
	char *temp, *temp1, *data_A, data_B[256];
	temp = "AT+QMTCFG=";
	temp1 = "\"aliauth\",0,";

	/* send the data AT+QMTCFG="recv/mode",0,0,1 */
	length = strlen(temp);
	strcpy(data_B, temp);
	
	data_A = "\"recv/mode\",0,0,1";
	strcpy(data_B + length, data_A);

	/* send the AT Command */
	result = LTE_TxRxDataProcess(Set_cmd, Cmd_data, data_B, NULL);
	if (result == R_ERROR)
		return result;
	
	vTaskDelay(pdMS_TO_TICKS(1000));
	/* send the AT+QMTCFG="aliauth",0,"a1iNmsdxpkR"... */
	memset(data_B, 0x00, 256);
	
	strcpy(data_B, temp);
	length = strlen(temp);
	
	strcpy(data_B + length, temp1);
	length += strlen(temp1);

	strcpy(data_B + length, LTE_infomation.device_info.ProductKey);
	length += strlen(LTE_infomation.device_info.ProductKey);
	
	strcpy(data_B + length,",");
	length++;

	strcpy(data_B + length, LTE_infomation.device_info.DeviceName);
	length += strlen(LTE_infomation.device_info.DeviceName);

	strcpy(data_B + length,",");
	length++;

	strcpy(data_B + length, LTE_infomation.device_info.DeviceSecret);
	
	/* send the AT Command */
	result = LTE_TxRxDataProcess(Set_cmd, Cmd_data, data_B, NULL);
	
	/* return the result */
	return result;
}

/**
  * @function  LTE_OpenConMQTTNet
  * @brief	   LTE moudles send AT+QMTOPEN=0... data to server.
  * @param[in] None.
  * @retval   Function Operation result.
  *			  @R_OK: data send successful.
  *			  @R_ERROR: data send failure.
  */
FunStatus LTE_OpenConMQTTNet(void)
{
	FunStatus result;
	result = R_ERROR;
	uint8_t length;
	char *temp, *temp1, data_A[128];
	temp = "AT+QMTOPEN=0,";
	temp1 = "AT+QMTCONN=0,";

	/* send the data AT+QMTOPEN=0,"iot-as-mqtt... */
	strcpy(data_A, temp);
	length = strlen(temp);

	strcpy(data_A + length, LTE_infomation.device_info.HostName);
	length += strlen(LTE_infomation.device_info.HostName);
	
	strcpy(data_A + length, ",");
	length++;

	strcpy(data_A + length, LTE_infomation.device_info.ServerPort);
	/* send the AT Command */
	result = LTE_TxRxDataProcess(Set_cmd, Cmd_data, data_A, NULL);
	if (result == R_ERROR)
		return result;
	
	vTaskDelay(pdMS_TO_TICKS(500));
	/* send the AT+QMTCONN=0,"Environment_device"... */
	memset(data_A, 0x00, 128);
	
	strcpy(data_A, temp1);
	length = strlen(temp1);
	strcpy(data_A + length, LTE_infomation.device_info.DeviceName);
	
	/* send the AT Command */
	result = LTE_TxRxDataProcess(Set_cmd, Cmd_data, data_A, NULL);
	
	/* return the result */
	return result;
}

/**
  * @function  LTE_SendData
  * @brief	   LTE moudles send message to server.
  * @param[in] None.
  * @retval   Function Operation result.
  *			  @R_OK: data send successful.
  *			  @R_ERROR: data send failure.
  */
FunStatus LTE_SendData(void)
{
	FunStatus result;
	result = R_ERROR;
	uint16_t length;
	char *temp;
	char ch[8];
	char data_A[256];
	
	temp = "AT+QMTPUB=0,0,0,0,";

	/* send the data AT+QMTPUB=0,0,0,0,"/sys/a1iNmsdxpkR/... */
	strcpy(data_A, temp);
	length = strlen(temp);

	strcpy(data_A + length, LTE_infomation.device_info.SystemTopic);
	length = strlen(LTE_infomation.device_info.SystemTopic);

	/* send the AT Command */
	result = LTE_TxRxDataProcess(Set_cmd, Cmd_data, data_A, NULL);
	if (result == R_ERROR)
		return result;
	
	vTaskDelay(pdMS_TO_TICKS(100));
	/* send the AT+QMTCONN=0,"Environment_device"... */
	memset(data_A, 0x00, 128);
	length = 0;
	strcpy(data_A, "{");
	length++;
	
	/* WindSpeed and value */
	strcpy(data_A + length, LTE_infomation.device_info.parame_ID[0]);
	length += strlen(LTE_infomation.device_info.parame_ID[0]);
	
	/* float value convert to string */
	gcvt_user(Art_Sunshine_Info.windspeed, ch);
	strcpy(data_A + length, ch);
	length += strlen(ch);
	strcpy(data_A + length, ",");
	length ++;

	/* DevicePower and value */
	strcpy(data_A + length, LTE_infomation.device_info.parame_ID[1]);
	length += strlen(LTE_infomation.device_info.parame_ID[1]);
	
	data_A[length++] = (Art_Sunshine_Info.console_status[1] + 48);

	strcpy(data_A + length, "}");
	length++;
	
	/* send the AT Command */
	if (length < 256)
	{
		result = LTE_TxRxDataProcess(Set_cmd, Only_data, data_A, NULL);
	}
	else
	{
		PDEBUG("The data size is more than 1024.\n");
	}
	
	/* return the result */
	return result;
}

/**
  * @function  LTE_QueryRTC
  * @brief	   LTE moudles get the RTC date and time.
  * @param[in] None.
  * @retval    RTC data and time.
  */
RTC_Type LTE_QueryRTC(void)
{
	FunStatus result;
	result = R_ERROR;
	PORT_BUF_FORMAT *ackbuffer;
	RTC_Type curdate;
	
	/* send the AT Command */
	result = LTE_TxRxDataProcess(Read_cmd, Cmd_data, "AT+CCLK?", &ackbuffer);
	if(result != R_ERROR)
	{
		/* Read the date */
		curdate.rtc_date.date_year = ((ackbuffer->buffer[10] - 48)* 10) + ackbuffer->buffer[11] - 48 + 2000;
		curdate.rtc_date.date_month = ((ackbuffer->buffer[13] - 48) * 10) + ackbuffer->buffer[14] - 48;
		curdate.rtc_date.date_day = ((ackbuffer->buffer[16] - 48) * 10) + ackbuffer->buffer[17] - 48;
	
		/* Read the time */
		curdate.rtc_time.time_hours = ((ackbuffer->buffer[19] - 48) * 10) + ackbuffer->buffer[20] - 48;
		curdate.rtc_time.time_minutes = ((ackbuffer->buffer[22] - 48) * 10) + ackbuffer->buffer[23] - 48;
		curdate.rtc_time.time_seconds = ((ackbuffer->buffer[25] - 48) * 10) + ackbuffer->buffer[26] - 48;

		vPortFree(ackbuffer);
	}
	
	/* return the current data */
	return curdate;
}

/**
  * @function  LTE_CloseMQTT
  * @brief	   LTE moudles close the MQTT Connect.
  * @param[in] None.
  * @retval   Function Operation result.
  *		   @R_OK: data send successful.
  *		   @R_ERROR: data send failure.
  */
FunStatus LTE_CloseMQTT(void)
{
	FunStatus result;
	result = R_ERROR;
	
	/* send the AT Command */
	result = LTE_TxRxDataProcess(Set_cmd, Cmd_data, "AT+QMTCLOSE=0", NULL);
	
	/* return the current data */
	return result;
}


/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/
