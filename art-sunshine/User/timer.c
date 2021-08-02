/**
  ******************************************************************************
  * @file    timer.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file provides all the operation and  configure functions for 
  *          system timer.
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
#include "timer.h"

/** @addtogroup Timer_Driver
  * @{
  */
  
/** @defgroup Timer control
  * @brief Timer control driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
volatile uint32_t timer_counter;		/* global timer nums */
volatile uint32_t seconds;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void TIM8_CHx_PWM_GPIO_Config(TIM8_PWM_CHx_m CHx);
static void TIM8_CHx_PWM_Config(TIM8_PWM_CHx_m CHx, uint16_t pwm_pluse);
static void TIM8_Configuration(uint16_t pwm_period);

/** @defgroup Timer_Functions
  * @{
  */

/**
  * @function   TIM8_CH1_PWM_GPIO_Config
  * @brief      Configurate the GPIOI5 as PWM output for TIM8-CH1.
  * @param[in]  CHx: pwm channel.
  * @retval     None.
  */ 
static void TIM8_CHx_PWM_GPIO_Config(TIM8_PWM_CHx_m CHx)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint32_t gpio_port_clock;
	GPIO_TypeDef * gpio_port;
	uint8_t gpio_sources;
	uint8_t gpio_af_func;
	uint16_t gpio_pin;

	/* select port and configuration */
	switch (CHx)
	{
		case TIM8_PWM_CH1:		/* motor a config */
			gpio_port_clock = MOTOR_A_PWM_CLOCK;
			gpio_port = MOTOR_A_PWM_PORT;
			gpio_sources = MOTOR_A_PWM_AF_SOURSE;
			gpio_af_func = MOTOR_A_PWM_AF_TIMER;
			gpio_pin = MOTOR_A_PWM_PIN;
		break;

		case TIM8_PWM_CH2:		/* motor b config */
			gpio_port_clock = MOTOR_B_PWM_CLOCK;
			gpio_port = MOTOR_B_PWM_PORT;
			gpio_sources = MOTOR_B_PWM_AF_SOURSE;
			gpio_af_func = MOTOR_B_PWM_AF_TIMER;
			gpio_pin = MOTOR_B_PWM_PIN;
		break;

		case TIM8_PWM_CH3:		/* motor c config */
			gpio_port_clock = MOTOR_C_PWM_CLOCK;
			gpio_port = MOTOR_C_PWM_PORT;
			gpio_sources = MOTOR_C_PWM_AF_SOURSE;
			gpio_af_func = MOTOR_C_PWM_AF_TIMER;
			gpio_pin = MOTOR_C_PWM_PIN;
		break;

		case TIM8_PWM_CH4:		/* motor d config */
			gpio_port_clock = MOTOR_D_PWM_CLOCK;
			gpio_port = MOTOR_D_PWM_PORT;
			gpio_sources = MOTOR_D_PWM_AF_SOURSE;
			gpio_af_func = MOTOR_D_PWM_AF_TIMER;
			gpio_pin = MOTOR_D_PWM_PIN;
		break;

		default:	/* don't anything */
		break;
				
	}
	/* Enable Clock */
	RCC_AHB1PeriphClockCmd(gpio_port_clock, ENABLE);

	/* Set AF GPIO */
	GPIO_PinAFConfig(gpio_port, gpio_sources, gpio_af_func);
	/* Set the GPIO as PWM Output */
	GPIO_InitStructure.GPIO_Pin = gpio_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(gpio_port, &GPIO_InitStructure);
}


/**
  * @function   TIM8_CHx_PWM_Config
  * @brief      Configurate and enable the pwm duty cycle for TIM8-CH1.
  * @param[in]  CHx: pwm channel.
  * @param[in]  pwm_pluse: pwm pluse counter.
  * @retval     None.
  */ 
