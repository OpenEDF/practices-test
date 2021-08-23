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
#include "lcd.h"
#include "uart.h"
#include "timer.h"
#include "led.h"
#include "key.h"
#include "pvd.h"

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
#define LCD_BUFFER_SIZE			128			/* Lcd buffer size */

/* Private variables ---------------------------------------------------------*/
SYSTEM_INFORMATION Art_Sunshine_Info;
static char *lcd_default_context = "CLR(0);DCV32(90,5,'System State',5);PL(0,40,376,40,4);\
DCV24(15,60,'Sunrise: 00:00  Sunset: 00:00',2);DCV24(70,90,'Wind Speed: 00.0 m/s',2);\
DCV24(15,120,'LTE Operation status: ',2);DCV24(30,150,'Elev: +00.00  Azim: 000.00',2);\
CIRF(300,132,8,1);PL(0,179,376,179,4);DCV24(20,184,'Map: 31.239692бу 121.499755бу',5);\
DCV24(70,213,'2021/08/14 13:00:00',5);";

char str_sun_ea[LCD_BUFFER_SIZE] = "SBC(0);DCV24(30,150,'Elev: +00.00  Azim: 000.00',2);";
char draw_lineup[LCD_BUFFER_SIZE] = "PL(0,40,376,40,4);";
char draw_linedown[LCD_BUFFER_SIZE] = "PL(0,179,376,179,4);";
char map_dis[LCD_BUFFER_SIZE] = "SBC(0);DCV24(20,184,'Map: 31.239692бу 121.499755бу',5);";
char lteonline[LCD_BUFFER_SIZE] = "CIRF(300,132,8,2);";

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
	
	/* system UART Initalizes */
	System_USART_Init();
	PDEBUG("\r[OK] System UART Initalized.\n");
	PDEBUG("\r[OK] The system clock: %d Hz.\n", get_system_clock.SYSCLK_Frequency);
	
	/* system LED Initalizes */
	System_LED_Init();
	PDEBUG("\r[OK] System LED Initalized.\n");
	
	/* LCD Init */
	LCD_DefaultShow();
	PDEBUG("\r[OK] System LCD Initalized.\n");
	/* Basic Timer config */
	TIM6_Configuration();
	TIM2_Configuration();
	PDEBUG("\r[OK] System TIMER6 & TIMER2 Initalized.\n");

	/* system sensor Init  */
	Sensor_Init();
	PDEBUG("\r[OK] System Seneor Initalized.\n");
	
	/* system RTC Init */
	System_RTC_Init();
	PDEBUG("\r[OK] System RTC Initalized.\n");

	/* system LED Init */
	TASK_LED_ON();
	PDEBUG("\r[OK] System TASK LED Initalized.\n");
	
	/* system BEEP Init */
	System_Beep_Init();
	PDEBUG("\r[OK] System BEEP Initalized.\n");

	/* system KEY Init */
	EXTI_KEY_Config();
	PDEBUG("\r[OK] System KEY Configured.\n");

	/* system LTE moudle Init */
	LTE_DeInit();
	PDEBUG("\r[OK] System LTE Moudles Initalized.\n");

	/* system PVD Config */
	PVD_Config(PVD_LEVEL_2_7);
	PDEBUG("\r[OK] system PVD Configured.\n");
	
	/* system motor pwm Init */
	system_motor_init(MOTOR_1000HZ_PARAMEMTERS);
	PDEBUG("\rsystem Motor A B C D Configured.\n");
	//motor_test();

	/* system LCD_Init */
	LCD_Init();
	PDEBUG("\r[OK] system LCD Initalized.\n");

	/* system parameters Initalized */
	Flash_LoadWorkParam();
	Flash_ReadByte(FLASH_WORK_PARAM_ADDR, &Flash_ID);
	//if (Flash_ID != 'W') 
	//{
		PDEBUG("\r[OK] Store the System parameters to Flash.\n");
		Flash_SaveWorkParam();
	//}

	PDEBUG("\r[OK] System Iniatlized.\n");
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
	PDEBUG("\r[OK] The Current Time: %02d/%02d/%02d - %02d:%02d:%02d.\n", current_time.rtc_date.date_year, current_time.rtc_date.date_month, \
													current_time.rtc_date.date_day, current_time.rtc_time.time_hours,  \
													current_time.rtc_time.time_minutes, current_time.rtc_time.time_seconds);
	/* upadate the time  */
	//PDEBUG("Test: %x:\n", current_time.rtc_date.date_year);											
	//current_time.rtc_date.date_year = 0x15;		
	//PDEBUG("Test: %x:\n", current_time.rtc_date.date_year);	
	RTC_TimeAndDate_Set(&current_time);

	/* LCD update the time */
	lcd_update_time();
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
	
	PDEBUG("\r[OK] The Current Time: %04d/%02d/%02d - %02d:%02d:%02d", current_time.rtc_date.date_year, current_time.rtc_date.date_month, \
													current_time.rtc_date.date_day, current_time.rtc_time.time_hours,  \
													current_time.rtc_time.time_minutes, current_time.rtc_time.time_seconds);
		
	Art_Sunshine_Info.sunrise_set = ClaculSun_RiseSet(current_time, Art_Sunshine_Info.HangZhou);
	PDEBUG("\r[OK] Today Sunrise Time: %02d:%02d, SunSet Time: %02d:%02d\n", \
		Art_Sunshine_Info.sunrise_set.sunrise_time.time_hours,  \
		Art_Sunshine_Info.sunrise_set.sunrise_time.time_minutes,\
		Art_Sunshine_Info.sunrise_set.sunset_time.time_hours,   \
		Art_Sunshine_Info.sunrise_set.sunset_time.time_minutes);

	/* update lcd */
	lcd_update_sunset_rise();
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
	PDEBUG("\r[OK] Wait for the LTE moudles to start up... %ds.\n", sec);
	
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
	//Send_DataTo_LCD("CLR(0);"); /* background */

	/* default setting */
	//Send_DataTo_LCD("DCV32(90,5,'System State',5);");
	//Send_DataTo_LCD("PL(0,40,376,40,4);");
	//Send_DataTo_LCD("DCV24(15,60,'Sunrise: 06:13  Sunset: 18:23',2);");
	//Send_DataTo_LCD("DCV24(70,90,'Wind Speed: 8.8 m/s',2);");
	//Send_DataTo_LCD("DCV24(15,120,'LTE Operation status: ',2);");
	//Send_DataTo_LCD("CIRF(300,132,8,1);");
	//Send_DataTo_LCD("PL(0,179,376,179,4);");
	//Send_DataTo_LCD("DCV24(20,184,'Map: 31.239692бу 121.499755бу',5);");
	//Send_DataTo_LCD("DCV24(54,213,'2021/08/14 13:00:00',5);");

	/*
	+++++++++++++++++++++++++++++++++
	+          System State         +
	+ Sunrise: 06:13  Sunset: 18:23 +
	+       Wind Speedг║8.8 m/s     + 
	+ LTE Operation status:  *      +
	+                               +
	+     Map: 75.3456бу 35.1234бу    +
	+     2020/10/17 18:25:00       +
	+++++++++++++++++++++++++++++++++	
	*/
	lcd_uart_tx_str(lcd_default_context);
}

