/**
  ******************************************************************************
  * @file    motor.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    16-June-2021
  * @brief   This file provides all the operation and configure functions for 
  *          system status led control.
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
#include "motor.h"
#include "stm32f4xx_gpio.h"
#include "timer.h"
#include "arm_math.h"
#include "led.h"

/** @addtogroup GYRO_Driver
  * @{
  */
  
/** @defgroup MOTOR control
  * @brief MOTOR control driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
motor_operation_t motor_opr[POINTER_MAX_MOTOR];

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void motor_gpio_init(motor_type_m motor);
static void motor_timer_init(motor_type_m motor, uint32_t period, uint16_t count);
static void motor_a_work_state(motor_state_m state);
static void motor_b_work_state(motor_state_m state);
static void motor_c_work_state(motor_state_m state);
static void motor_d_work_state(motor_state_m state);
static void motor_work_state(motor_type_m motor, motor_state_m state);
static void motor_a_work_state(motor_state_m state);
static void motor_b_work_state(motor_state_m state);
static void motor_c_work_state(motor_state_m state);
static void motor_d_work_state(motor_state_m state);
static void motor_roration_direction(motor_type_m motor, motor_dircetion_m direction);
static void motor_init(motor_operation_t *motor_t, motor_type_m motor_local, uint16_t motor_pwm_period, uint16_t motor_pwm_pluse);
static void motor_a_location_exti_config(void);
static void motor_b_location_exti_config(void);
static void motor_c_location_exti_config(void);
static void motor_d_location_exti_config(void);

/** @defgroup MOTOR_Functions
  * @{
  */
  
/**
  * @function   motor_a_gpio_init
  * @brief      Motor A ENA and DIR GPIO Configitation.
  * @param[in]  motor_t the motor will be init.
  * @retval     None.
  */ 
static void motor_gpio_init(motor_type_m motor)
{
	/* DIR Pin is GPIOD3, ENA Pin is GPIOD7  */
	GPIO_InitTypeDef GPIO_InitStructure;

	uint32_t dir_gpio_port_clock;
	GPIO_TypeDef * dir_gpio_port;
	uint16_t dir_gpio_pin;

	uint32_t eda_gpio_port_clock;
	GPIO_TypeDef * eda_gpio_port;
	uint16_t eda_gpio_pin;

	/* select port and configuration */
	switch (motor)
	{
		case POINTER_A_MOTOR:		/* motor a config */
			dir_gpio_port_clock = MOTOR_A_DIR_CLOCK;
			dir_gpio_port 		= MOTOR_A_DIR_PORT;
			dir_gpio_pin 		= MOTOR_A_DIR_PIN ;
			eda_gpio_port_clock = MOTOR_A_ENA_CLOCK;
	 		eda_gpio_port		= MOTOR_A_ENA_PORT;
			eda_gpio_pin		= MOTOR_A_ENA_PIN;
		break;

		case POINTER_B_MOTOR:		/* motor b config */
			dir_gpio_port_clock = MOTOR_B_DIR_CLOCK;
			dir_gpio_port 		= MOTOR_B_DIR_PORT;
			dir_gpio_pin 		= MOTOR_B_DIR_PIN ;
			eda_gpio_port_clock = MOTOR_B_ENA_CLOCK;
	 		eda_gpio_port		= MOTOR_B_ENA_PORT;
			eda_gpio_pin		= MOTOR_B_ENA_PIN;
		break;

		case POINTER_C_MOTOR:		/* motor c config */
			dir_gpio_port_clock = MOTOR_C_DIR_CLOCK;
			dir_gpio_port 		= MOTOR_C_DIR_PORT;
			dir_gpio_pin 		= MOTOR_C_DIR_PIN ;
			eda_gpio_port_clock = MOTOR_C_ENA_CLOCK;
	 		eda_gpio_port		= MOTOR_C_ENA_PORT;
			eda_gpio_pin		= MOTOR_C_ENA_PIN;
		break;

		case POINTER_D_MOTOR:		/* motor d config */
			dir_gpio_port_clock = MOTOR_D_DIR_CLOCK;
			dir_gpio_port 		= MOTOR_D_DIR_PORT;
			dir_gpio_pin 		= MOTOR_D_DIR_PIN ;
			eda_gpio_port_clock = MOTOR_D_ENA_CLOCK;
	 		eda_gpio_port		= MOTOR_D_ENA_PORT;
			eda_gpio_pin		= MOTOR_D_ENA_PIN;
		break;

		default: /* don't anything */
		break;
	}
		
	/* Enable Clock */
	RCC_AHB1PeriphClockCmd(dir_gpio_port_clock, ENABLE);
	RCC_AHB1PeriphClockCmd(eda_gpio_port_clock, ENABLE);

	/* Set the DIR Output LOW */
	/* Set the ENA Output LOW */
	GPIO_InitStructure.GPIO_Pin = dir_gpio_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(dir_gpio_port, &GPIO_InitStructure);

	/* Set the ENA Output LOW */
	GPIO_InitStructure.GPIO_Pin = eda_gpio_pin;
	GPIO_Init(eda_gpio_port, &GPIO_InitStructure);
}

