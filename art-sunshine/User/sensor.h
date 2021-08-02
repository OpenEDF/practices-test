/**
  ******************************************************************************
  * @file    sensor.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file contains all the functions prototypes for the sensor  
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
#ifndef __SENSOR__H__
#define __SENSOR__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"
#include "uart.h"
#include "timer.h"
#include "arm_math.h"


/** @addtogroup Sensor_Driver
  * @{
  */
/* Exported macro ------------------------------------------------------------*/
#define COMMAND_SIZE	8		/* command data length */
#define REV_LENGTH		7		/* uart receiver frame length */
#define SENSOR_ADDR		0x01	/* sensor default drress is 0x01 */

/* Exported types ------------------------------------------------------------*/

/*
 * enum: @Beaufort Wind Scale Type
*/
typedef enum {
	Clam = 0, 			/* Smoke rises vertically */
	Light_air,			/* Smoke drifts with air, weather vanes inactive */
	Light_breeze,		/* Weather vanes active, wind felt on face, leaves rustle */
	Gentle_breeze,		/* Leaves &amp; small twigs move, light flags extend */
	Moderate_breeze,	/* Small branches sway, dust &amp; loose paper blows about */
	Fresh_breeze,		/* Small trees sway, waves break on inland waters */
	Strong_breeze,		/* Large branches sway, umbrellas difficult to use */
	Moderate_gale,		/* Whole trees sway, difficult to walk against wind */
	Fresh_gale,			/* Twigs broken off trees, walking against wind very difficult */
	Strong_gale,		/* Slight damage to buildings, shingles blown off roof */
	Whole_gale,			/* Trees uprooted, considerable damage to buildings */
	Storm,				/* Widespread damage, very rare occurrence */
	Hurricane,			/* Violent destruction */
	error_windspeed = 0xEE,		/* Error wind speed */
} Beaufort_WindType;

/* 
 * enum: @Sensor device Status
*/
typedef enum {
	EXIST = 0,			/* Sensor device is exist */
	LOST = !EXIST,		/* Sensor device is losT */
} Sensor_Status;

#include "user.h"

/*
 * struct: @sensor device data struct
 */
typedef struct {
	uint32_t samp_interval;         			/* sensor sampling interval */
	QueueHandle_t xSensorUARTRxQueue;			/* 485 serial port receiver data Queue. */
	SemaphoreHandle_t xSensoeBinarySemaphore; 	/* Sensor Binary Semaphore */
}Sensor_Str;

#define UART485_QUEUE_DEPTH	  1		/* queue depth */
#define CLEAR_FLAG_TIME		  (30 * 60 * 1000)	/* EXECPTION MODE flag clear delay  */
#define SENSOR_SAMP_FRE		  (30 * 1000)		/* sensor sample intervale time */




/* Exported constants --------------------------------------------------------*/
extern Sensor_Str wind_device;		/* global variable */

/*command for read wind speed value */	
extern uint8_t read_command[COMMAND_SIZE];
extern float32_t windspeed_level[Hurricane];

/* Exported functions --------------------------------------------------------*/
Beaufort_WindType windspeed_to_beaufort(float32_t windspeed);
static FunStatus SensorFram_UnPack(PORT_BUF_FORMAT *rxbufferdata);
PORT_BUF_FORMAT *SensorTxRx_Proc(uint8_t *cmd_frame, uint16_t cmd_size);
void Sensor_Init(void);
void SunshineReadWind_Task(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif  /*__SENSOR__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
