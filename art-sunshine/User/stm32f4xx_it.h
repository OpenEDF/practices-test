/**
  ******************************************************************************
  * @file    stm32f4xx_it.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    19-August-2020
  * @brief   This file contains  all  the  functions  prototypes for the system   
  *          interrupt handler.
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
#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void USART1_IRQHandler(void);
void USART3_IRQHandler(void);
void UART4_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);
void TIM8_CC_IRQHandler(void);
void RTC_Alarm_IRQHandler(void);
void RTC_WKUP_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void PVD_IRQHandler(void);
void TIM2_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */

/***************************** END OF FILE *************************************/
