/**
  ******************************************************************************
  * @file    sensor.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file provides all the sensor read-write and configure functi-
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
#include "sensor.h"


/** @addtogroup Sensor_Driver
  * @{
  */
  
/** @defgroup SENSOR 
  * @brief Sensor driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define LCD_BUFFER_SIZE 128		/* set the lcd buffer size */
/* Private variables ---------------------------------------------------------*/
Sensor_Str wind_device;		/* global variable */

/*command for read wind speed value */	
uint8_t read_command[COMMAND_SIZE] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
float32_t windspeed_level[Hurricane] = {0.5F, 1.5F, 3.3F, 5.5F, 7.9F, 10.7F, 13.8F, 17.1F, 20.7F, 24.4F, 28.4F,32.6F};
char display_str_buffer[LCD_BUFFER_SIZE] = "SBC(0);DCV24(70,90,'Wind Speed: 88.8 m/s',2);";

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
  
/** @defgroup Sensor_Private_Functions
  * @{
  */
  
/**
  * @name	windspeed_to_beaufort
  * @brief	convert the wind speed to beaufort wind scale.
  * @param	windspeed: wind speed value.
  * @retval beaufort wind type value 0 - 12 level.
  * 		The -1 indicates the error parameters.
  */
Beaufort_WindType windspeed_to_beaufort(float32_t windspeed)
{
	/* check the input parameters */
	if (windspeed < 0.0F)
		return error_windspeed;

	if (windspeed >= 32.7F)
		return Hurricane;

	/* convert and return beaufort type */
	if (windspeed <= 10.8F)
	{
		if (windspeed <= windspeed_level[Clam])
			return Clam;
		else if (windspeed <= windspeed_level[Light_air])
			return Light_air;
		else if (windspeed <= windspeed_level[Light_breeze])
			return Light_breeze;
		else if (windspeed <= windspeed_level[Gentle_breeze])
			return Gentle_breeze;
		else if (windspeed <= windspeed_level[Moderate_breeze])
			return Moderate_breeze;
		else if (windspeed <= windspeed_level[Fresh_breeze])
			return Fresh_breeze;
		else
			return error_windspeed;
	}
	else	/* dichotomy speed up function running  */
	{
		if (windspeed <= windspeed_level[Strong_breeze])
			return Strong_breeze;
		else if (windspeed <= windspeed_level[Moderate_gale])
			return Moderate_gale;
		else if (windspeed <= windspeed_level[Fresh_gale])
			return Fresh_gale;
		else if (windspeed <= windspeed_level[Strong_gale])
			return Strong_gale;
		else if (windspeed <= windspeed_level[Whole_gale])
			return Whole_gale;
		else if (windspeed <= windspeed_level[Storm])
			return Storm;
		else
			return error_windspeed;
	}
}

/**
  * @function  SensorTxRx_Proc
  * @brief	   transmit the command to deviced amd read sensor value.
  * @param[in] cmd_frame: command sequence for read the sensor value.
  * @param[in] cmd_size: the size of the command frame.
  * @retval    pointe data indate the receiver data.
  *			   vaild data is return NULL.
  */