/**
  * @function   motor_timer_init
  * @brief      Configitation and initalization the timer for motor A.
  * @param[in]  motor_t the motor will be init.
  * @param[in]  period: The PWM period.
  * @param[in]	pluse_count: The pwm pluse.
  * @retval     None.
  */ 
static void motor_timer_init(motor_type_m motor, uint32_t period, uint16_t pluse)
{
  	/* motor timer init */
	switch (motor)
	{
		case POINTER_A_MOTOR:		/* motor A timer */
			TIM8_Init_CHx_PWM_Output(TIM8_PWM_CH1, period, pluse);
		break;

		case POINTER_B_MOTOR:		/* motor B timer */
			TIM8_Init_CHx_PWM_Output(TIM8_PWM_CH2, period, pluse);
		break;

		case POINTER_C_MOTOR:		/* motor C timer */
			TIM8_Init_CHx_PWM_Output(TIM8_PWM_CH3, period, pluse);
		break;

		case POINTER_D_MOTOR:		/* motor D timer */
			TIM8_Init_CHx_PWM_Output(TIM8_PWM_CH4, period, pluse);
		break;

		default: /* don't anything */
		break;
	}
}

/**
  * @function   motor_a_work_state
  * @brief      Set the motor work state.
  * @param[in]  state: The motor work state start or stop.
  * @retval     None.
  */
static void motor_a_work_state(motor_state_m state)
{
	/* set up the motor stop or run */
	if (state == MOTOR_STATE_START)
	{
		/* Timer start and PWM output */
		GPIO_ResetBits(MOTOR_A_ENA_PORT,MOTOR_A_ENA_PIN);
		TIM_ClearFlag(MOTOR_CONTRL_TIMER, TIM_IT_CC1);
		TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC1, ENABLE);
		TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_1, ENABLE);
	}
	else
	{
		/* Timer stop and PWM stop output */
		TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC1, DISABLE);
		TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_1, DISABLE);
	}
}

/**
  * @function   motor_b_work_state
  * @brief      Set the motor work state.
  * @param[in]  state: The motor work state start or stop.
  * @retval     None.
  */
static void motor_b_work_state(motor_state_m state)
{
	if (state == MOTOR_STATE_START)
	{
		/* Timer start and PWM output */
		GPIO_ResetBits(MOTOR_B_ENA_PORT,MOTOR_B_ENA_PIN);
		TIM_ClearFlag(MOTOR_CONTRL_TIMER, TIM_IT_CC2);
		TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC2, ENABLE);
		TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_2, ENABLE);
	}
	else
	{
		/* Timer stop and PWM stop output */
		TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC2, DISABLE);
		TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_2, DISABLE);
	}
}

