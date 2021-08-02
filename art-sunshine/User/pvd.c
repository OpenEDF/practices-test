/**
  ******************************************************************************
  * @file    pvd.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    27-August-2020
  * @brief   This file provides all the operation and configure functions for 
  *          system PVD config.
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
#include "pvd.h" 

/** @addtogroup PVD_Driver
  * @{
  */
  
/** @defgroup PVD control
  * @brief PVD control driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
  
/** @defgroup Flash_Functions
  * @{
  */
  
/**
  * @function	IWDG_Config
  * @brief		Config the system pvd and enable.
  * @param[in]	pvdlevel: voltage threshold detected.
  *				@PVD_LEVEL_2_0 --- 2.0V
  *				@PVD_LEVEL_2_3 --- 2.3V
  *				.....
  * @retval     None.
  */  
void PVD_Config(uint32_t pvdlevel)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  	EXTI_InitTypeDef EXTI_InitStructure;
	
	/* Eanbele the RCC */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Config NVIC */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/* Config Exit */
	EXTI_ClearITPendingBit(EXTI_Line16);
 	EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);

	/* Set the voltage threshold */
	PWR_PVDLevelConfig(pvdlevel);

	/* Enable the PVD */
	PWR_PVDCmd(ENABLE);
}

/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/

