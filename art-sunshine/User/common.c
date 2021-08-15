/**
  ******************************************************************************
  * @file    common.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file provides all the operation and data struct type for whole 
  *          project.
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
#include "common.h"

/** @addtogroup Common_Driver
  * @{
  */
  
/** @defgroup Common control
  * @brief Common control driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
  
/** @defgroup Common_Functions
  * @{
  */
  
/**
  * @fuction Cal_CRC16
  * @brief   Calculates the CRC-16 check value for a string of data buffer.
  * @param   databuf: a string of data to be calculted.
  *			 datalen: the length of the databuf.
  *			 typex: CRC16 check type.
  * @retval  CRC-16check results.
  */
uint16_t Cal_CRC16(CRC16x_Type typex, uint8_t *databuf, uint32_t datalen)
{
	/* Choose Initalization */
	uint16_t result;
	switch (typex)
		{
		case CRC16_MODBUS:
			result = 0xFFFF;
			break;
		case CRC16_IBM:
		default:
			result = 0x0000; 
			break;
		}
	/* Caculation result */
	uint16_t table_No = 0;
	uint32_t index;
	for (index = 0; index < datalen; ++index)
	{
		table_No = ((result & 0xFF) ^ (databuf[index] & 0xFF));
		result = ((result >> 8) & 0xFF) ^ CRC16_Table[table_No];
	}
	/* Return the Calculated Result */
	return result;
}  

/**
  * @fuction Cal_CRC16_X25
  * @brief   Calculates the CRC-16/X25 check value for a string of data buffer.
  * @param   databuf: a string of data to be calculted.
  *			 datalen: the length of the databuf.
  * @retval  CRC-16/X25 check results.
  */
uint16_t Cal_CRC16_X25(uint8_t *databuf, uint32_t datalen)
{
	uint16_t result = 0xFFFF;
	uint8_t  index;
	while(datalen--)
	{
		result ^= *databuf++;
		for (index = 0; index < 8; ++index)
		{
			if (result & 0x0001)
			{
				result >>= 1;
				result ^= 0x8408;
			}
			else
			{
				result >>= 1;
			}
		}
	}
	result ^= 0xFFFF;
	return result;
}
/**
  * @fuction simple_delay
  * @brief   Implement simple delay operation in the programe.
  * @param   delay_nums: The time number of loop.
  * @retval  none.
  */
void simple_delay(uint32_t delay_nums)
{
	uint32_t temp;
	for (temp = 0; temp < delay_nums; ++temp);
}

/**
  * @fuction Extractstr_fromstr
  * @brief   According to the start and end strings, Extract \
  *			 the string from the sourcestr store to the savestr.
  * @param   sourcestr: source string. startstr: find string start concent.
  *			 endstr: find string end concent. savetostr: save the concent
  *			 from start to end pointer address
  * @retval  none.
  */
FunStatus Extractstr_fromstr(char *sourcestr, char *startstr, char*endstr, char *savetostr)
{
	int start_length, copysize;
	char *tempaddr;
	
	/* check the sourcestr & savetostr is not empty */
	if ((sourcestr == NULL) || (savetostr == NULL))
		return R_ERROR;

	/* calcluation length for startstr */
	start_length = strlen(startstr);

	/* find startstr string address and size for (endstr - startstr) */
	tempaddr = strstr(sourcestr, startstr);
	copysize = ((strstr(sourcestr, endstr) - tempaddr) - start_length);

	/* store the string to save */
	if (copysize < 0)  /* address error */
		return R_ERROR;
	
	strncpy(savetostr, tempaddr + start_length, copysize);

	return R_OK;
	
}

/**
  * @fuction InttoString
  * @brief   Converts an integer to a string.
  * @param   value: int value.
  *			 dstptr: string store convered.
  * @retval  none.
  */
void InttoString(int value, char *dstptr)
{
	uint8_t loop, result, flag;
    uint32_t div = 1000000000;
	
	/* if value is negative */
    if (value < 0) {
        *dstptr++ = '-';
        value = -value;
    }

	/* Convert bit by bit */
    flag = 0;
    for (loop = 0; loop < 9; loop++) {
        result = (uint8_t)(value / div);
        if (result > 0 || 1 == flag) {
            *dstptr++ = result + '0';
            flag = 1;
        }
        value %= div;
        div /= 10;
    }
    *dstptr++ = value + '0';
    *dstptr = 0;
}

/**
* @fuction DectoHex.
* @brief   Converts a decimal number to hexadecimal number. 
* @param[in] decstr: Array pointer for decimal number needs to be converted.
* @retval  Function operation status or result for convert to hexadecimal numbers.
*	@reg 0XFF: input pointer is equal NULL.
*	@reg other: one hexadecimal numbers for decstr converted.
* @warning only one hexadecimal number will be convered.
*/
uint8_t DectoHex(uint8_t *decstr)
{
	uint8_t tempch[2];
	uint8_t temp;

	/* first items */
	temp = (decstr[0] * 10) + decstr[1];
	if (temp > 99)
	{
		return 0xFF;
	}
	tempch[0] = temp / 16;

	/* return result */
	return (tempch[0] << 4) + (temp % 16);
}