/**
  * @function   motor_c_work_state
  * @brief      Set the motor work state.
  * @param[in]  state: The motor work state start or stop.
  * @retval     None.
  */
static void motor_c_work_state(motor_state_m state)
{
	if (state == MOTOR_STATE_START)
	{
		/* Timer start and PWM output */
		GPIO_ResetBits(MOTOR_C_ENA_PORT,MOTOR_C_ENA_PIN);
		TIM_ClearFlag(MOTOR_CONTRL_TIMER, TIM_IT_CC3);
		TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC3, ENABLE);
		TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_3, ENABLE);
	}
	else
	{
		/* Timer stop and PWM stop output */
		TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC3, DISABLE);
		TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_3, DISABLE);
	}
}


/**
  * @function   motor_d_work_state
  * @brief      Set the motor work state.
  * @param[in]  state: The motor work state start or stop.
  * @retval     None.
  */
static void motor_d_work_state(motor_state_m state)
{
	if (state == MOTOR_STATE_START)
	{
		/* Timer start and PWM output */
		GPIO_ResetBits(MOTOR_D_ENA_PORT,MOTOR_D_ENA_PIN);
		TIM_ClearFlag(MOTOR_CONTRL_TIMER, TIM_IT_CC4);
		TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC4, ENABLE);
		TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_4, ENABLE);
	}
	else
	{
		/* Timer stop and PWM stop output */
		TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC4, DISABLE);
		TIM_CCxCmd(MOTOR_CONTRL_TIMER, TIM_Channel_4, DISABLE);
	}
}


/**
  * @function   motor_a_work_state
  * @brief      Set the motor work state asscording the local.
  * @param[in]  motor_t the motor will be set up.
  * @param[in]  state: The motor work state start or stop.
  * @retval     None.
  */
static void motor_work_state(motor_type_m motor, motor_state_m state)
{
    switch (motor)
    {
    	case POINTER_A_MOTOR:	/* motor A state */
			motor_a_work_state(state);
		break;

		case POINTER_B_MOTOR:	/* motor B state */
			motor_b_work_state(state);
		break;

		case POINTER_C_MOTOR:	/* motor C state */
			motor_c_work_state(state);
		break;

		case POINTER_D_MOTOR:	/* motor D state */
			motor_d_work_state(state);
		break;

		default: /* don't anything */
		break;
    }
}

/**
  * @function   motor_a_roration_direction
  * @brief      Set the motor rarotion dirction 
  * @param[in]  direction: The motor work dirction positive or opposite.
  * @retval     None.
  */
static void motor_a_roration_direction(motor_dircetion_m direction)
{
    if (direction == MOTOR_DIR_POSITIVER)
    {
        /* Motor positiver roration */
		GPIO_ResetBits(MOTOR_A_DIR_PORT,MOTOR_A_DIR_PIN);
    }
    else
    {
        /* Motor opposite roration */
		GPIO_SetBits(MOTOR_A_DIR_PORT,MOTOR_A_DIR_PIN);
    }
}

/**
  * @function   motor_a_roration_direction
  * @brief      Set the motor rarotion dirction 
  * @param[in]  direction: The motor work dirction positive or opposite.
  * @retval     None.
  */
static void motor_b_roration_direction(motor_dircetion_m direction)
{
    if (direction == MOTOR_DIR_POSITIVER)
    {
        /* Motor positiver roration */
		GPIO_ResetBits(MOTOR_B_DIR_PORT,MOTOR_B_DIR_PIN);
    }
    else
    {
        /* Motor opposite roration */
		GPIO_SetBits(MOTOR_B_DIR_PORT,MOTOR_B_DIR_PIN);
    }
}

/**
  * @function   motor_a_roration_direction
  * @brief      Set the motor rarotion dirction 
  * @param[in]  direction: The motor work dirction positive or opposite.
  * @retval     None.
  */
