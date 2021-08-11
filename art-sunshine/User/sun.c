/**
  ******************************************************************************
  * @file    sun.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file provides all the related  sun operation  and  calculater 
  *          functions.
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
#include "sun.h"
#include "motor.h"
#include "user.h"
/** @addtogroup SUN_Driver
  * @{
  */
  
/** @defgroup SUN 
  * @brief SUN driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define MOTOR_ALL_WORK_OK 0x0F	/* 0000DCBA */
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
  
/** @defgroup SUN_Private_Functions
  * @{
  */
  
/**
  * @function  SunshineControl_Task
  * @brief	   The task of control console by calculater sunrise and sunset.
  * @param[in] pvParameters: default.
  * @retval    None
  */
void SunshineControl_Task(void *pvParameters)
{
	RTC_Type curtime;
	uint32_t curtime_second;
	uint8_t motor_state = 0x00; 
	uint8_t temp;
	motor_operation_t *motor_t;
	vTaskDelay(pdMS_TO_TICKS(5000));	/* wait the motor selt chech finish */
	
	while(TRUE)
	{
		PDEBUG("\rSunshineControl_Task is Runing.\n");
		/* get the current time */

		/* get the system motor status */
		motor_state = get_system_motor_check_state();
		if (motor_state == MOTOR_ALL_WORK_OK)
		{
			curtime = RTC_TimeAndDate_Get();
			curtime_second = Conv_TimeToSecond(&curtime.rtc_time);
			/* choose the system mode and control */
			switch (Art_Sunshine_Info.mode)
			{
				case NORMAL_MODE:
					/* Normal mode operation function */
					Normal_Mode_Operation(curtime_second, curtime);
					
				break;

				case EXCEPTION_MODE:
					/* exception mode operation function */
					Exception_Mode_Operation(curtime_second, curtime);
			
				break;

				case CLEAR_MODE:
					/* clear and default mode operation function */
					Clear_Mode_Operation(curtime_second, curtime);
				
				break;

				default:
				break;
			}
		}
		else /* set the motor seft checking */
		{
			for (uint8_t index = 0; index < POINTER_MAX_MOTOR; index++)
			{
				motor_t = &motor_opr[index];
				if ((motor_state >> index) & 0x01)
				{
					motor_self_checking(motor_t);
				}
			}
		}
		vTaskDelay(pdMS_TO_TICKS(Art_Sunshine_Info.control_interval));
	}
}

/* The "workhorse" function for sun rise/set times */
static int __sunriset__( int year, int month, int day, float lon, float lat,
                  float altit, int upper_limb, float *trise, float *tset )
/***************************************************************************/
/* Note: year,month,date = calendar date, 1801-2099 only.             */
/*       Eastern longitude positive, Western longitude negative       */
/*       Northern latitude positive, Southern latitude negative       */
/*       The longitude value IS critical in this function!            */
/*       altit = the altitude which the Sun should cross              */
/*               Set to -35/60 degrees for rise/set, -6 degrees       */
/*               for civil, -12 degrees for nautical and -18          */
/*               degrees for astronomical twilight.                   */
/*         upper_limb: non-zero -> upper limb, zero -> center         */
/*               Set to non-zero (e.g. 1) when computing rise/set     */
/*               times, and to zero when computing start/end of       */
/*               twilight.                                            */
/*        *rise = where to store the rise time                        */
/*        *set  = where to store the set  time                        */
/*                Both times are relative to the specified altitude,  */
/*                and thus this function can be used to compute       */
/*                various twilight times, as well as rise/set times   */
/* Return value:  0 = sun rises/sets this day, times stored at        */
/*                    *trise and *tset.                               */
/*               +1 = sun above the specified "horizon" 24 hours.     */
/*                    *trise set to time when the sun is at south,    */
/*                    minus 12 hours while *tset is set to the south  */
/*                    time plus 12 hours. "Day" length = 24 hours     */
/*               -1 = sun is below the specified "horizon" 24 hours   */
/*                    "Day" length = 0 hours, *trise and *tset are    */
/*                    both set to the time when the sun is at south.  */
/*                                                                    */
/**********************************************************************/
{
      float  d,  /* Days since 2000 Jan 0.0 (negative before) */
      sr,         /* Solar distance, astronomical units */
      sRA,        /* Sun's Right Ascension */
      sdec,       /* Sun's declination */
      sradius,    /* Sun's apparent radius */
      t,          /* Diurnal arc */
      tsouth,     /* Time when Sun is at south */
      sidtime;    /* Local sidereal time */

      int rc = 0; /* Return cde from function - usually 0 */

      /* Compute d of 12h local mean solar time */
      d = days_since_2000_Jan_0(year,month,day) + 0.5F - lon/360.0F;

      /* Compute the local sidereal time of this moment */
      sidtime = revolution( GMST0(d) + 180.0F + lon );

      /* Compute Sun's RA, Decl and distance at this moment */
      sun_RA_dec( d, &sRA, &sdec, &sr );

      /* Compute time when Sun is at south - in hours UT */
      tsouth = 12.0F - rev180(sidtime - sRA) / 15.0F;

      /* Compute the Sun's apparent radius in degrees */
      sradius = 0.2666F / sr;

      /* Do correction to upper limb, if necessary */
      if ( upper_limb )
            altit -= sradius;

      /* Compute the diurnal arc that the Sun traverses to reach */
      /* the specified altitude altit: */
      {
            float cost;
            cost = ( sind(altit) - sind(lat) * sind(sdec) ) /
                  ( cosd(lat) * cosd(sdec) );
            if ( cost >= 1.0F )
                  rc = -1, t = 0.0F;       /* Sun always below altit */
            else if ( cost <= -1.0F )
                  rc = +1, t = 12.0F;      /* Sun always above altit */
            else
                  t = acosd(cost) / 15.0F;   /* The diurnal arc, hours */
      }

      /* Store rise and set times - in hours UT */
      *trise = tsouth - t;
      *tset  = tsouth + t;
      return rc;
}  /* __sunriset__ */

