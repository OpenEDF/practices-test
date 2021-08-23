/**
  ******************************************************************************
  * @file    flash.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file provides all the operation and configure functions for 
  *          system flash read-write.
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
#include "flash.h" 

/** @addtogroup Flash_Driver
  * @{
  */
  
/** @defgroup Flash control
  * @brief Flash control driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* default local address is shanghai */
/* get latitude and longitude: https://keisan.casio.com/exec/system/1224682277 */

const char *default_version = "Default_version_2020_08_15_V_1_0_0";
System_Mode default_mode = NORMAL_MODE; 
LTE_Status default_lte_status = offline;
CONSOLE_Status default_console_status = fault;
float default_windspeed = 0.0F;
Beaufort_WindType default_windspeed_threshold = Gentle_breeze;
Beaufort_WindType default_wind_level = Hurricane;
Sensor_Status default_winddevice_status = LOST;
uint8_t default_interval_exception = 60;
uint16_t default_interval_normal = 60;   /* 60 * 10 */
uint32_t default_control_interval = (10 * 1000);	/* 10 * 1000  */
//Lati_Longi default_HangZhou =  {31.239692F, 121.499755F};  //TODO: release modify jiangnanbuyi locating
Lati_Longi default_HangZhou =  {30.287459F, 120.153576F};
Suntime_Type default_sunrise_set = {6, 0, 0, 18, 0, 0, 0x5460, 0xFD20};	
Server_Ctl default_serctl_status = de_control;	/* default is de_control */
uint32_t default_syswork_time = 0;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
  
/** @defgroup Flash_Functions
  * @{
  */
  
/**
  * @function	Flash_Erase
  * @brief		Flash Memory Erase Operation.
  * @param[in]	start_addr: Erase Flash Memory operation start address.
  * @param[in]	end_addr: Erase Flash Memory operation end address.
  * @retval    Function Operation result.
  *			   @R_OK: buffer is unpackage successful.
  *			   @R_ERROR: buffer is unpackage failed.
  */  
FunStatus Flash_Erase(uint32_t start_addr, uint32_t end_addr)
{
	/* calculater the sector no */
	uint16_t start_sector, end_sector;
	uint32_t index_sector;
	/* check parameters */
	if ((start_addr < ADDR_FLASH_SECTOR_0) || (end_addr > ADDR_FLASH_SECTOR_11) || (start_addr > end_addr))
		return R_ERROR;

	start_sector = Flash_GetSector(start_addr);
	end_sector = Flash_GetSector(end_addr);

	/* unlock and config flash control register */
	FLASH_Unlock();
	FLASH->SR = ((uint32_t)0x000000F3);

	/* Erase from the start address to end address */
	for (index_sector = start_sector; index_sector <= end_sector; index_sector += 8)
	{
		if ((FLASH_EraseSector(index_sector, VoltageRange_3)) != FLASH_COMPLETE)
			return R_ERROR;
	}
	/* lock teh flash */
	FLASH_Lock();
	
	/* operation successful */
	return R_OK;
}

/**
  * @function	Flash_Program
  * @brief		Flash Memory Programe Operation.
  * @param[in]	start_addr: start address for write data to Flash Memory.
  * @param[in]	data: data will be write to flash memory depend on start_addr.
  * @param[in]	length: The size of byte data write to flash.  
  * @retval    Function Operation result.
  *			   @R_OK: buffer is unpackage successful.
  *			   @R_ERROR: buffer is unpackage failed.
  */ 
FunStatus Flash_Program(uint32_t start_addr, uint8_t *data, uint16_t length)
{
	uint8_t *tempindex;
	/* check parameters */
	if ((start_addr < ADDR_FLASH_SECTOR_0) || (data == NULL) || ((start_addr + length) > FLASH_ADDR_END))
		return R_ERROR;
	
	/* unlock and config flash control register */
	FLASH_Unlock();
	FLASH->SR = ((uint32_t)0x000000F3);

	/* write the data to flash */
	tempindex = data;
	while (length--)
	{
		if (FLASH_ProgramByte(start_addr++, *tempindex++) != FLASH_COMPLETE)
			return R_ERROR;
	}

	/* lock the flash */
	FLASH_Lock();
	
	/* operation successful */
	return R_OK;
}