/**
* @fuction Bcd_To_Ascii.
* @brief   BCD Code convert to ASCII Code. 
* @param[in] srcptr: BCD code pointer.
* @param[in][out] dstptr: convered Ascii code.
* @param[in] count: counter of the convert BCD code.
* @retval The length of the dstptr ascii code length.
* @example:
*	@arg:[input]£º 0x25, *arrary, 1, 2.
*	@arg:[output]: 0x32, 0x35.
*/
uint16_t Bcd_To_Ascii(uint8_t *srcptr, uint8_t *dstptr, uint8_t count, uint8_t dstlen)
{
	uint8_t msb, lsb;
	
	/* BCD code convert the ASCII code */
    for (uint8_t i = 0; i < count; i++)
    {
    	/* convert MS-Byte */
        msb = *srcptr >> 4;
        *dstptr++ = (msb <= 9) ? (msb + '0') : (msb + 'A' - 10);
		
		/* convert Low Byte */
        lsb = *srcptr & 0x0F;
        *dstptr++ = (lsb <= 9) ? (lsb + '0') : (lsb + 'A' - 10);

        if (dstlen == 3)
        {
            *dstptr++ = ' ';
        }
		/* dstptr pointer increase */
        srcptr++;
    }
    return (count * dstlen);
}

/**
* @fuction Bcd_To_String.
* @brief   BCD Code convert to string. 
* @param[in] srcptr: BCD code pointer.
* @param[out] dstptr: string pointer.
* @param[in] count: The length that how many counters BCD to convert
* @retval OPeration Status.
*	@arg: Operation Successful.
*	@arg: Operation Failed.
*/
uint8_t Bcd_To_String(uint8_t *srcptr, uint8_t *dstptr, uint8_t count)
{
	uint8_t msb, lsb;
	
	/* check data invaild */
	if ((srcptr == NULL) || (dstptr == NULL) || (count < 1))
	{
		return R_ERROR;
	}

	/* convert data */
	for (uint8_t index = 0; index < count; index++)
	{
		msb = (srcptr[index] >> 4) & 0x0F;
		lsb =  srcptr[index] & 0x0F; 

		*dstptr++ = msb <= 9 ? (msb + 0x30) : (msb + 0x40 - 10);
		*dstptr++ = lsb <= 9 ? (lsb + 0x30) : (lsb + 0x40 - 10);
	}

	/* add the space */
	*dstptr++ = ' ';

 	/* end */
	return R_OK;
}

/**
  * @function   bcd_to_decimal
  * @brief      BCD number convert to decimal number.
  * @param[in]  bcd_number: BCD number.
  * @retval     None.
  */ 
uint8_t bcd_to_decimal(uint8_t bcd_number)
{
	if (bcd_number >= 100)
		return 0x00;
	/* Decimal number */
	return ((bcd_number >> 4) & 0x0F) * 10 + (bcd_number& 0x0F);
}

/**
  * @function   decimal_to_bcd
  * @brief      decimal number convert to bcd number.
  * @param[in]  decimal_number: decimal number.
  * @retval     None.
  */ 
uint8_t decimal_to_bcd(uint8_t decimal_number)
{
	if (decimal_number >= 100)
		return 0x00;

	/* BCD number */
	return ((decimal_number / 10) << 4) + (decimal_number % 10);
}


/**
 * @fuction   calmemsize
 * @brief     calcuter the memory size for sysytem need..
 * @param[in] needsize: need memory size foe sysytem.
 * @retval    depend on the parameters output the size for system dynamic allocated.
 */
uint16_t calmemsize(uint16_t needsize)
{
	if (needsize > BIGGEST_MEMSIZE)
		return 0;
	else if (needsize > LARGE_MIMSIZE)
		return BIGGEST_MEMSIZE;
	else if (needsize > HUGE_MEMSIZE)
		return LARGE_MIMSIZE;
	else if (needsize > MEDIUM_MEMSIZE)
		return HUGE_MEMSIZE;
	else if (needsize > SMALL_MEMSIZE)
		return MEDIUM_MEMSIZE;
	else
		return SMALL_MEMSIZE;
}

/**
  * @function  gcvt_user
  * @brief	   Converts a floating-point number greater than 0 and less than 100 with only one decimal point.
  * @param[in] x: only one decimal point float numbers  will be convert.
  * @param[in] buf: store the string from converted float, buf size is more that 4.
  * @retval    None.
  */
void gcvt_user(float x, char *buf)
{
    char temp;
    if ((x < 1.0F) && (x >= 0.0F))  /* 0.x */
    {
       buf[0] = 0 + '0';
       buf[1] = '.';
       buf[2] = (x * 10.0F) + '0';
       buf[3] = 0;
    }
    else if (x < 10.0F)				/* x.x */
    {
       buf[0] = floor(x) + '0';
       buf[1] = '.';
       buf[2] = ((x - floor(x)) * 10.0F) + '0';
       buf[3] = 0;

    }else if (x < 100.0F)			/* xx.xx */
    {
        temp = floor(x) / 10.0F;
        buf[0] = temp + '0';
        buf[1] = x - (temp * 10.0F)  + '0';
        buf[2] = '.';
        buf[3] = (x - floor(x)) * 10.0F + '0';
        buf[4] = 0;
    }
    else
    {
        buf = NULL;
    }
}

/**
  * @function   print_uart_data
  * @brief      print the uart port send and receive data.
  * @param[in]  desrc: desrciption information.
  * @param[in]  data: the data will be print.
  * @param[in]  len: the data length.
  * @retval     None.
  */
void print_uart_data(char *desrc, uint8_t *data, uint8_t len)
{
	/* description */
	PDEBUG("\r%s:", desrc);
				
	for (uint8_t index = 0; index <=len; index++)
	{
		PDEBUG(" %02X", data[index]);
	}
	PDEBUG("\n");
}

/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/