/* This function computes the Sun's position at any instant */

static void sunpos( float d, float *lon, float *r )
/******************************************************/
/* Computes the Sun's ecliptic longitude and distance */
/* at an instant given in d, number of days since     */
/* 2000 Jan 0.0.  The Sun's ecliptic latitude is not  */
/* computed, since it's always very near 0.           */
/******************************************************/
{
      float M,         /* Mean anomaly of the Sun */
             w,         /* Mean longitude of perihelion */
                        /* Note: Sun's mean longitude = M + w */
             e,         /* Eccentricity of Earth's orbit */
             E,         /* Eccentric anomaly */
             x, y,      /* x, y coordinates in orbit */
             v;         /* True anomaly */

      /* Compute mean elements */
      M = revolution( 356.0470F + 0.985600F * d );
      w = 282.9404F + 4.70935E-5F * d;
      e = 0.016709F - 1.151E-9F * d;

      /* Compute true longitude and radius vector */
      E = M + e * RADEG * sind(M) * ( 1.0F + e * cosd(M) );
            x = cosd(E) - e;
      y = sqrt( 1.0F - e*e ) * sind(E);
      *r = sqrt( x*x + y*y );              /* Solar distance */
      v = atan2d( y, x );                  /* True anomaly */
      *lon = v + w;                        /* True solar longitude */
      if ( *lon >= 360.0F )
            *lon -= 360.0F;                   /* Make it 0..360 degrees */
}

static void sun_RA_dec( float d, float *RA, float *dec, float *r )
/******************************************************/
/* Computes the Sun's equatorial coordinates RA, Decl */
/* and also its distance, at an instant given in d,   */
/* the number of days since 2000 Jan 0.0.             */
/******************************************************/
{
      float lon, obl_ecl, x, y, z;

      /* Compute Sun's ecliptical coordinates */
      sunpos( d, &lon, r );

      /* Compute ecliptic rectangular coordinates (z=0) */
      x = *r * cosd(lon);
      y = *r * sind(lon);

      /* Compute obliquity of ecliptic (inclination of Earth's axis) */
      obl_ecl = 23.4393F - 3.563E-7F * d;

      /* Convert to equatorial rectangular coordinates - x is unchanged */
      z = y * sind(obl_ecl);
      y = y * cosd(obl_ecl);

      /* Convert to spherical coordinates */
      *RA = atan2d( y, x );
      *dec = atan2d( z, sqrt(x*x + y*y) );

}  /* sun_RA_dec */


/******************************************************************/
/* This function reduces any angle to within the first revolution */
/* by subtracting or adding even multiples of 360.0 until the     */
/* result is >= 0.0 and < 360.0                                   */
/******************************************************************/