/**
  * @function	Flash_ReadByte
  * @brief		Read a byte data from flash memory.
  * @param[in]	addr: flash address for will be read.
  * @param[in][out]	end_addr: data: The data from the flash memory address.
  * @retval    Function Operation result.
  *			   @R_OK: buffer is unpackage successful.
  *			   @R_ERROR: buffer is unpackage failed.
  */ 
FunStatus Flash_ReadByte(uint32_t addr, uint8_t *data)
{
	/* check parameters */
	if ((addr < ADDR_FLASH_SECTOR_0) || (addr > ADDR_FLASH_SECTOR_11))
		return R_ERROR;
	
	/* Read the one Byte */
	*data = *(__IO uint8_t *)addr;

	/* return result */
	return R_OK;
}

/**
  * @function	Flash_ReadWord
  * @brief		Read a Word data from flash memory.
  * @param[in]	addr: flash address for will be read.
  * @param[in][out]	end_addr: data: The data from the flash memory address.
  * @retval    Function Operation result.
  *			   @R_OK: buffer is unpackage successful.
  *			   @R_ERROR: buffer is unpackage failed.
  */ 
FunStatus Flash_ReadWord(uint32_t addr, uint32_t *data)
{
	/* check parameters */
	if ((addr < ADDR_FLASH_SECTOR_0) || (addr > ADDR_FLASH_SECTOR_11))
		return R_ERROR;
	
	/* Read the one Byte */
	*data = *(__IO uint32_t *)addr;

	/* return result */
	return R_OK;
}


/**
  * @function	Flash_GetSector
  * @brief		Get the Flash Sector nums according the address.
  * @param[in]	addr: flash memory address.
  * @retval     Flash Sector nums.
  */ 
static uint16_t Flash_GetSector(uint32_t addr)
{
	uint32_t sector = 0;
  	/* calculater the sector no */
  	if((addr < ADDR_FLASH_SECTOR_1) && (addr >= ADDR_FLASH_SECTOR_0))
  	{
    	sector = FLASH_Sector_0;  
  	}
 	 else if((addr < ADDR_FLASH_SECTOR_2) && (addr >= ADDR_FLASH_SECTOR_1))
  	{
    	sector = FLASH_Sector_1;  
  	}
  	else if((addr < ADDR_FLASH_SECTOR_3) && (addr >= ADDR_FLASH_SECTOR_2))
  	{
    	sector = FLASH_Sector_2;  
  	}
 	else if((addr < ADDR_FLASH_SECTOR_4) && (addr >= ADDR_FLASH_SECTOR_3))
  	{
    	sector = FLASH_Sector_3;  
 	 }
  	else if((addr < ADDR_FLASH_SECTOR_5) && (addr >= ADDR_FLASH_SECTOR_4))
  	{
    	sector = FLASH_Sector_4;  
  	}
  	else if((addr < ADDR_FLASH_SECTOR_6) && (addr >= ADDR_FLASH_SECTOR_5))
  	{
    	sector = FLASH_Sector_5;  
  	}
 	else if((addr < ADDR_FLASH_SECTOR_7) && (addr >= ADDR_FLASH_SECTOR_6))
  	{
    	sector = FLASH_Sector_6;  
  	}
  	else if((addr < ADDR_FLASH_SECTOR_8) && (addr >= ADDR_FLASH_SECTOR_7))
  	{
    	sector = FLASH_Sector_7;  
  	}
  	else if((addr < ADDR_FLASH_SECTOR_9) && (addr >= ADDR_FLASH_SECTOR_8))
  	{
    	sector = FLASH_Sector_8;  
  	}
  	else if((addr < ADDR_FLASH_SECTOR_10) && (addr >= ADDR_FLASH_SECTOR_9))
  	{
    	sector = FLASH_Sector_9;  
  	}
  	else if ((addr < ADDR_FLASH_SECTOR_11) && (addr >= ADDR_FLASH_SECTOR_10))
  	{
    	sector = FLASH_Sector_10;  
  	}

	/* return result */
	return sector;
  
}