static void motor_c_roration_direction(motor_dircetion_m direction)
{
    if (direction == MOTOR_DIR_POSITIVER)
    {
        /* Motor positiver roration */
		GPIO_ResetBits(MOTOR_C_DIR_PORT,MOTOR_C_DIR_PIN);
    }
    else
    {
        /* Motor opposite roration */
		GPIO_SetBits(MOTOR_C_DIR_PORT,MOTOR_C_DIR_PIN);
    }
}

/**
  * @function   motor_a_roration_direction
  * @brief      Set the motor rarotion dirction 
  * @param[in]  direction: The motor work dirction positive or opposite.
  * @retval     None.
  */
static void motor_d_roration_direction(motor_dircetion_m direction)
{
    if (direction == MOTOR_DIR_POSITIVER)
    {
        /* Motor positiver roration */
		GPIO_ResetBits(MOTOR_D_DIR_PORT,MOTOR_D_DIR_PIN);
    }
    else
    {
        /* Motor opposite roration */
		GPIO_SetBits(MOTOR_D_DIR_PORT,MOTOR_D_DIR_PIN);
    }
}

/**
  * @function   motor_a_roration_direction
  * @brief      Set the motor rarotion dirction 
  * @param[in]  motor_t the motor will be set up.
  * @param[in]  direction: The motor work dirction positive or opposite.
  * @retval     None.
  */
static void motor_roration_direction(motor_type_m motor, motor_dircetion_m direction)
{
    switch (motor)
    {
    	case POINTER_A_MOTOR:	/* motor A dir */
			motor_a_roration_direction(direction);
		break;

		case POINTER_B_MOTOR:	/* motor B dir */
			motor_b_roration_direction(direction);
		break;

		case POINTER_C_MOTOR:	/* motor C dir */
			motor_c_roration_direction(direction);
		break;

		case POINTER_D_MOTOR:	/* motor D dir */
			motor_d_roration_direction(direction);
		break;

		default: /* don't anything */
		break;
    }
}

/**
  * @function   motor_init
  * @brief      Configitation and initalization the motor A.
  * @param[in][out]  motor_t: motor need to configuration.
  *	@param[in]  motor_local: special the motor localtion and type.
  * @param[in]	motor_pwm_period: The pwm period.
  * @param[in]	motor_pwm_pluse: The pwm pluse.
  * @retval     None.
  */ 
static void motor_init(motor_operation_t *motor_t, motor_type_m motor_local, uint16_t motor_pwm_period, uint16_t motor_pwm_pluse)
{
    /* Initalization base data */
	motor_t->motor_work = MOTOR_WORK_ERROR;
	motor_t->motor_local = motor_local;
    motor_t->motor_state = MOTOR_STATE_STOP;
    motor_t->motor_direction = MOTOR_DIR_POSITIVER;
    motor_t->motor_pluse_count = 0x00;
    motor_t->motor_pwm_pluse = motor_pwm_pluse;
    motor_t->motor_pwm_period = motor_pwm_period;
	motor_t->motor_pwm_total_pluse = 0x00;
	motor_t->motor_angle = 0.0f;
    
    /* Initzlization the special data */
    motor_t->motor_gpio_init = motor_gpio_init;
    motor_t->motor_timer_init = motor_timer_init;
    motor_t->motor_work_state = motor_work_state;
    motor_t->motor_roration_direction = motor_roration_direction; 

    /* set the default state */
    motor_t->motor_gpio_init(motor_local);
    motor_t->motor_timer_init(motor_local, motor_pwm_period, motor_pwm_pluse);
	motor_t->motor_work_state(motor_local, motor_t->motor_state);
    motor_t->motor_roration_direction(motor_local, motor_t->motor_direction);
}

/**
  * @function   motor_a_location_exti_config
  * @brief      configuration the wxit pin as motor a control wire.
  * @param[in]  None
  * @retval     None.
  */