PORT_BUF_FORMAT *SensorTxRx_Proc(uint8_t *cmd_frame, uint16_t cmd_size)
{
	PORT_BUF_FORMAT *txcmdbuffer, *rxackbuffer;
	USART_BUFStruct *uartPtr;
	uint16_t memsize;
	uint8_t loop_times;
	uartPtr = &USART_PORTDATA[USART_485];

	/* Check the mutex is available? */
	if (xSemaphoreTake(xSerial485Mutex, pdMS_TO_TICKS(100)) == pdFAIL)
	{
		PDEBUG("\r[ERROR] The Mutex xSerial485Mutex is not be obtained.\n");
		return NULL;
	}
	
	/* Package the data frame */
	memsize = calmemsize(cmd_size);
	if (NULL == (txcmdbuffer = pvPortMalloc(memsize)))
	{
		PDEBUG("\r[ERROR] No more memory to be allocate.\n");
		return NULL;
	}
	txcmdbuffer->port = USART_485;
	txcmdbuffer->length = cmd_size;
	memcpy(txcmdbuffer->buffer, cmd_frame, cmd_size);

	/* debug */
	//print_uart_data("[INFO] UART 485 Port Send data:", txcmdbuffer->buffer, txcmdbuffer->length);

	/* send the command to deviced by 485 interface */
	if(xQueueSendToBack(xSerialTxQueue, &txcmdbuffer, pdMS_TO_TICKS(100)) != pdPASS)
	{
		PDEBUG("\r[ERROR] UART_485 Queue SEND message failed.\n");
		return NULL;
	}

	loop_times = 3;
	/* receiver the data frame */
	while(loop_times--)
	{
		if (xSemaphoreTake(wind_device.xSensoeBinarySemaphore, pdMS_TO_TICKS(100)) != pdPASS)
		{
			PDEBUG("\r[ERROR] UART_485 Queue RECEIVER message failed.\n");
			continue;
		}
		else
		{
			/* copy the data from UART_BUFFER array  to rxackbuffer */
			memsize = calmemsize(uartPtr->rxbuf_count + 16);
			if (NULL != (rxackbuffer = pvPortMalloc(memsize)))
			{
				/* copy the data to string struct */
				rxackbuffer->port = USART_485;
				rxackbuffer->length = uartPtr->rxbuf_count;
				memcpy(rxackbuffer->buffer, uartPtr->rxbuffer, rxackbuffer->length);
				
				/* debug */
				//print_uart_data("[INFO] UART 485 Port Receiver data:", rxackbuffer->buffer, rxackbuffer->length);
				/* clear and initalizes the uart[485] buffer */
				memset(uartPtr->rxbuffer, 0x00, uartPtr->rxbuf_count);
				uartPtr->rxbuf_count = 0;
			}
		
			/* unpack data frame */
			if (R_ERROR == SensorFram_UnPack(rxackbuffer))
			{
				Art_Sunshine_Info.winddevice_status = LOST;
				vPortFree(rxackbuffer);
				xSemaphoreGive(xSerial485Mutex);
				xSemaphoreGive(wind_device.xSensoeBinarySemaphore);
				PDEBUG("\r[ERROR] UART_485 RECEIVER message is unpack ERROR.\n");
				return NULL;
			}
			else
			{
				Art_Sunshine_Info.winddevice_status = EXIST;
				/* return received data pointe */
				xSemaphoreGive(wind_device.xSensoeBinarySemaphore);
				xSemaphoreGive(xSerial485Mutex);
				return rxackbuffer;
			}
		}
	}
	/* does reach here incidate 3 times receiver is not vailed data */
	/* Release the Mutex */
	xSemaphoreGive(xSerial485Mutex);
	return NULL;	
}

/**
  * @function  SensorFram_UnPack
  * @brief	   unpacking the data frame from the 485 interface.
  * @param[in] rxbufferdata: receiver data frame.
  * @retval    Function operation status.
  */
static FunStatus SensorFram_UnPack(PORT_BUF_FORMAT *rxbufferdata)
{
	uint16_t crc16_result;
	/* check the data port, length, address and function code */
	if ((rxbufferdata->port != USART_485) || (rxbufferdata->length != REV_LENGTH) || \
		(rxbufferdata->buffer[0] != SENSOR_ADDR))
	{
		return R_ERROR;
	}
		
	/* claculater the CRC16 MOUBUS code */
	crc16_result = Cal_CRC16(CRC16_MODBUS, rxbufferdata->buffer, rxbufferdata->length - 2);	

	/* check the CRC16 code */
	if ((rxbufferdata->buffer[rxbufferdata->length - 2] != (uint8_t)crc16_result) ||
		(rxbufferdata->buffer[rxbufferdata->length - 1] != (uint8_t)(crc16_result >> 8)))
	{
		return R_ERROR;
	}

	/* return result */
	return R_OK;
}

/**
  * @function  Sensor_Init
  * @brief	   wind speed sensor devied struct initalizes.
  * @param[in] None.
  * @retval    None. 
  */