#define INV360    ( 1.0F / 360.0F )

static float revolution( float x )
/*****************************************/
/* Reduce angle to within 0..360 degrees */
/*****************************************/
{
      return( x - 360.0F * floor( x * INV360 ) );
}  /* revolution */

float rev180( float x )
/*********************************************/
/* Reduce angle to within +180..+180 degrees */
/*********************************************/
{
      return( x - 360.0F * floor( x * INV360 + 0.5F ) );
}  /* revolution */

/*******************************************************************/
/* This function computes GMST0, the Greenwich Mean Sidereal Time  */
/* at 0h UT (i.e. the sidereal time at the Greenwhich meridian at  */
/* 0h UT).  GMST is then the sidereal time at Greenwich at any     */
/* time of the day.  I've generalized GMST0 as well, and define it */
/* as:  GMST0 = GMST - UT  --  this allows GMST0 to be computed at */
/* other times than 0h UT as well.  While this sounds somewhat     */
/* contradictory, it is very practical:  instead of computing      */
/* GMST like:                                                      */
/*                                                                 */
/*  GMST = (GMST0) + UT * (366.2422/365.2422)                      */
/*                                                                 */
/* where (GMST0) is the GMST last time UT was 0 hours, one simply  */
/* computes:                                                       */
/*                                                                 */
/*  GMST = GMST0 + UT                                              */
/*                                                                 */
/* where GMST0 is the GMST "at 0h UT" but at the current moment!   */
/* Defined in this way, GMST0 will increase with about 4 min a     */
/* day.  It also happens that GMST0 (in degrees, 1 hr = 15 degr)   */
/* is equal to the Sun's mean longitude plus/minus 180 degrees!    */
/* (if we neglect aberration, which amounts to 20 seconds of arc   */
/* or 1.33 seconds of time)                                        */
/*                                                                 */
/*******************************************************************/

static float GMST0( float d )
{
      float sidtim0;
      /* Sidtime at 0h UT = L (Sun's mean longitude) + 180.0 degr  */
      /* L = M + w, as defined in sunpos().  Since I'm too lazy to */
      /* add these numbers, I'll let the C compiler do it for me.  */
      /* Any decent C compiler will add the constants at compile   */
      /* time, imposing no runtime or code overhead.               */
      sidtim0 = revolution( ( 180.0F + 356.0470F + 282.9404F ) +
                          ( 0.985600F + 4.70935E-5F ) * d );
      return sidtim0;
}  /* GMST0 */

/**
  * @name	CalculateSunHsAs
  * @brief	Calculate the pitch and azimuth of the sun.
  * @param[in]	nows: time and date.
  * @param[in]	lons: Longitude of the earth.
  * @param[in]	lats: Latitude on earth.
  * @param[in][out]	azimuths: azimuths of the sun.
  * @param[in][out]	altangles: altangles of the sun.
  * @retval 	None.
  */
static void CalculateSunHsAs(DateTime *now, float lon, float lat, float *azimuth, float *altangle)
{
    float m_sita, m_Ed, m_tau, m_Et, m_N0, m_U, m_K, m_Sd, m_L, m_H, m_N;
    float lat_radian;

    /* Calculate the declination */
    m_K = 2 * PI / 365.2422F;
    m_N0 = 79.6764F + 0.2422F * (now->Year - 1985) - (int)((now->Year - 1985) / 4);
    m_L = lon / 15.0F;
    m_H = now->Hour - 8 + now->Minute / 60;
    m_N = Get_DayNumber(now->Year, now->Month, now->Day) + ((m_H - m_L) / 24.0F);
    m_sita = (m_N - m_N0) * m_K;
    m_Ed = 0.3723F + 23.2567F * sin(m_sita) + 0.1149F * sin(2 * m_sita) - 0.1712F * sin(3 * m_sita) - 0.758F * cos(m_sita) + 0.3656F * cos(2 * m_sita) + 0.0201F * cos(3 * m_sita);//³àÎ³(½Ç¶È)

    /* Calculate the solar hour Angle */
    m_Et = 0.0028F - 1.9857F * sin(m_sita) + 9.9059F * sin(2 * m_sita) - 7.0924F * cos(m_sita) - 0.6882F * cos(2 * m_sita);
    m_Sd = now->Hour + (now->Minute - (120 - lon) * 4) / 60;
    m_U = m_Sd + m_Et / 60;
    m_tau = (m_U + (m_U - (int)m_U) / 60 - 12) * 15;

    /* Calculated altitude Angle */
    lat_radian = asin(sin(m_Ed * Ang_Rev_Rad) * sin(lat * Ang_Rev_Rad) + cos(m_Ed * Ang_Rev_Rad) * cos(lat * Ang_Rev_Rad) * cos(m_tau * Ang_Rev_Rad));
    *altangle = lat_radian * Rad_Rev_Ang;

    /* Calculate the azimuth */
    *azimuth  = acos(-(sin(lat_radian) * sin(lat * Ang_Rev_Rad) - sin(Ang_Rev_Rad * m_Ed)) / (cos(lat_radian) * cos(lat * Ang_Rev_Rad)));
}

