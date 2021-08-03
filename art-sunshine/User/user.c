/**
  ******************************************************************************
  * @file    user.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file provides all the operation and configure functions for 
  *          user operation and system information.
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
#include "user.h"
#include "motor.h"

/** @addtogroup User_Driver
  * @{
  */
  
/** @defgroup User control
  * @brief User control driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SYSTEM_INFORMATION Art_Sunshine_Info;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
  
/** @defgroup User_Functions
  * @{
  */
/**
  * @function   System_Init
  * @brief 	    System Initalization.
  * @param[in]  None.
  * @retval	    None.
  */ 
void System_Init(void)
{
	uint8_t Flash_ID;
	
	RCC_ClocksTypeDef get_system_clock;

	/* Check the system clock */
	RCC_GetClocksFreq(&get_system_clock);

 	/* system LED Initalizes */
	System_LED_Init();
	
	/* system UART Initalizes */
	System_USART_Init();
	PDEBUG("System UART Initalized.\n");
	PDEBUG("The system clock: %d Hz.\n", get_system_clock.SYSCLK_Frequency);
 
	/* Basic Timer config */
	TIM6_Configuration();
	TIM2_Configuration();
	PDEBUG("System TIMER6 Initalized.\n");

	/* system sensor Init  */
	Sensor_Init();
	PDEBUG("System Seneor Initalized.\n");
	
	/* system RTC Init */
	System_RTC_Init();
	PDEBUG("System RTC Initalized.\n");

	/* system LED Init */
	TASK_LED_ON();
	PDEBUG("System TASK LED Initalized.\n");
	
	/* system BEEP Init */
	System_Beep_Init();
	PDEBUG("System BEEP Initalized.\n");

	/* system KEY Init */
	EXTI_KEY_Config();
	PDEBUG("System KEY Configured.\n");

	/* system LTE moudle Init */
	LTE_DeInit();
	PDEBUG("System LTE Moudles Initalized.\n");

	/* system PVD Config */
	PVD_Config(PVD_LEVEL_2_7);
	PDEBUG("system PVD Configured.\n");
	
	/* system motor pwm Init */
	system_motor_init(MOTOR_1000HZ_PARAMEMTERS);
	motor_test();

	/* system parameters Initalized */
	Flash_LoadWorkParam();
	Flash_ReadByte(FLASH_WORK_PARAM_ADDR, &Flash_ID);
	//if (Flash_ID != 'W') 
	//{
		PDEBUG("Store the System parameters to Flash.\n");
		Flash_SaveWorkParam();
	//}

	PDEBUG("System Iniatlized.\n");
	/* END */
}

/**
  * @function   Calibration_Time
  * @brief 	    Calibration the system.
  * @param[in]  None.
  * @retval	    None.
  */ 
void Calibration_Time(void)
{
	RTC_Type current_time;
	
	/* check the date and time from Aliyun and set local RTC */
	
	LTE_ATE0();		/* CLOSE THE ECH0 */
	
	current_time = LTE_QueryRTC();
	PDEBUG("The Current Time: %d/%d/%d - %d:%d:%d", current_time.rtc_date.date_year, current_time.rtc_date.date_month, \
													current_time.rtc_date.date_day, current_time.rtc_time.time_hours,  \
													current_time.rtc_time.time_minutes, current_time.rtc_time.time_seconds);
	
	/* Calculater the time of sunset and sunrise */
	Art_Sunshine_Info.sunrise_set = ClaculSun_RiseSet(current_time, Art_Sunshine_Info.HangZhou);
	PDEBUG("Today Sunrise Time: %d:%d, SunSet Time: %d:%d\n", 	\
		Art_Sunshine_Info.sunrise_set.sunrise_time.time_hours,  \
		Art_Sunshine_Info.sunrise_set.sunrise_time.time_minutes,\
		Art_Sunshine_Info.sunrise_set.sunset_time.time_hours,   \
		Art_Sunshine_Info.sunrise_set.sunset_time.time_minutes);

	/* upadate the time  */
	RTC_TimeAndDate_Set(&current_time);
}

/**
  * @function   Calibration_Time
  * @brief 	    Calibration the system.
  * @param[in]  None.
  * @retval	    None.
  */ 
void CalSUNRiseSet_Time(void)
{
	RTC_Type current_time;
	/* calculater the sun rise and sun set time's than very day */
	current_time = RTC_TimeAndDate_Get();
	
	PDEBUG("The Current Time: %d/%d/%d - %d:%d:%d", current_time.rtc_date.date_year, current_time.rtc_date.date_month, \
													current_time.rtc_date.date_day, current_time.rtc_time.time_hours,  \
													current_time.rtc_time.time_minutes, current_time.rtc_time.time_seconds);
		
	Art_Sunshine_Info.sunrise_set = ClaculSun_RiseSet(current_time, Art_Sunshine_Info.HangZhou);
	PDEBUG("Today Sunrise Time: %d:%d, SunSet Time: %d:%d\n", 	\
		Art_Sunshine_Info.sunrise_set.sunrise_time.time_hours,  \
		Art_Sunshine_Info.sunrise_set.sunrise_time.time_minutes,\
		Art_Sunshine_Info.sunrise_set.sunset_time.time_hours,   \
		Art_Sunshine_Info.sunrise_set.sunset_time.time_minutes);
}

/**
  * @function   Wait_LTEStartUp
  * @brief 	    Wait the LTE moudles Start up.
  * @param[in]  sec: seconds for wait.
  * @retval	    None.
  */
void Wait_LTEStartUp(uint8_t sec)
{
	uint8_t endtime;
	/* Wait the LTE Moudles Start up */
	endtime = seconds + sec;
	PDEBUG("Wait for the LTE moudles to start up... %ds.\n", sec);
	
  	while (seconds < endtime);

	/* disable the timer 2 */
	TIM_ITConfig(INDEPENT_TIMER, TIM_IT_Update, DISABLE);
	TIM_Cmd(INDEPENT_TIMER, DISABLE); 
}