void Sensor_Init(void)
{

	/* Initalize the sampling interval */
	wind_device.samp_interval = SENSOR_SAMP_FRE;	/* millisecond period value */
	
	/* Initalize the binary semaphore and queue */
	wind_device.xSensorUARTRxQueue = xQueueCreate(UART485_QUEUE_DEPTH,(BaseType_t)sizeof(uint8_t *));
	/* check cerated */
	if (wind_device.xSensorUARTRxQueue == NULL )
	{
		PDEBUG("\r[ERROR] The Seneor Queue xUART485RxQueue is Create Failed.\n");
	}
	else
	{
		PDEBUG("\r[OK] wind_device.xSensorUARTRxQueue is Created.\n");
	}

	wind_device.xSensoeBinarySemaphore = xSemaphoreCreateBinary();
	/* check cerated */
	if (wind_device.xSensoeBinarySemaphore == NULL )
	{
		PDEBUG("\r[ERROR] The Seneor xSensoeBinarySemaphore is Create Failed.\n");
	}
	else
	{
		PDEBUG("\r[OK] wind_device.xSensoeBinarySemaphore is Created.\n");
	}

	
}

/**
  * @function  SunshineReadWind_Task
  * @brief	   Read the wind speed and Compare with the threshold.
  * @param[in] pvParameters: default.
  * @retval    None.
  */
void SunshineReadWind_Task(void *pvParameters)
{
	PORT_BUF_FORMAT *read_windspeed_buf;
	uint16_t temp;
	static uint32_t delay_counts;
	/* wait the wind sensor ready */
	vTaskDelay(pdMS_TO_TICKS(1000));
	
	while(TRUE)
	{
		PDEBUG("\r[OK] Enter SunshineReadWind_Task Task.\n");
		
		/* Read the wind speed value. */
		read_windspeed_buf = SensorTxRx_Proc(read_command, COMMAND_SIZE);
		if ((read_windspeed_buf != NULL) && (Art_Sunshine_Info.winddevice_status == EXIST))
		{
			/* update system information */
			temp = (uint16_t)(read_windspeed_buf->buffer[3] << 8) + read_windspeed_buf->buffer[4];

			/* TODO:check the follow code is used FPU on assembly?  yes. */
			Art_Sunshine_Info.windspeed = (float32_t)temp / 10.0F;
		
			Art_Sunshine_Info.wind_level = windspeed_to_beaufort(Art_Sunshine_Info.windspeed);
			PDEBUG("\r[OK] The Current wind speed value: %f m/s\tBeaufort: %d.\n", \
				Art_Sunshine_Info.windspeed, \
				Art_Sunshine_Info.wind_level);

			/* show to the lcd: xx.x m/s */
			display_str_buffer[32] = (temp / 100) + 0x30;
			display_str_buffer[33] = (temp % 100 / 10) + 0x30;
			display_str_buffer[35] = (temp % 10) + 0x30;

			lcd_uart_tx_str(display_str_buffer);
			/* free memory */
			vPortFree(read_windspeed_buf);
		}
		else
		{
			PDEBUG("\r[ERROR] Read the wind speed Failed.\n");
			Art_Sunshine_Info.winddevice_status = LOST;
		}
		
		/* check the windspeed is greather than threshold value. */
		if(Art_Sunshine_Info.windspeed >= windspeed_level[Art_Sunshine_Info.windspeed_threshold])
		{
			PDEBUG("\r[OK] Current wind speed: %f m/s, wind speed threshold: %f m/s.\n", Art_Sunshine_Info.windspeed, windspeed_level[Art_Sunshine_Info.windspeed_threshold]);
			PDEBUG("\r[OK] Current wind speed exceeds the threshold and system enters exception mode.\n");
			/* update the sysytem mode. */
			Art_Sunshine_Info.mode = EXCEPTION_MODE;
			
			/* add the clear flag delay */
			delay_counts = timer_counter + CLEAR_FLAG_TIME;
		}
		
		/* clear the sysytem mode. */
		/* !!! NOTE: When the server sends the data control device into exception mode,
		   the command must be send again de-control*/ 
		if ((timer_counter > delay_counts) && (Art_Sunshine_Info.serctl_status == control)) /* before de_control */
		{
			PDEBUG("\r[OK] Current wind speed can guarantee the system working.\n");
			Art_Sunshine_Info.mode = NORMAL_MODE;
			delay_counts = 0;
		}
		vTaskDelay(pdMS_TO_TICKS(SENSOR_SAMP_FRE));
	}
}

/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/	