/**
  * @function	Flash_LoadWorkParam
  * @brief		when system start up, load the system work parameters.
  * @param[in]	None.
  * @retval     Function Operation result.
  *			    @R_OK: buffer is unpackage successful.
  *			    @R_ERROR: buffer is unpackage failed.
  */
FunStatus Flash_LoadWorkParam(void)
{
	FLASH_PARA_STR *paramPtr;
	paramPtr = (FLASH_PARA_STR *)FLASH_WORK_PARAM_ADDR;
	/* load the work parameters from default value */
	if (paramPtr->MemoryId != 'W')
	{
		/* user default parameters */
		strcpy(Art_Sunshine_Info.version, default_version);
		Art_Sunshine_Info.mode = default_mode;
		
		Art_Sunshine_Info.console_status[0] = default_console_status;
		Art_Sunshine_Info.console_status[1] = default_console_status;
		Art_Sunshine_Info.console_status[2] = default_console_status;
		Art_Sunshine_Info.console_status[3] = default_console_status;
		Art_Sunshine_Info.console_status[4] = default_console_status;
		Art_Sunshine_Info.console_status[5] = default_console_status;
		
		Art_Sunshine_Info.windspeed = default_windspeed;
		Art_Sunshine_Info.windspeed_threshold = default_windspeed_threshold;
		Art_Sunshine_Info.wind_level = default_wind_level;
		Art_Sunshine_Info.winddevice_status = default_winddevice_status;
		
		Art_Sunshine_Info.interval_normal = default_interval_normal;
		Art_Sunshine_Info.interval_exception = default_interval_exception;
		Art_Sunshine_Info.control_interval = default_control_interval;
		
		Art_Sunshine_Info.HangZhou = default_HangZhou;
		Art_Sunshine_Info.sunrise_set = default_sunrise_set;
		Art_Sunshine_Info.serctl_status = default_serctl_status;
		
		Art_Sunshine_Info.syswork_time = default_syswork_time;

		/* return result */
		return R_OK;
	}
	
	/* load the work parameters from flash memory */
	strcpy(Art_Sunshine_Info.version, paramPtr->SUNShine_Info.version);
	Art_Sunshine_Info.mode = paramPtr->SUNShine_Info.mode;
	
	Art_Sunshine_Info.console_status[0] = paramPtr->SUNShine_Info.console_status[0];
	Art_Sunshine_Info.console_status[1] = paramPtr->SUNShine_Info.console_status[1];
	Art_Sunshine_Info.console_status[2] = paramPtr->SUNShine_Info.console_status[2];
	Art_Sunshine_Info.console_status[3] = paramPtr->SUNShine_Info.console_status[3];
	Art_Sunshine_Info.console_status[4] = paramPtr->SUNShine_Info.console_status[4];
	Art_Sunshine_Info.console_status[5] = paramPtr->SUNShine_Info.console_status[5];
	
	Art_Sunshine_Info.windspeed = paramPtr->SUNShine_Info.windspeed;
	Art_Sunshine_Info.windspeed_threshold = paramPtr->SUNShine_Info.windspeed_threshold;
	Art_Sunshine_Info.wind_level = paramPtr->SUNShine_Info.wind_level;
	Art_Sunshine_Info.winddevice_status = paramPtr->SUNShine_Info.winddevice_status;
	
	Art_Sunshine_Info.interval_normal = paramPtr->SUNShine_Info.interval_normal;
	Art_Sunshine_Info.interval_exception = paramPtr->SUNShine_Info.interval_exception;
	Art_Sunshine_Info.control_interval = paramPtr->SUNShine_Info.control_interval;
	
	Art_Sunshine_Info.HangZhou = paramPtr->SUNShine_Info.HangZhou;
	Art_Sunshine_Info.sunrise_set = paramPtr->SUNShine_Info.sunrise_set;
	Art_Sunshine_Info.serctl_status = paramPtr->SUNShine_Info.serctl_status;
	
	Art_Sunshine_Info.syswork_time = paramPtr->SUNShine_Info.syswork_time;
	
	return R_OK;
}