/**
  * @function   LCD_DefaultShow
  * @brief 	    information for lcd show default.
  * @param[in]  None.
  * @retval	    None.
  */
void LCD_DefaultShow(void)
{
	/* clear the background */
	Send_DataTo_LCD("CLR(0);"); /* background */

	/* default setting */
	Send_DataTo_LCD("DCV32(90,5,'System State',5);");
	Send_DataTo_LCD("PL(0,40,376,40,4);");
	Send_DataTo_LCD("DCV24(15,60,'Sunrise: 06:13  Sunset: 18:23',2);");
	Send_DataTo_LCD("DCV24(70,90,'Wind Speed: 8.8 m/s',2);");
	Send_DataTo_LCD("DCV24(15,120,'LTE Operation status: ',2);");
	Send_DataTo_LCD("CIRF(300,132,8,1);");
	Send_DataTo_LCD("PL(0,179,376,179,4);");
	Send_DataTo_LCD("DCV24(50,184,'Map: 31.239692¡ã 121.499755¡ã',5);");
	Send_DataTo_LCD("DCV24(54,213,'2020/10/17 18:25:00',5);");

	/*
	+++++++++++++++++++++++++++++++++
	+          System State         +
	+ Sunrise: 06:13  Sunset: 18:23 +
	+       Wind Speed£º8.8 m/s     + 
	+ LTE Operation status:  *      +
	+                               +
	+     Map: 75.3456¡ã 35.1234¡ã    +
	+     2020/10/17 18:25:00       +
	+++++++++++++++++++++++++++++++++	
	*/
}

/**
  * @function   UserApplication_Task
  * @brief      User opeartion system and moniter.
  * @param[in]  pvParameters: default.
  * @retval     None.
  */
void UserApplication_Task(void *pvParameters)
{
	/* system init and connect network */
	char str[64] = "DCV24(15,60,'Sunrise: 06:13  Sunset: 18:23:00',2);";
	uint16_t temp;
	RTC_Type current_time;

	/* Calibration time and caluculater the sunrise and sunset*/
	Calibration_Time();

	/* LTE Moudles connect Network */
	LTE_ConnetNetwork();

	while(TRUE)
	{
		if(Art_Sunshine_Info.lte_status == online)
		{
			/* Handler the 4G send data */
			switch(Art_Sunshine_Info.mode)
			{
				case EXCEPTION_MODE:	/* Exception mode */
					if ((second_count % Art_Sunshine_Info.interval_exception) == 0)
					{
						PDEBUG("Exception Mode system send the self information to Aliyun.\n");
						/* 4G moudle Transmit the system information */
						LTE_SendMessage();
					}
				break;
				
				case NORMAL_MODE:
				case SLEEP_MODE:
					if ((second_count % Art_Sunshine_Info.interval_normal) == 0)
					{
						PDEBUG("Normal Mode system send the self information to Aliyun.\n");
						/* 4G moudle Transmit the system information */
						LTE_SendMessage();
					}
				break;

				case CLEAR_MODE:
				default:
				break;
			}
		}
		else
		{
			PDEBUG("Connecting the LTE Device to the network.\n");
			LTE_ConnetNetwork();	
		}

		/* moniter the alarm interrrupt */
		if (Alarm_Flag == 0xFF)
		{
			/* check the local time */
			Calibration_Time();

			vTaskDelay(pdMS_TO_TICKS(1000));

			/* calculater the sunrise and sunset time for new day */
			CalSUNRiseSet_Time();	

			/* show to the lcd */
			str[22] = (Art_Sunshine_Info.sunrise_set.sunrise_time.time_hours / 10) + 0x30;
			str[23] = (Art_Sunshine_Info.sunrise_set.sunrise_time.time_minutes % 10) + 0x30;

			str[25] = (Art_Sunshine_Info.sunrise_set.sunset_time.time_hours / 10) + 0x30;
			str[26] = (Art_Sunshine_Info.sunrise_set.sunset_time.time_hours % 10) + 0x30;

			Send_DataTo_LCD(str);
			/* Clear the Alarm Flag */
			Alarm_Flag = 0x00;
		}

		/* moniter the second interrupt */
		if (Second_Flag == 0xFF)
		{
			current_time = RTC_TimeAndDate_Get();
			/* year */
			str[14] = (current_time.rtc_date.date_year / 1000) + 0x30;
			temp = current_time.rtc_date.date_year % 1000;
			str[15] = (temp / 100) + 0x30;
			temp = temp % 100;
			str[16] = (temp / 10) + 0x30;
			str[17] = (temp % 10) + 0x30;

			/* month */
			str[19] = (current_time.rtc_date.date_month / 10) + 0x30;
			str[20] = (current_time.rtc_date.date_month % 10) + 0x30;

			/* day */
			str[22] = (current_time.rtc_date.date_day / 10) + 0x30;
			str[23] = (current_time.rtc_date.date_day % 10) + 0x30;

			/* hour */
			str[25] = (current_time.rtc_time.time_hours / 10) + 0x30;
			str[26] = (current_time.rtc_time.time_hours % 10) + 0x30;

			/* minutes */
			str[28] = (current_time.rtc_time.time_minutes / 10) + 0x30;
			str[29] = (current_time.rtc_time.time_minutes % 10) + 0x30;
			
			/* second */
			str[31] = (current_time.rtc_time.time_seconds / 10) + 0x30;
			str[32] = (current_time.rtc_time.time_seconds % 10) + 0x30;

			/* show to the lcd */
			Send_DataTo_LCD(str);

		}
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}


/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/
