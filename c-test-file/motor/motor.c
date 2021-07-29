/* Includes ------------------------------------------------------------------*/
#include "motor.h"
#include "stm32f4xx_gpio.h"
#include "timer.h"

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
static void motor_a_gpio_init(void);
static void motor_a_timer_init(uint32_t period, uint16_t count);
static void motor_a_work_state(motor_state_m state);
static void motor_a_roration_direction(motor_dircetion_m direction);

/** @defgroup MOTOR_Functions
  * @{
  */
  
/**
  * @function   motor_a_gpio_init
  * @brief      Motor A ENA and DIR GPIO Configitation.
  * @param[in]  None.
  * @retval     None.
  */ 
static void motor_a_gpio_init(void)
{
	/* DIR Pin is GPIOD3, ENA Pin is GPIOD7  */
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable Clock */
	RCC_AHB1PeriphClockCmd(MOTOR_A_ENA_CLOCK, ENABLE);

	/* Set the GPIOD3 as DIR Output LOW */
	/* Set the GPIOD7 as ENA Output LOW */
	GPIO_InitStructure.GPIO_Pin = MOTOR_A_DIR_PIN | MOTOR_A_ENA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MOTOR_A_DIR_PORT, &GPIO_InitStructure);
}

/**
  * @function   motor_a_timer_init
  * @brief      Configitation and initalization the timer for motor A.
  * @param[in]  period: The PWM period.
  * @param[in]	pluse_count: The pwm pluse.
  * @retval     None.
  */ 
static void motor_a_timer_init(uint32_t period, uint16_t pluse)
{
  	/* motor timer init */
	TIM8_Init_PWM_Output(period, pluse);
}

/**
  * @function   motor_a_work_state
  * @brief      Set the motor work state.
  * @param[in]  state: The motor work state start or stop.
  * @retval     None.
  */
static void motor_a_work_state(motor_state_m state)
{
    if (state == MOTOR_STATE_START)
    {
		/* Timer start and PWM output */
		GPIO_ResetBits(MOTOR_A_ENA_PORT,MOTOR_A_ENA_PIN);
		TIM_ClearFlag(MOTOR_CONTRL_TIMER, TIM_FLAG_CC1);
		TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC1, ENABLE);
		TIM_Cmd(MOTOR_CONTRL_TIMER, ENABLE); 
		TIM_CtrlPWMOutputs(MOTOR_CONTRL_TIMER, ENABLE);
    }
    else
    {
		/* Timer stop and PWM stop output */
		TIM_CtrlPWMOutputs(MOTOR_CONTRL_TIMER, DISABLE);
		TIM_Cmd(MOTOR_CONTRL_TIMER, DISABLE); 
		TIM_ClearFlag(MOTOR_CONTRL_TIMER, TIM_FLAG_CC1);
		TIM_ITConfig(MOTOR_CONTRL_TIMER, TIM_IT_CC1, DISABLE);
		GPIO_SetBits(MOTOR_A_ENA_PORT, MOTOR_A_ENA_PIN);
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
  * @function   motor_init
  * @brief      Configitation and initalization the motor A.
  * @param[in][out]  motor_t: motor need to configuration.
  *	@param[in]  motor_local: special the motor localtion and type.
  * @param[in]	motor_pwm_period: The pwm period.
  * @param[in]	motor_pwm_pluse: The pwm pluse.
  * @retval     None.
  */ 
void motor_init(motor_operation_t *motor_t, motor_type_m motor_local, uint16_t motor_pwm_period, uint16_t motor_pwm_pluse)
{
    /* Initalization base data */
	motor_t->motor_local = motor_local;
    motor_t->motor_state = MOTOR_STATE_STOP;
    motor_t->motor_direction = MOTOR_DIR_POSITIVER;
    motor_t->motor_pluse_count = 0x00;
    motor_t->motor_pwm_pluse = motor_pwm_pluse;
    motor_t->motor_pwm_period = motor_pwm_period;
    
    /* Initzlization the special data */
    switch (motor_t->motor_local) {
        case POINTER_A_MOTOR:
            motor_t->motor_gpio_init = motor_a_gpio_init;
            motor_t->motor_timer_init = motor_a_timer_init;
            motor_t->motor_work_state = motor_a_work_state;
            motor_t->motor_roration_direction = motor_a_roration_direction; 
            break;

        default:    /* default value */
            break;
    }

    /* set the default state */
    motor_t->motor_gpio_init();
    motor_t->motor_timer_init(motor_pwm_period, motor_pwm_pluse);
    motor_t->motor_roration_direction(motor_t->motor_direction);
    motor_t->motor_work_state(motor_t->motor_state);
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
        motor_t->motor_roration_direction(dir);
    }
    motor_t->motor_state = MOTOR_STATE_START;
    motor_t->motor_work_state(motor_t->motor_state);
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
    motor_t->motor_work_state(motor_t->motor_state);
	GPIO_ResetBits(MOTOR_A_PWM_PORT,MOTOR_A_PWM_PIN);	
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

void test_motor(void)
{
	motor_operation_t *motor_t = &motor_opr[POINTER_A_MOTOR];
	motor_init(motor_t, POINTER_A_MOTOR, 800, 400);
	motor_work_by_pluse_count(motor_t, 1000, MOTOR_DIR_POSITIVER);
}


/**
  * @}
  */
  
/**
  * @}
  */

/***************************** END OF FILE *************************************/