/**
  * @function	Flash_SaveWorkParam
  * @brief		save the system to flash work parmeters region.
  * @param[in]	None.
  * @retval     Function Operation result.
  *			    @R_OK: buffer is unpackage successful.
  *			    @R_ERROR: buffer is unpackage failed.
  */
FunStatus Flash_SaveWorkParam(void)
{
	FLASH_PARA_STR *paramPtr;
	FunStatus status;
	/* dynamic allocation memory to store the data */
	if (NULL == (paramPtr = pvPortMalloc(MEDIUM_MEMSIZE)))
	{
		PDEBUG("No more memory to allocated.\n");
		return R_ERROR;
	}
	
	/* save the work parameters to flash memory */
	paramPtr->MemoryId = 'W';
	
	strcpy(paramPtr->SUNShine_Info.version, Art_Sunshine_Info.version);
	paramPtr->SUNShine_Info.mode = Art_Sunshine_Info.mode;

	paramPtr->SUNShine_Info.console_status[0] = Art_Sunshine_Info.console_status[0];
	paramPtr->SUNShine_Info.console_status[1] = Art_Sunshine_Info.console_status[1];
	paramPtr->SUNShine_Info.console_status[2] = Art_Sunshine_Info.console_status[2];
	paramPtr->SUNShine_Info.console_status[3] = Art_Sunshine_Info.console_status[3];
	paramPtr->SUNShine_Info.console_status[4] = Art_Sunshine_Info.console_status[4];
	paramPtr->SUNShine_Info.console_status[5] = Art_Sunshine_Info.console_status[5];

	paramPtr->SUNShine_Info.windspeed = Art_Sunshine_Info.windspeed;
	paramPtr->SUNShine_Info.windspeed_threshold = Art_Sunshine_Info.windspeed_threshold;
	paramPtr->SUNShine_Info.wind_level = Art_Sunshine_Info.wind_level;
	paramPtr->SUNShine_Info.winddevice_status = Art_Sunshine_Info.winddevice_status;

	paramPtr->SUNShine_Info.interval_normal = Art_Sunshine_Info.interval_normal;
	paramPtr->SUNShine_Info.interval_exception = Art_Sunshine_Info.interval_exception;
	paramPtr->SUNShine_Info.control_interval = Art_Sunshine_Info.control_interval;

	paramPtr->SUNShine_Info.HangZhou = Art_Sunshine_Info.HangZhou;
	paramPtr->SUNShine_Info.sunrise_set = Art_Sunshine_Info.sunrise_set;
	paramPtr->SUNShine_Info.serctl_status = Art_Sunshine_Info.serctl_status;

	paramPtr->SUNShine_Info.syswork_time = Art_Sunshine_Info.syswork_time;
	
	/* store the data to flash memory */
	Flash_Erase(FLASH_WORK_PARAM_ADDR, FLASH_WORK_PARAM_ADDR);
	status = Flash_Program(FLASH_WORK_PARAM_ADDR, (uint8_t *)paramPtr, sizeof(FLASH_PARA_STR));

	/* free memory */
	vPortFree(paramPtr);
	
	/* return result */
	return status;
}

/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/