static void motor_a_location_exti_config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	/* RCC Enable */
	RCC_AHB1PeriphClockCmd(MOTORA_INT_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* GPIO Config */
	Config_GPIO_IN(MOTORA_INT_GPIO_PORT, MOTORA_INT_GPIO_PIN);
	SYSCFG_EXTILineConfig(MOTORA_INT_EXTI_PORTSOURCE, MOTORA_INT_EXTI_PINSOURCE);

	/* EXIT Config */
	EXTI_InitStructure.EXTI_Line = MOTORA_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/**
  * @function   motor_b_location_exti_config
  * @brief      configuration the wxit pin as motor a control wire.
  * @param[in]  None
  * @retval     None.
  */
static void motor_b_location_exti_config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	/* RCC Enable */
	RCC_AHB1PeriphClockCmd(MOTORB_INT_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* GPIO Config */
	Config_GPIO_IN(MOTORB_INT_GPIO_PORT, MOTORB_INT_GPIO_PIN);
	SYSCFG_EXTILineConfig(MOTORB_INT_EXTI_PORTSOURCE, MOTORB_INT_EXTI_PINSOURCE);

	/* EXIT Config */
	EXTI_InitStructure.EXTI_Line = MOTORB_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/**
  * @function   motor_c_location_exti_config
  * @brief      configuration the wxit pin as motor a control wire.
  * @param[in]  None
  * @retval     None.
  */
static void motor_c_location_exti_config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	/* RCC Enable */
	RCC_AHB1PeriphClockCmd(MOTORC_INT_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* GPIO Config */
	Config_GPIO_IN(MOTORC_INT_GPIO_PORT, MOTORC_INT_GPIO_PIN);
	SYSCFG_EXTILineConfig(MOTORC_INT_EXTI_PORTSOURCE, MOTORC_INT_EXTI_PINSOURCE);

	/* EXIT Config */
	EXTI_InitStructure.EXTI_Line = MOTORC_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/**
  * @function   motor_d_location_exti_config
  * @brief      configuration the wxit pin as motor a control wire.
  * @param[in]  None
  * @retval     None.
  */
static void motor_d_location_exti_config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	/* RCC Enable */
	RCC_AHB1PeriphClockCmd(MOTORD_INT_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* GPIO Config */
	Config_GPIO_IN(MOTORD_INT_GPIO_PORT, MOTORD_INT_GPIO_PIN);
	SYSCFG_EXTILineConfig(MOTORD_INT_EXTI_PORTSOURCE, MOTORD_INT_EXTI_PINSOURCE);

	/* EXIT Config */
	EXTI_InitStructure.EXTI_Line = MOTORD_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/**
  * @function   system_motor_exti_config
  * @brief      configuration the all exit pin for motor control wire.
  * @param[in]  None
  * @retval     None.
  */
void system_motor_exti_config(void)
{
	/* Config and enable EXTI9_5_IRQn */
	NVIC_InitTypeDef NVIC_InitStructure;
	/* NVIC Config */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* config motor a b c d exit interrupt pin */
	motor_a_location_exti_config();
	motor_b_location_exti_config();
	motor_c_location_exti_config();
	motor_d_location_exti_config();
}

/**
  * @function   motor_control_start
  * @brief      Configitation the direction and after motor start.
  * @param[in][out]  motor_t: motor need to configuration.
  * @param[in]	dir: The motor work dirction positive or opposite.
  * @retval     None.
  */ 
void motor_control_start(motor_operation_t *motor_t, motor_dircetion_m dir)
{
    /* WARNING: must set state before set direction */
    if (motor_t->motor_direction != dir)
    {
        motor_t->motor_direction = dir;
        motor_t->motor_roration_direction(motor_t->motor_local, dir);
    }
    motor_t->motor_state = MOTOR_STATE_START;
    motor_t->motor_work_state(motor_t->motor_local, motor_t->motor_state);
}

/**
  * @function  motor_control_stop
  * @brief     stop the motor.
  * @param[in][out] motor_t: motor need to configuration.
  * @retval    None.
  */ 
void motor_control_stop(motor_operation_t *motor_t)
{
    motor_t->motor_state = MOTOR_STATE_STOP;
    motor_t->motor_work_state(motor_t->motor_local, motor_t->motor_state);
}

/**
  * @function  set_pluse_count
  * @brief     set the pluse counter for motor.
  * @param[in][out] motor_t: motor need to configuration.
  * @param[in] pluse_count: the pluse number.
  * @retval    None.
  */ 
void set_pluse_count(motor_operation_t *motor_t, uint32_t pluse_count)
{
    /* set the motor stop */
	if (motor_t->motor_state != MOTOR_STATE_STOP)
	{
		motor_control_stop(motor_t); 
	}
    motor_t->motor_pluse_count = pluse_count;
}

/**
  * @function  get_pluse_count
  * @brief     get the pluse counter from motor.
  * @param[in][out] motor_t: motor need to configuration.
  * @retval    the pluse counter.
  */ 
uint32_t get_pluse_count(motor_operation_t *motor_t)
{
    return motor_t->motor_pluse_count;
}

/**
  * @function  get_motor_state
  * @brief     get the work state from motor.
  * @param[in][out] motor_t: motor need to configuration.
  * @retval    the work state, stop or start.
  */
motor_state_m get_motor_state(motor_operation_t *motor_t)
{
    /* motor state */
    return motor_t->motor_state;
}

/**
  * @function  get_motor_direction
  * @brief     get the rarotion direction from motor.
  * @param[in][out] motor_t: motor need to configuration.
  * @retval    the rarotion direction, opposite or positive.
  */
motor_dircetion_m get_motor_direction(motor_operation_t *motor_t)
{
    /* motor direction */
    return motor_t->motor_direction;
}

/**
  * @function  get_motor_cur_poistion
  * @brief     get the current poistion for motor.
  * @param[in][out] motor_t: motor need to configuration.
  * @retval    the angle of motor current poistion.
  */
float32_t get_motor_cur_poistion(motor_operation_t *motor_t)
{
	/* get the angle */
	return (motor_t->motor_pwm_total_pluse / PER_DEGREE_PLUSES_NUMBER);
}


/**
  * @function  motor_work_by_pluse_count
  * @brief     accroding the pluse counter driver the motor work.
  * @param[in][out] motor_t: motor need to configuration.
  * @param[in] pluse_count: the work pluse number.
  * @param[in] dir: the motor work direction.
  * @retval    None.
  */
void motor_work_by_pluse_count(motor_operation_t *motor_t, uint32_t pluse_count, motor_dircetion_m dir)
{
    /* set the pluse count */
    set_pluse_count(motor_t, pluse_count);
    /* start the motor work */
    motor_control_start(motor_t, dir);
}

/**
  * @function  system_motor_init
  * @brief     initailxation the all motor on system platform.
  * @param[in] motor_pwm_period: the motor work pwm period.
  * @param[in] motor_pwm_pluse: the motor work pwm duty cycle.
  * @retval    None.
  */
void system_motor_init(uint16_t motor_pwm_period, uint16_t motor_pwm_pluse)
{
	motor_type_m index;
	motor_operation_t *motor_t = NULL;

	/* configuration motor exit control pin */
	system_motor_exti_config();

	for (index = POINTER_A_MOTOR; index < POINTER_MAX_MOTOR; index++)
	{
		motor_t = &motor_opr[index];
		motor_init(motor_t, index, motor_pwm_period, motor_pwm_pluse);
	}
}

/**
  * @function  control_motor_run
  * @brief     control motor run by the angle and direction.
  * @param[in] motor_t: the motor will be control.
  * @param[in] angle: Control the motor rotation to the angle.
  * @param[in] dir: motor rarotion direction.
  * @retval    None.
  */
void control_motor_run(motor_operation_t *motor_t, float32_t *angle, motor_dircetion_m dir)
{
	float32_t additi = PER_DEGREE_PLUSES_NUMBER;
	float32_t pluse_count;
	uint32_t temp_count;
	
	/* calculater the pluse counter */
	arm_mult_f32(angle, &additi, &pluse_count, 1);
	temp_count = (uint32_t)pluse_count;

	/* add the pluse value */
	motor_t->motor_pwm_total_pluse += temp_count;
	motor_work_by_pluse_count(motor_t, temp_count, dir);	
}

/**
  * @function  system_motor_self_checking
  * @brief     set the system all motor selt checking and find the zero position.
  * @param[in] None.
  * @retval    None.
  */
void system_motor_self_checking(void)
{
	motor_operation_t *motor_t = NULL;
	motor_type_m index;
	float32_t angle = 180.00f;

	/* calculater the pluse counter */
	for (index = POINTER_A_MOTOR; index < POINTER_MAX_MOTOR; index++)
	{
		motor_t = &motor_opr[index];
		control_motor_run(motor_t, &angle, MOTOR_DIR_POSITIVER);	
	}
}

/**
  * @function  motor_self_checking
  * @brief     set the signal motor selt checking and find the zero position.
  * @param[in] motor_operation_t: The motor will be run self checking.
  * @retval    None.
  */
void motor_self_checking(motor_operation_t *motor_t)
{
	float32_t angle = 180.00f;
	control_motor_run(motor_t, &angle, MOTOR_DIR_POSITIVER);	
}


/**
  * @function  system_motor_all_stop
  * @brief     set the motor stop all of sysytem.
  * @param[in] None.
  * @retval    None.
  */
void system_motor_all_stop(void)
{
	motor_operation_t *motor_t = NULL;
	motor_type_m index;

	/* stop and clear */
	for (index = POINTER_A_MOTOR; index < POINTER_MAX_MOTOR; index++)
	{
		motor_t = &motor_opr[index];
		motor_control_stop(motor_t);	
	}
}

/**
  * @function  get_motor_check_state
  * @brief     get the system motor status.
  * @param[in] None.
  * @retval    motor_work_m: motor work state.
  */
motor_work_m get_motor_check_state(motor_operation_t *motor_t)
{
	return (motor_t->motor_work);
}


/**
  * @function  get_system_motor_check_state
  * @brief     get the system all motor status.
  * @param[in] None.
  * @retval    motor_work_m: all of motor work state.
  */
uint8_t get_system_motor_check_state(void)
{
	uint8_t state = 0x00;
	motor_operation_t *motor_t = NULL;
	
	/* get the motor state */
	motor_t =  &motor_opr[POINTER_D_MOTOR];
	state |= (get_motor_check_state(motor_t) << 3);

	motor_t =  &motor_opr[POINTER_C_MOTOR];
	state |= (get_motor_check_state(motor_t) << 2);

	motor_t =  &motor_opr[POINTER_B_MOTOR];
	state |= (get_motor_check_state(motor_t) << 1);

	motor_t =  &motor_opr[POINTER_A_MOTOR];
	state |= get_motor_check_state(motor_t);
	
	return state;
}


/**
  * @function  motor_test
  * @brief     motor basic test.
  * @param[in] None.
  * @retval    None.
  */
void motor_test(void)
{
	motor_operation_t *motor_t = NULL;
	motor_type_m index;
	float32_t angle = 1.82f;

	/* calculater the pluse counter */
	for (index = POINTER_A_MOTOR; index < POINTER_MAX_MOTOR; index++)
	{
		motor_t = &motor_opr[index];
		control_motor_run(motor_t, &angle, MOTOR_DIR_POSITIVER);	
	}
}

/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/
