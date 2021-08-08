/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    19-August-2020
  * @brief   This file contains all the functions system interrupt handler.
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
#include "stm32f4xx_it.h"
#include "uart.h"
#include "timer.h"
#include "rtc.h"
#include "user.h"
#include "lte.h"
#include "motor.h"


/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
		PDEBUG("\rsystem Enter the HardFault_Handler, Please Ckeck and restart.\n");
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
/*
void SVC_Handler(void)
{
}
*/
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */

void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*
void PendSV_Handler(void)
{
}
*/
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/*
void SysTick_Handler(void)
{
 // TimingDelay_Decrement();
}
*/
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @function	USART1_IRQHandler
  * @brief		This function handles USART1_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void USART1_IRQHandler(void)
{
	USARTx_IRQHandler(USART_DEBUG, USART1, DEBUGUSART_TIMEOUT);
}

/**
  * @function	USART3_IRQHandler
  * @brief		This function handles USART3_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void USART3_IRQHandler(void)
{
	USARTx_IRQHandler(USART_485, USART3, RS485USART_TIMEOUT);
}

/**
  * @function	UART4_IRQHandler
  * @brief		This function handles UART2_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void USART2_IRQHandler(void)
{
	USARTx_IRQHandler(USART_LCD, USART2, LCDUSART_TIMEOUT);
}

/**
  * @function	UART2_IRQHandler
  * @brief		This function handles UART4_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void UART4_IRQHandler(void)
{
	USARTx_IRQHandler(USART_LTE, UART4, LETUSART_TIMEOUT);
}


/**
  * @function	TIM6_DAC_IRQHandler
  * @brief		This function handles TIM6_DAC_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void TIM6_DAC_IRQHandler(void)
{
	/* check timer interrupt */
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		timer_counter++;
		if (timer_counter % 100 == 0)
		{
			TASK_LED_TOGGLE();
		}
		/* UART Receiver Data Timeout Check */
		TimerOneFrame_Data();
		/* According the system mode update the system LED Status. */
		System_Status_Show(Art_Sunshine_Info.mode);
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

/**
  * @function	TIM2_IRQHandler
  * @brief		This function handles TIM2_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void TIM2_IRQHandler(void)
{
	/* check timer interrupt */
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		seconds++;
		TASK_LED_TOGGLE();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

/**
  * @function	TIM8_CC_IRQHandler
  * @brief		This function handles TIM8_CC_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void TIM8_CC_IRQHandler(void)
{
	motor_operation_t *motor_t = NULL;

	motor_t = &motor_opr[POINTER_A_MOTOR];
	/* TIM8 is according the motor A */
	if (TIM_GetITStatus(MOTOR_CONTRL_TIMER, TIM_IT_CC1) != RESET)	/* TIM PWM CH1 */
	{
		if ((motor_t->motor_pluse_count > 0) && (motor_t->motor_state == MOTOR_STATE_START))
        {
            motor_t->motor_pluse_count--;	/* pwm count */
        }
        else
        {
			/* motor stop work */
            motor_t->motor_pluse_count = 0;
			motor_control_stop(motor_t);
        }
		/* Clear pending */
		TIM_ClearITPendingBit(MOTOR_CONTRL_TIMER, TIM_IT_CC1);
	}

	motor_t = &motor_opr[POINTER_B_MOTOR];
	/* TIM8 is according the motor B */
	if (TIM_GetITStatus(MOTOR_CONTRL_TIMER, TIM_IT_CC2) != RESET)	/* TIM PWM CH2 */
	{
		if ((motor_t->motor_pluse_count > 0) && (motor_t->motor_state == MOTOR_STATE_START))
            motor_t->motor_pluse_count--;
        else
        {
            motor_t->motor_pluse_count = 0;
			motor_control_stop(motor_t);
        }
		TIM_ClearITPendingBit(MOTOR_CONTRL_TIMER, TIM_IT_CC2);
	}

	motor_t = &motor_opr[POINTER_C_MOTOR];
	/* TIM8 is according the motor C */
	if (TIM_GetITStatus(MOTOR_CONTRL_TIMER, TIM_IT_CC3) != RESET)	/* TIM PWM CH3 */
	{
		if ((motor_t->motor_pluse_count > 0) && (motor_t->motor_state == MOTOR_STATE_START))
            motor_t->motor_pluse_count--;
        else
        {
            motor_t->motor_pluse_count = 0;
			motor_control_stop(motor_t);
        }
		TIM_ClearITPendingBit(MOTOR_CONTRL_TIMER, TIM_IT_CC3);
	}

	motor_t = &motor_opr[POINTER_D_MOTOR];
	/* TIM8 is according the motor D */
	if (TIM_GetITStatus(MOTOR_CONTRL_TIMER, TIM_IT_CC4) != RESET)	/* TIM PWM CH4 */
	{
		if ((motor_t->motor_pluse_count > 0) && (motor_t->motor_state == MOTOR_STATE_START))
            motor_t->motor_pluse_count--;
        else
        {
            motor_t->motor_pluse_count = 0;
			motor_control_stop(motor_t);
        }
		TIM_ClearITPendingBit(MOTOR_CONTRL_TIMER, TIM_IT_CC4);
	}
}

