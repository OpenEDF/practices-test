/**
  ******************************************************************************
  * @file    user.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file contains all the functions prototypes for user operation 
  *			 configure and system information.
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
#ifndef __USER__H__
#define __USER__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "sun.h"
#include "lte.h"
#include "console.h"
#include "sensor.h"

/** @addtogroup User_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/* 
 * struct: @system golbal information 
*/
typedef struct {
	char version[64];					/* system software version */
	System_Mode mode;				/* system working mode */
	LTE_Status  lte_status;			/* system LTE moudle network status */
	CONSOLE_Status console_status[CONSOLE_NUMS];	/* system console control working Status, 
												   the array size is the decvice numbers */
	float32_t  windspeed;						/* the wind speed value for system measurement */
	Beaufort_WindType windspeed_threshold;		/* the threshold for wind speed */
	Beaufort_WindType wind_level;				/* beaufort wind speed level  */
	Sensor_Status winddevice_status;			/* wind speed sensor device status */
	uint16_t interval_normal;		/* time interval for push information to aliyun on normal mode */
	uint8_t interval_exception;		/* time interval for push information to aliyun on exception mode */
	uint32_t  control_interval;		/* console control time interval */
	Lati_Longi HangZhou;			/* The Latitude Longitude value of HangZhou */
	Suntime_Type sunrise_set;		/* The sunrise and sunset time */ 
	Server_Ctl serctl_status;		/* server force control sataus */
	uint32_t syswork_time;			/* the cumulative time for system working */ 
} SYSTEM_INFORMATION;

#include "flash.h"

/* Exported constants --------------------------------------------------------*/
extern SYSTEM_INFORMATION Art_Sunshine_Info; /* system information global variable */

/* Exported macro ------------------------------------------------------------*/
#define YSYTEM_RESTART_TIMES	5			/* system restart times */
#define LTE_WAIT_SECOND			12			/* Wait second times */

/* Exported functions --------------------------------------------------------*/
void System_Init(void);
void Calibration_Time(void);
void CalSUNRiseSet_Time(void);
void Wait_LTEStartUp(uint8_t sec);
void LCD_DefaultShow(void);
void display_system_status(void);
void lcd_update_time(void);
void lcd_update_sunset_rise(void);
void lcd_update_title(void);
void lcd_update_sun_ea(void);
void UserApplication_Task(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif  /*__USER__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
