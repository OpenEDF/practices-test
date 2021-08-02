/**
  ******************************************************************************
  * @file    flash.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file contains all the functions prototypes  for  sytem  flash  
  *			 configure and read-write operation.
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
#ifndef __FLASH__H__
#define __FLASH__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_flash.h"
#include "common.h"
#include "user.h"
#include "rtc.h"
#include "sun.h"

/** @addtogroup Flash_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* 
 * struct: @ Flsh memory information struct.
 */
 typedef struct {
 	char MemoryId; 		/* flash memory ID */
	SYSTEM_INFORMATION SUNShine_Info;	/* system workparameters */
 }FLASH_PARA_STR;

/* Exported constants --------------------------------------------------------*/
	
/* Exported macro ------------------------------------------------------------*/
/* Base address of the Flash sectors */ 
#define FLASH_ADDR_START		((uint32_t)0x08000000)	/* Flash Start Address */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) 	/* Base address of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) 	/* Base address of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) 	/* Base address of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) 	/* Base address of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) 	/* Base address of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) 	/* Base address of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) 	/* Base address of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) 	/* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) 	/* Base address of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) 	/* Base address of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000)	/* Base address of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) 	/* Base address of Sector 11, 128 Kbytes */
#define FLASH_ADDR_END			((uint32_t)0x080FFFFF)	/* Flash End Address */

/* Flash area planning */
#define FLASH_APP_START_APP		((uint32_t)0x08000000)	/* APP code start addr 0K */
#define FLASH_WORK_PARAM_ADDR	((uint32_t)0x08010000)	/* Working parameters 64K */

/* Exported functions --------------------------------------------------------*/
FunStatus Flash_Erase(uint32_t start_addr, uint32_t end_addr);
FunStatus Flash_Program(uint32_t start_addr, uint8_t *data, uint16_t length);
FunStatus Flash_ReadByte(uint32_t addr, uint8_t *data);
FunStatus Flash_ReadWord(uint32_t addr, uint32_t *data);
static uint16_t Flash_GetSector(uint32_t addr);
FunStatus Flash_LoadWorkParam(void);
FunStatus Flash_SaveWorkParam(void);
//void Flash_SetRDP(void);
//void Flash_SetWEP(void);

#ifdef __cplusplus
}
#endif

#endif  /*__FLASH__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/