/**
  * @function   display_system_status
  * @brief 	    printf the system state.
  * @param[in]  None.
  * @retval	    None.
  */
void display_system_status(void)
{
	PDEBUG("\r[OK] ======== Display the system state ======\n");

	/* LTE state */
	if (Art_Sunshine_Info.lte_status == online)
		PDEBUG("\r[OK] LTE Status: online.\n");
	else
		PDEBUG("\r[OK] LTE Status: offline.\n");

	/* Wind speed sendor Device */
	if (Art_Sunshine_Info.winddevice_status == EXIST)
		PDEBUG("\r[OK] Wind Speed Sensor Status: EXIST.\n");
	else
		PDEBUG("\r[OK] Wind Speed Sensor Status: LOST.\n");

	/* Wind speed */
	PDEBUG("\r[OK] Current wind speed: %f m/s.\n", Art_Sunshine_Info.windspeed);
	PDEBUG("\r[OK] Current wind speed threshold: %d m/s.\n", Art_Sunshine_Info.windspeed_threshold);
	PDEBUG("\r[OK] ============= Display End ==============\n");
}

/**
  * @function   lcd_update_time
  * @brief      update the time and show LCD.
  * @param[in]  None.
  * @retval     None.
  */
void lcd_update_time(void)
{
	char str_timeupdate[LCD_BUFFER_SIZE] = "SBC(0);DCV24(70,213,'2021/08/19 18:00:00',5);";
	RTC_Type current_time;
	uint16_t temp;
	
	current_time = RTC_TimeAndDate_Get();
	/* year */
	str_timeupdate[14+7] = (current_time.rtc_date.date_year / 1000) + 0x30;
	temp = current_time.rtc_date.date_year % 1000;
	str_timeupdate[15+7] = (temp / 100) + 0x30;
	temp = temp % 100;
	str_timeupdate[16+7] = (temp / 10) + 0x30;
	str_timeupdate[17+7] = (temp % 10) + 0x30;

	/* month */
	str_timeupdate[19+7] = (current_time.rtc_date.date_month / 10) + 0x30;
	str_timeupdate[20+7] = (current_time.rtc_date.date_month % 10) + 0x30;

	/* day */
	str_timeupdate[22+7] = (current_time.rtc_date.date_day / 10) + 0x30;
	str_timeupdate[23+7] = (current_time.rtc_date.date_day % 10) + 0x30;

	/* hour */
	str_timeupdate[25+7] = (current_time.rtc_time.time_hours / 10) + 0x30;
	str_timeupdate[26+7] = (current_time.rtc_time.time_hours % 10) + 0x30;

	/* minutes */
	str_timeupdate[28+7] = (current_time.rtc_time.time_minutes / 10) + 0x30;
	str_timeupdate[29+7] = (current_time.rtc_time.time_minutes % 10) + 0x30;
			
	/* second */
	str_timeupdate[31+7] = (current_time.rtc_time.time_seconds / 10) + 0x30;
	str_timeupdate[32+7] = (current_time.rtc_time.time_seconds % 10) + 0x30;

	/* show to the lcd */
	lcd_uart_tx_str(str_timeupdate);
}