/**
  * @function	RTC_Alarm_IRQHandler
  * @brief		This function handles RTC_Alarm_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void RTC_Alarm_IRQHandler(void)
{
	/* check the alarm interrupt is occur */
	if (RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		PDEBUG("Alarm Interrupt Runing.\n");
		/* upgrade the alalrm flag */
		Alarm_Flag = 0xFF;															
	}
	EXTI_ClearITPendingBit(EXTI_Line17);
}

/**
  * @function	RTC_WKUP_IRQHandler
  * @brief		This function handles RTC_WKUP_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void RTC_WKUP_IRQHandler(void)
{
	/* check the alarm interrupt is occur */
	if (RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_WUT);
		Art_Sunshine_Info.syswork_time++;
		second_count++;
		/* update the flag */
		Second_Flag = 0xFF;
	}
	EXTI_ClearITPendingBit(EXTI_Line22);
}

/**
  * @function	EXTI0_IRQHandler
  * @brief		This function handles EXTI0_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void EXTI0_IRQHandler(void)
{
	/* check the interrupt */
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) 
	{
		PDEBUG("System Key EXTI0_IRQHandler runing...\n");
		/* Clear the TI */
		EXTI_ClearITPendingBit(EXTI_Line0); 
	}
}

/**
  * @function	EXTI9_5_IRQHandler
  * @brief		This function handles EXTI4_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void EXTI9_5_IRQHandler(void)
{
	motor_operation_t *motor = NULL;
	/* check the interrupt */

	motor = &motor_opr[POINTER_A_MOTOR];
	if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
	{
		PDEBUG("System Key EXTI5_IRQHandler runing...\n");
		/* stop and clear */
		motor->motor_pluse_count = 0;
		motor_control_stop(motor);
		motor->motor_pwm_total_pluse = 0;
		
		/* Clear the TI */
		EXTI_ClearITPendingBit(EXTI_Line5); 
	}

	motor = &motor_opr[POINTER_B_MOTOR];
	if(EXTI_GetITStatus(EXTI_Line6) != RESET) 
	{
		PDEBUG("System Key EXTI6_IRQHandler runing...\n");
		
		motor->motor_pluse_count = 0;
		motor_control_stop(motor);
		motor->motor_pwm_total_pluse = 0;
		
		/* Clear the TI */
		EXTI_ClearITPendingBit(EXTI_Line6); 
	}

	motor = &motor_opr[POINTER_C_MOTOR];
	if(EXTI_GetITStatus(EXTI_Line7) != RESET) 
	{
		PDEBUG("System Key EXTI7_IRQHandler runing...\n");
		
		motor->motor_pluse_count = 0;
		motor_control_stop(motor);
		motor->motor_pwm_total_pluse = 0;
		
		/* Clear the TI */
		EXTI_ClearITPendingBit(EXTI_Line7); 
	}

	motor = &motor_opr[POINTER_D_MOTOR];
	if(EXTI_GetITStatus(EXTI_Line8) != RESET) 
	{
		PDEBUG("System Key EXTI8_IRQHandler runing...\n");

		motor->motor_pluse_count = 0;
		motor_control_stop(motor);
		motor->motor_pwm_total_pluse = 0;
		
		/* Clear the TI */
		EXTI_ClearITPendingBit(EXTI_Line8); 
	}
}


/**
  * @function	PVD_IRQHandler
  * @brief		This function handles PVD_IRQHandler interrupt request.
  * @param[in]	None.
  * @retval 	None.
  */
void PVD_IRQHandler(void)
{
	/* check the PVDO */
	if (PWR_GetFlagStatus(PWR_FLAG_PVDO) != RESET)
	{
		PDEBUG("System Enter the PVD interrupt Handler.\n");
		PDEBUG("System will save parameters to flash and shutdown.\n");
		/* save the parameters */
		Flash_LoadWorkParam();
		PDEBUG("goodbye dear friend.\n");
	}
}

/**
  * @}
  */ 

/***************************** END OF FILE *************************************/