/**
  * @name	Get_DayNumber
  * @brief	calculater the day is the of the year.
  * @param[in]	year: year, eg: 2020
  * @param[in]	month: month 1 - 12.
  * @param[in]	day: day 1 - 31.
  * @retval 	the day of the year, 0 < x < 365 or 366.
  */
static int Get_DayNumber(int year, int month, int day)
{
	int leap;
	int accum_days[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	/* decide if this year is a leap year then set Feburary is 29 days */
	leap = 0;
	if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
		leap = 1;

	/* calculater days and return */
	month--;
    return (day + accum_days[month] + leap);
}

/**
  * @name	CalculateSunElanAzan
  * @brief	Calculate the pitch and azimuth of the sun.
  * @param[in]	nows: time and date.
  * @param[in]	lons: Longitude of the earth.
  * @param[in]	lats: Latitude on earth.
  * @param[in][out]	azimuths: azimuths of the sun.
  * @param[in][out]	altangles: altangles of the sun.
  * @retval 	None.
  */
static void  CalculateSunElanAzan(DateTime *nows, float lons, float lats, float *azimuths, float *altangles)
{
    /* calculater the real */
    float temp1, temp2;
    temp1 = 0.0F;
    temp2 = 0.0F;
    CalculateSunHsAs(nows, lons, lats, &temp1, altangles);

    /* increases in time */
    nows->Minute += 1;
    CalculateSunHsAs(nows, lons, lats, &temp2, altangles);

    /* judge curver */
    if ((temp2 -temp1) > 0.0F)
    {
        temp1 *= Rad_Rev_Ang;
        temp1 = (180.0F - temp1) + 180.0F;
    }
    else
        temp1 *= Rad_Rev_Ang;

    /* assignment */
    *azimuths = 360.0F - temp1;
}

/**
  * @name	ClaculSun_AltAzi
  * @brief	Calculate the pitch and azimuth of the sun.
  * @param[in]	data_time: time and date.
  * @param[in]	latlong: Longitude and Latitude of the earth.
  * @param[in][out]	altangles: altangles of the sun.
  * @retval 	Alta_Azim type constant azimuths and altangles.
  */
Alta_Azim ClaculSun_AltAzi(RTC_Type data_time, Lati_Longi latlong)
{
	DateTime curtime_date;
	Alta_Azim temp;

	/* write the data and time to DateTime data struct */
	curtime_date.Year = data_time.rtc_date.date_year;
	curtime_date.Month = data_time.rtc_date.date_month;
	curtime_date.Day = data_time.rtc_date.date_day;
	curtime_date.Hour = data_time.rtc_time.time_hours;
	curtime_date.Minute = data_time.rtc_time.time_minutes;
	
	/* calculater the pitch and azimuth */
	CalculateSunElanAzan(&curtime_date, latlong.longitude, latlong.latitude, &temp.azimuths, &temp.altangles);

	/* return result */
	return temp;
}

/**
  * @name	ClaculSun_RiseSet
  * @brief	Calculate the sunrise and sun set time.
  * @param[in]	data_time: time and date.
  * @param[in]	latlong: Longitude and Latitude of the earth.
  * @param[in][out]	altangles: altangles of the sun.
  * @retval 	Suntime_Type sunrise and sunset time.
  */
Suntime_Type ClaculSun_RiseSet(RTC_Type data_time, Lati_Longi latlong)
{
	Suntime_Type temp;
	int year, month, day;
	float sunrise, sunset;
	uint8_t time[4];
	
	/* set the date */
	year = data_time.rtc_date.date_year;
	month = data_time.rtc_date.date_month;
	day = data_time.rtc_date.date_day;
	
	/* calculater sunrise and sunset */
	sun_rise_set(year, month, day, latlong.longitude, latlong.latitude, &sunrise, &sunset);
	/* China is East Eight Zone. */
	sunrise += CHINA_ZONE;
	sunset += CHINA_ZONE;

	/* sunrise time */
	time[0] = floor(sunrise);
	time[1] = floor((sunrise - time[0]) * 60.0F);

	/* sunset time */
	time[2] = floor(sunset);
	time[3] = floor((sunset - time[2]) * 60.0F);

	/* set the result to temp */
	temp.sunrise_time.time_hours = time[0];
	temp.sunrise_time.time_minutes = time[1];
	temp.sunrise_time.time_seconds = 0;

	temp.sunset_time.time_hours = time[2];
	temp.sunset_time.time_minutes =	time[3];
	temp.sunset_time.time_seconds = 0;
	
	/* covert the time to second */
	temp.sunrise_second = Conv_TimeToSecond(&temp.sunrise_time);
	temp.sunset_second = Conv_TimeToSecond(&temp.sunset_time);

	/* return result */
	return temp;	
}

/**
  * @name	Normal_Mode_Operation
  * @brief	Normal mode opeartion control console.
  * @param[in]	second_value: Converts the current time to the second value.
  * @param[in]	date_time: current time and date.
  * @retval 	None.
  */
void Normal_Mode_Operation(uint32_t second_value, RTC_Type date_time)
{
	PDEBUG("\rSystem entry the normal mode !\n");
	Alta_Azim altazi_value;
	float32_t angle;
	
	/* check the time is day or night? */
	if ((second_value > Art_Sunshine_Info.sunrise_set.sunrise_second) && (second_value < Art_Sunshine_Info.sunrise_set.sunset_second))
	{
		/* Calculate the azimuth and pittch angles of the sun. */
		altazi_value = ClaculSun_AltAzi(date_time, Art_Sunshine_Info.HangZhou);

		/* ptintf the importmant parameters */
		PDEBUG("\rDate: %d/%02d/%02d - Time: %02d:%02d:%02d\n", date_time.rtc_date.date_year, date_time.rtc_date.date_month, date_time.rtc_date.date_day, \
												   date_time.rtc_time.time_hours, date_time.rtc_time.time_minutes, date_time.rtc_time.time_seconds);
		PDEBUG("\rThe altazi_value is: altangle: %f\t azimuths: %f\t\n", altazi_value.altangles, altazi_value.azimuths);

		/* Operte A motor to run */
		angle = altazi_value.altangles - mootra.angle;
		if (angle > 0.0f)
			motor_run(angle);

		/* Operte B motor to run */

		/* Operte C motor to run */

		/* Operte D motor to run */
	}
	else
	{
		/* Operate the A motor to definite state */

		/* Operate the B motor to definite state */

		/* Operate the C motor to definite state */

		/* Operate the D motor to definite state */
		
		PDEBUG("\rThe sunrise time: %d : %d\n", Art_Sunshine_Info.sunrise_set.sunrise_time.time_hours,  Art_Sunshine_Info.sunrise_set.sunrise_time.time_minutes);
		PDEBUG("\rGood night!\n");
	}
}

/**
  * @name	Exception_Mode_Operation
  * @brief	Excption mode opeartion control console.
  * @param[in]	second_value: Converts the current time to the second value.
  * @param[in]	date_time: current time and date.
  * @retval 	None.
  */
void Exception_Mode_Operation(uint32_t second_value, RTC_Type date_time)
{
	PDEBUG("\rSystem entry the exception mode !\n");

	/* display the exception cause */

	/* Operate the A motor to definite state */

	/* Operate the B motor to definite state */

	/* Operate the C motor to definite state */

	/* Operate the D motor to definite state */
}

/**
  * @name	Clear_Mode_Operation
  * @brief	Excption mode opeartion control console.
  * @param[in]	second_value: Converts the current time to the second value.
  * @param[in]	date_time: current time and date.
  * @retval 	None.
  */
void Clear_Mode_Operation(uint32_t second_value, RTC_Type date_time)
{
	PDEBUG("\rSystem entry the clear mode !\n");

	
}


/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/