/**
  * @function   lcd_update_sunset_rise
  * @brief      update the sun set and sun rise time.
  * @param[in]  None.
  * @retval     None.
  */
void lcd_update_sunset_rise(void)
{
	char str_sunset_rise[LCD_BUFFER_SIZE] = "SBC(0);DCV24(15,60,'Sunrise: 06:13  Sunset: 18:23',2);";

	/* show to the lcd */
	str_sunset_rise[22+7] = (Art_Sunshine_Info.sunrise_set.sunrise_time.time_hours / 10) + 0x30;
	str_sunset_rise[23+7] = (Art_Sunshine_Info.sunrise_set.sunrise_time.time_hours % 10) + 0x30;

	str_sunset_rise[25+7] = (Art_Sunshine_Info.sunrise_set.sunrise_time.time_minutes / 10) + 0x30;
	str_sunset_rise[26+7] = (Art_Sunshine_Info.sunrise_set.sunrise_time.time_minutes % 10) + 0x30;
	
	str_sunset_rise[37+7] = (Art_Sunshine_Info.sunrise_set.sunset_time.time_hours / 10) + 0x30;
	str_sunset_rise[38+7] = (Art_Sunshine_Info.sunrise_set.sunset_time.time_hours % 10) + 0x30;

	str_sunset_rise[40+7] = (Art_Sunshine_Info.sunrise_set.sunset_time.time_minutes / 10) + 0x30;
	str_sunset_rise[41+7] = (Art_Sunshine_Info.sunrise_set.sunset_time.time_minutes % 10) + 0x30;
			
	/* display the lcd */
	lcd_uart_tx_str(str_sunset_rise);	
}

/**
  * @function   lcd_update_title
  * @brief      update the title display for lcd.
  * @param[in]  None.
  * @retval     None.
  */
void lcd_update_title(void)
{
	char lcd_title[LCD_BUFFER_SIZE] = "SBC(0);DCV32(90,5,'System State',5);";
	/* display the lcd */
	lcd_uart_tx_str(lcd_title);
}

/**
  * @function   lcd_update_sun_ea
  * @brief      update the sun Eleative and Azimuth angle for lcd.
  * @param[in]  None.
  * @retval     None.
  */
