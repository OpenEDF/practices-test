/**
  ******************************************************************************
  * @file    rtc.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file contains  all  the  functions  prototypes  for  the  rtc   
  *          set-get and configure.
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
#ifndef __RTC__H__
#define __RTC__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_rtc.h"
#include "common.h"
/** @addtogroup RTC_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/* 
 * struct: @time type 
*/
typedef struct {
	uint8_t time_hours;			/* parameter for time hour 00-23 */
	uint8_t time_minutes;		/* parameter for time minute 00-59 */
	uint8_t time_seconds;		/* parameter for time minute 00-59 */
} Time_Type;

/* 
 * struct: @Date type 
 */
typedef struct {
	uint16_t date_year;		/* year  */
	uint8_t date_month;     /* month 1 - 12 */
	uint8_t date_day;		/* day   1 - 28or29or30or31*/
	uint8_t date_weekday;	/* week  1 - 7 */
} Date_Type;

/* 
 * struct: @RTC type 
 */
typedef struct {
	Date_Type rtc_date;		/* RTC date */
	Time_Type rtc_time;		/* RTC time */
} RTC_Type;

/* Exported constants --------------------------------------------------------*/
extern uint32_t second_count;		/* RTC second */
extern uint8_t Alarm_Flag;			/* RTC Alarm interrupt Flag */
extern uint8_t Second_Flag;			/* RTC Second Flag */

/* Exported macro ------------------------------------------------------------*/
#define RTC_WACKUP_COUNT	(2039)	/* RTC Wake up conuter */

/* Exported functions --------------------------------------------------------*/
static void RTC_Config_Init(void);
ErrorStatus RTC_TimeAndDate_Set(RTC_Type *date_time);
RTC_Type RTC_TimeAndDate_Get(void);
static void RTC_Alarm_Config(uint32_t RTC_Alarm_X, Time_Type *alarm_time);
static void RTC_Wakeup_Config(void);
void System_RTC_Init(void);
uint32_t Conv_TimeToSecond(Time_Type *conv_time);

#ifdef __cplusplus
}
#endif

#endif  /*__RTC__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
