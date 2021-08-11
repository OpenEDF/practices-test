/**
  ******************************************************************************
  * @file    sun.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file contains  all  the  functions  prototypes  for  the  sun   
  *          related operation and calculater.
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
#ifndef __SUN__H__
#define __SUN__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "projdefs.h"
#include "led.h"
#include "rtc.h"


/** @addtogroup SUN_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/* 
 * struct: @ Loacal latitude and longitude. 
*/
typedef struct {
	float32_t latitude;			   /* latitude value  */
	float32_t longitude;		   /* longitude value */
} Lati_Longi;

/* 
 * struct: @ The Sun pitch Angle and azimuth Angle. 
*/
typedef struct {
	float32_t altangles;		  /* pitch Angle value  */
	float32_t azimuths;			  /* azimuth Angle value */
} Alta_Azim;

/* 
 * struct: @ The sunrise time and sunset time. 
*/
typedef struct {
	Time_Type sunrise_time;		/* The sunrise time */
	Time_Type sunset_time;		/* The sunset time */
	uint32_t sunrise_second;	/* The sunrise time convert to second */
	uint32_t sunset_second;		/* The sunset time convert to second */
} Suntime_Type;

/* 
 * struct: @ default date and time struct about sun. 
 */
typedef struct{
	int Year;
	int Month;
	int Day;
	int Hour;
	int Minute;
}DateTime;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* A macro to compute the number of days elapsed since 2000 Jan 0.0 */
/* (which is equal to 1999 Dec 31, 0h UT)                           */
#define days_since_2000_Jan_0(y,m,d) \
    (367L*(y)-((7*((y)+(((m)+9)/12)))/4)+((275*(m))/9)+(d)-730530L)
#ifndef PI    
	#define PI        3.141593F
#endif

/* This macro computes times for sunrise/sunset.                      */
/* Sunrise/set is considered to occur when the Sun's upper limb is    */
/* 35 arc minutes below the horizon (this accounts for the refraction */
/* of the Earth's atmosphere). */
#define sun_rise_set(year,month,day,lon,lat,rise,set)  \
        __sunriset__( year, month, day, lon, lat, -35.0/60.0, 1, rise, set )

#define RADEG     ( 180.0F / PI )
#define DEGRAD    ( PI / 180.0F )
#define CHINA_ZONE	8.0F

#define Ang_Rev_Rad (PI / 180.0F)
#define Rad_Rev_Ang (180.0F / PI)

/* The trigonometric functions in degrees */

#define sind(x)  sin((x)*DEGRAD)
#define cosd(x)  cos((x)*DEGRAD)
#define tand(x)  tan((x)*DEGRAD)

#define atand(x)    (RADEG*atan(x))
#define asind(x)    (RADEG*asin(x))
#define acosd(x)    (RADEG*acos(x))
#define atan2d(y,x) (RADEG*atan2(y,x))

/* Exported functions --------------------------------------------------------*/
void SunshineControl_Task(void *pvParameters);
static int __sunriset__( int year, int month, int day, float lon, float lat,
                  float altit, int upper_limb, float *rise, float *set );
static void sunpos( float d, float *lon, float *r );
static void sun_RA_dec(float d, float *RA, float *dec, float *r);
static float revolution(float x);
static float rev180(float x);
static float GMST0(float d);
static void CalculateSunHsAs(DateTime *now, float lon, float lat, float *azimuth, float *As);
static int Get_DayNumber(int year, int month, int day);
static void CalculateSunElanAzan(DateTime *nows, float lons, float lats, float *azimuths, float *altangles);
Alta_Azim ClaculSun_AltAzi(RTC_Type data_time, Lati_Longi latlong);
Suntime_Type ClaculSun_RiseSet(RTC_Type data_time, Lati_Longi latlong);
void Normal_Mode_Operation(uint32_t second_value, RTC_Type date_time);
void Exception_Mode_Operation(uint32_t second_value, RTC_Type date_time);
void Clear_Mode_Operation(uint32_t second_value, RTC_Type date_time);	

#ifdef __cplusplus
}
#endif

#endif  /*__SUN__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