static void TIM8_CHx_PWM_Config(TIM8_PWM_CHx_m CHx, uint16_t pwm_pluse)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* Configuration TIM8-CH1 as PWM Mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = pwm_pluse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	
	/* select channel and init */
	switch (CHx)
	{
		case TIM8_PWM_CH1:	/* channel 1 */
			/* Enable PWM function */
			TIM_OC1Init(MOTOR_CONTRL_TIMER, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(MOTOR_CONTRL_TIMER, TIM_OCPreload_Enable);
			/* disable the pwm output */
			TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_1, DISABLE);
		break;

		case TIM8_PWM_CH2:	/* channel 2 */
			/* Enable PWM function */
			TIM_OC2Init(MOTOR_CONTRL_TIMER, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(MOTOR_CONTRL_TIMER, TIM_OCPreload_Enable);
			/* disable the pwm output */
			TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_2, DISABLE);
		break;

		case TIM8_PWM_CH3:	/* channel 3 */
			/* Enable PWM function */
			TIM_OC3Init(MOTOR_CONTRL_TIMER, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(MOTOR_CONTRL_TIMER, TIM_OCPreload_Enable);
			/* disable the pwm output */
			TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_3, DISABLE);
		break;

		case TIM8_PWM_CH4:	/* channel 3 */
			/* Enable PWM function */
			TIM_OC4Init(MOTOR_CONTRL_TIMER, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(MOTOR_CONTRL_TIMER, TIM_OCPreload_Enable);
			/* disable the pwm output */
			TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_4, DISABLE);
		break;

		default:	/* don't anything */
		break;
	}
}

/**
  * @function   TIM6_Configuration
  * @brief      Configurate the time-base TIM6 timer general basic timer 1ms.
  * @param[in]  None.
  * @retval     None.
  */ 
void TIM6_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/* Configuration NVIC for timer Interrupt */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configuration timer Functions */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 100 -1; 
	TIM_TimeBaseStructure.TIM_Prescaler = 840 -1;
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
	TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);
	TIM_Cmd(BASIC_TIM, ENABLE); 
}

/**
  * @function   TIM2_Configuration
  * @brief      Configurate the time-base TIM2 timer general basic timer 1ms.
  * @param[in]  None.
  * @retval     None.
  */ 
void TIM2_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/* Configuration NVIC for timer Interrupt */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configuration timer Functions */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 10000 -1; 
	TIM_TimeBaseStructure.TIM_Prescaler = 8400 -1;
	TIM_TimeBaseInit(INDEPENT_TIMER, &TIM_TimeBaseStructure);
	TIM_ClearFlag(INDEPENT_TIMER, TIM_FLAG_Update);
	TIM_ITConfig(INDEPENT_TIMER, TIM_IT_Update, ENABLE);
	TIM_Cmd(INDEPENT_TIMER, ENABLE); 
}

/**
  * @function   TIM8_Configuration
  * @brief      Configurate the advanced-control TIM8 timer as PWM output.
  * @param[in]  pwm_period: the pwm period.
  * @param[in]  pwm_pluse: the pwm pluse.
  * @retval     None.
  */ 
static void TIM8_Configuration(uint16_t pwm_period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	/* Configuration NVIC for timer Interrupt */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configuration timer Functions */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = pwm_period - 1; 
	TIM_TimeBaseStructure.TIM_Prescaler = 210 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	
	/* Enable Timer */
	TIM_TimeBaseInit(MOTOR_CONTRL_TIMER, &TIM_TimeBaseStructure);
	
	/* Disable the interrupt */
	TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC1, DISABLE);
	TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC2, DISABLE);
	TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC3, DISABLE);
	TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC4, DISABLE);
	
	/* default close PWM output */
	TIM_Cmd(MOTOR_CONTRL_TIMER, ENABLE); 
	TIM_CtrlPWMOutputs(MOTOR_CONTRL_TIMER, ENABLE);
}

/**
  * @function   TIM8_Init_CHx_PWM_Output
  * @brief      Initalization the TIM8 timer channel 1 as PWM output.
  * @param[in]  CHx: pwm channel.
  * @param[in]  pwm_period: the pwm period.
  * @param[in]  pwm_pluse: the pwm duty cycle.
  * @retval     None.
  */ 
void TIM8_Init_CHx_PWM_Output(TIM8_PWM_CHx_m CHx, uint16_t pwm_period, uint16_t pwm_pluse)
{
	/* Configuration the PWM GPIO */
	TIM8_CHx_PWM_GPIO_Config(CHx);
	TIM8_Configuration(pwm_period);		/* reduce code couping */
	TIM8_CHx_PWM_Config(CHx, pwm_pluse);
}

/**
  * @function   Timer_Delayms
  * @brief      Set millisecond delay by timer.
  * @param[in]  None.
  * @retval     None.
  */ 
void Timer_Delayms(uint32_t millisecond)
{
	uint32_t currentms = timer_counter;
	while ((timer_counter - currentms) < millisecond)
	{
		__NOP();
	}
}

/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/
