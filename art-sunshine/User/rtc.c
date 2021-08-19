/**
  ******************************************************************************
  * @file    rtc.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file provides all the rtc configure and set-get real time.
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
#include "rtc.h"
#include "common.h"

/** @addtogroup RTC_Driver
  * @{
  */
  
/** @defgroup RTC 
  * @brief Uart driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint32_t second_count;				/* sencond */

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/** @defgroup RTC_Private_Functions
  * @{
  */

/**
  * @function   RTC_Config_Init
  * @brief      Initalizes the system RTC.
  * @param[in]  None.
  * @retval     None.
  */ 
void RTC_Config_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;

	/* ENBALE the RTC RCC */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Config and Select rcc source */
  	RCC_LSEConfig(RCC_LSE_ON);  
  	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
  	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);    

	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();

	/* Init the RTC reg */
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	/* 32768 / (0x7F + 1) = 0x100 */
	RTC_InitStructure.RTC_AsynchPrediv = (uint32_t)0x7F;
	/* 0x100 / (0xFF + 1) = 1 */
	RTC_InitStructure.RTC_SynchPrediv = (uint32_t)0xFF;
	/* 1 / 1 = 1s */
	RTC_Init(&RTC_InitStructure);
}

/**
  * @function   RTC_TimeAndDate_Set
  * @brief      Set the system RTC time and date.
  * @param[in]  None.
  * @retval     Function operation Status.
  *             SUCCESS: RTC Date register is configured
  *             ERROR: RTC Date register is not configured
  */ 
ErrorStatus RTC_TimeAndDate_Set(RTC_Type *date_time)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	ErrorStatus status = ERROR;

	/* set date */
	RTC_DateStructure.RTC_Year = decimal_to_bcd(date_time->rtc_date.date_year);
	RTC_DateStructure.RTC_Month = decimal_to_bcd(date_time->rtc_date.date_month);
	RTC_DateStructure.RTC_Date = decimal_to_bcd(date_time->rtc_date.date_day);
	RTC_DateStructure.RTC_WeekDay = decimal_to_bcd(date_time->rtc_date.date_weekday);
	status = RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
		
	/* set time */
	//RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = decimal_to_bcd(date_time->rtc_time.time_hours); 
	RTC_TimeStructure.RTC_Minutes = decimal_to_bcd(date_time->rtc_time.time_minutes);
	RTC_TimeStructure.RTC_Seconds = decimal_to_bcd(date_time->rtc_time.time_seconds);
	status = RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

	/* return Error Value */
	return status;
}

/**
  * @function   RTC_TimeAndDate_Get
  * @brief      Get the system RTC time and date.
  * @param[in]  None.
  * @retval     RTC_Type struct constant the time and date.
  *           	NULL indicate the operation failed.
  */ 
RTC_Type RTC_TimeAndDate_Get(void)
{
	RTC_Type cur_date_time;
	
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	/* Get the date */
	RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
	cur_date_time.rtc_date.date_year = bcd_to_decimal(RTC_DateStructure.RTC_Year) + 2000;
	cur_date_time.rtc_date.date_month = bcd_to_decimal(RTC_DateStructure.RTC_Month);
	cur_date_time.rtc_date.date_day = bcd_to_decimal(RTC_DateStructure.RTC_Date);
	cur_date_time.rtc_date.date_weekday = bcd_to_decimal(RTC_DateStructure.RTC_WeekDay);

	/* Get the time */
	RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
	cur_date_time.rtc_time.time_hours = bcd_to_decimal(RTC_TimeStructure.RTC_Hours);
	cur_date_time.rtc_time.time_minutes = bcd_to_decimal(RTC_TimeStructure.RTC_Minutes);
	cur_date_time.rtc_time.time_seconds = bcd_to_decimal(RTC_TimeStructure.RTC_Seconds);

	/* return struct for date and time */
	return cur_date_time;
}