void lcd_update_sun_ea(void)
{
	Alta_Azim altazi_value;
	RTC_Type date_time;
	uint32_t temp_ea;
	uint32_t temp;

	/* get the cureent time */
	date_time = RTC_TimeAndDate_Get();

	/* calculater the sun pittch and azimuth */
	altazi_value = ClaculSun_AltAzi(date_time, Art_Sunshine_Info.HangZhou);
	PDEBUG("\r[INFO] The altazi_value is: altangle: %f\t azimuths: %f\t\n", altazi_value.altangles, altazi_value.azimuths);
	
	/* elevation angle is less that zero */
	if (altazi_value.altangles < 0)
	{
		altazi_value.altangles = -(altazi_value.altangles);
		str_sun_ea[27] = 0x2D;	/* - */
	}
	else
	{
		str_sun_ea[27] = 0x2B;	/* + */
	}
	/* set the elevation angle,eg:180.88 */
	temp_ea = (uint32_t)(altazi_value.altangles * 100.0F);
	str_sun_ea[28] = temp_ea / 1000 + 0x30;
	temp = temp_ea % 1000;
	str_sun_ea[29] = temp / 100 + 0x30;
	temp = temp % 100;
	str_sun_ea[31] = temp / 10 + 0x30;
	str_sun_ea[32] = temp % 10 + 0x30;

	/* set the azimuth angle, eg:180.88 */
	temp_ea = (uint32_t)(altazi_value.azimuths * 100.0F);
	str_sun_ea[41] = temp_ea / 10000 + 0x30;
	temp = temp_ea % 10000;
	str_sun_ea[42] = temp / 1000 + 0x30;
	temp = temp % 1000;
	str_sun_ea[43] = temp / 100 + 0x30;
	temp = temp % 100;
	str_sun_ea[45] = temp / 10 + 0x30;
	str_sun_ea[46] = temp % 10 + 0x30;

	/* display the lcd */
	lcd_uart_tx_str(str_sun_ea);
}


/**
  * @function   UserApplication_Task
  * @brief      User opeartion system and moniter.
  * @param[in]  pvParameters: default.
  * @retval     None.
  */
void UserApplication_Task(void *pvParameters)
{
	PDEBUG("\r[OK] System enter UserApplication_Task.\n");
	/* LTE Moudles connect Network */
	LTE_ConnetNetwork();

	/* Calibration time and caluculater the sunrise and sunset*/
	Calibration_Time();
	
	/* update the sun rise and sun set */
	CalSUNRiseSet_Time();

	/* main task */
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
						PDEBUG("\r[OK] Exception Mode system send the self information to Aliyun.\n");
						/* 4G moudle Transmit the system information */
						LTE_SendMessage();
						/* display system state */
						display_system_status();
					}
				break;
				
				case NORMAL_MODE:
				case SLEEP_MODE:
					if ((second_count % Art_Sunshine_Info.interval_normal) == 0)
					{
						PDEBUG("\r[OK] Normal Mode system send the self information to Aliyun.\n");
						/* 4G moudle Transmit the system information */
						LTE_SendMessage();
						display_system_status();
					}
				break;

				case CLEAR_MODE:
				break;
				default:	/* never be here */
				break;
			}
		}
		else
		{
			PDEBUG("\r[OK] Connecting the LTE Device to the network.\n");
			LTE_ConnetNetwork();	
		}

		/* BUG: must update the LCD display title */
		lcd_update_title();
		vTaskDelay(pdMS_TO_TICKS(500));	    /* must set */
		
		lcd_uart_tx_str(draw_lineup);
		vTaskDelay(pdMS_TO_TICKS(500));		/* must set */
		
		lcd_uart_tx_str(draw_linedown);
		vTaskDelay(pdMS_TO_TICKS(500));		/* must set */
		
		lcd_uart_tx_str(map_dis);
		vTaskDelay(pdMS_TO_TICKS(500));		/* must set */
	
		lcd_update_sun_ea();
		vTaskDelay(pdMS_TO_TICKS(500)); 	/* must set */

		if (Art_Sunshine_Info.lte_status == offline)
			lcd_uart_tx_str(lteonline);
		vTaskDelay(pdMS_TO_TICKS(500)); 	/* must set */
		
		/* Calculater the sunrise and sunset */
		CalSUNRiseSet_Time();
		
		/* moniter the second interrupt */
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
}

/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/