/**
  * @function   RTC_Alarm_Config
  * @brief      Set and config the system RTC Alarm.
  * @param[in]  RTC_Alarm_X: stm32f4 can use RTC_Alarm_A or RTC_Alarm_B.  
  * @retval     None.
  */ 
static void RTC_Alarm_Config(uint32_t RTC_Alarm_X, Time_Type *alarm_time)
{
	EXTI_InitTypeDef  EXTI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	RTC_AlarmTypeDef  RTC_AlarmStructure;

	/* config exti interrupt */
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* config NVIC struct  */
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* config and set alarm time  */  
	RTC_AlarmCmd(RTC_Alarm_X, DISABLE);
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = alarm_time->time_hours;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = alarm_time->time_minutes;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = alarm_time->time_seconds;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	/* when RTC_AlarmMask_DateWeekDay is set, follow two item has been not used. */
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 1;
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_X, &RTC_AlarmStructure);
	
	/* enable alarm interrupt */
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	RTC_AlarmCmd(RTC_Alarm_X, ENABLE);
	RTC_ClearFlag(RTC_FLAG_ALRAF);
	EXTI_ClearITPendingBit(EXTI_Line17);
	
}

/**
  * @function   RTC_Wakeup_Config
  * @brief      Set and config the system Wack Up interrupt.
  * @param[in]  None.  
  * @retval     None.
  */ 
static void RTC_Wakeup_Config(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;

	/* config exti interrupt */
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStructure.EXTI_Line = EXTI_Line22; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* config NVIC struct  */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* config Wakeup interrupt */ 
	RTC_WakeUpCmd(DISABLE);
	RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
	RTC_SetWakeUpCounter(RTC_WACKUP_COUNT);
	RTC_ClearITPendingBit(RTC_IT_WUT);
	EXTI_ClearITPendingBit(EXTI_Line22);
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	RTC_WakeUpCmd(ENABLE);
}

/**
  * @function   System_RTC_Init
  * @brief      Set the system RTC and Initalize RTC interrupt.
  * @param[in]  None.  
  * @retval     None.
  */
void System_RTC_Init(void)
{
	Time_Type set_alarm_time;
	RTC_Type initdate;
	/* 00:10:00 calculate sunrise and sunset time and check local time */
	set_alarm_time.time_hours = 00;
	set_alarm_time.time_minutes = 10;
	set_alarm_time.time_seconds = 0;

	/* RTC basic struct Initalization */
	RTC_Config_Init();

	/* Set the time and date 2021/08/14 13:00:00  */
	//initdate.rtc_date.date_year = 0x07E5;  /* 2021 = 2000 + 21 */
	initdate.rtc_date.date_year = 0x15;
	initdate.rtc_date.date_month = 0x08;
	initdate.rtc_date.date_day = 0x0E;
	initdate.rtc_date.date_weekday = 6;
	initdate.rtc_time.time_hours = 0x0D;
	initdate.rtc_time.time_minutes = 0x00;
	initdate.rtc_time.time_seconds = 0x00;
	RTC_TimeAndDate_Set(&initdate);

	PDEBUG("\r[OK] Set the Init time is: %d/%02d/%02d %02d:%02d:%02d\n", initdate.rtc_date.date_year, initdate.rtc_date.date_month, \
		initdate.rtc_date.date_day, initdate.rtc_time.time_hours, initdate.rtc_time.time_minutes, \
		initdate.rtc_time.time_seconds);

	/* RTC Alarm interrupt */
	RTC_Alarm_Config(RTC_Alarm_A, &set_alarm_time);

	/* RTC Wakeup interrupt */
	RTC_Wakeup_Config();
}

/**
  * @function   Conv_TimeToSecond
  * @brief      convert the time struct hour,minute,second to second.
  * @param[in]  conv_time: time to be convert the second.
  * 			The time must the 24 Hour Format.
  * @retval     according the time to convert second value.
  */
uint32_t Conv_TimeToSecond(Time_Type *conv_time)
{
	return ((conv_time->time_hours * 3600) + \
		   (conv_time->time_minutes * 60) + \
		   + conv_time->time_seconds);	   
}

/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/

