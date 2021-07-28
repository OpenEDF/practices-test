/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR__H__
#define __MOTOR__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/** @addtogroup LED_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* motor type enumeration */
typedef enum {
	POINTER_A_MOTOR	= 0x00,		/* 1# A/B motor */
	POINTER_B_MOTOR,
	POINTER_MAX_MOTOR,
}motor_type_m;

/* motor direction */
typedef enum {
	MOTOR_DIR_POSITIVER = 0x00,		/* positive rarotion */
	MOTOR_DIR_OPPOSITE,				/* opposite rarotion */
}motor_dircetion_m;

/* motor control state */
typedef enum {
	MOTOR_STATE_START = 0x00,			/* motor start */
	MOTOR_STATE_STOP,					/* motor Stop */
}motor_state_m;

/* motor control structer */
typedef struct {
	motor_state_m motor_state;			/* motor operaton status */
	motor_dircetion_m motor_dirction;	/* motor direction */
	uint32_t motor_pluse_count;         /* pluse counter */
	uint32_t motor_pwm_period;          /* PWM period */
	uint16_t motor_pwm_pluse;           /* PWM pluse */
    void (*motor_timer_init)(uint32_t period, uint16_t pluse_count);    /* init the timer */
    void (*motor_gpio_init)(void);                                      /* init the gpio pin for the motor */
    void (*motor_roration_direction)(motor_dircetion_m dirction);       /* set the motor direction */
	void (*motor_work_state)(motor_state_m state);                      /* set teh motor state */
}motor_operation_t;
/* Exported constants --------------------------------------------------------*/
	
/* Exported macro ------------------------------------------------------------*/

/* 
 * macro: @system MOTOR GPIO define.
 */
#define MOTOR_A_DIR_PORT    GPIOD
#define MOTOR_A_DIR_PIN     GPIO_Pin_3

#define MOTOR_A_ENA_PORT    GPIOD
#define MOTOR_A_ENA_PIN     GPIO_Pin_7


/* Exported functions --------------------------------------------------------*/
void motor_init(motor_operation_t *motor_t, uint32_t motor_pwm_period, uint16_t motor_pwm_pluse);
void motor_control_start(motor_operation_t *motor_t, motor_dircetion_m dir);
void motor_control_stop(motor_operation_t *motor_t);
void set_pluse_count(motor_operation_t *motor_t, uint32_t pluse_count);
uint32_t get_pluse_count(motor_operation_t *motor_t);
motor_dircetion_m get_motor_state(motor_operation_t *motor_t);
motor_dircetion_m get_motor_direction(motor_operation_t *motor_t);
void motor_work_by_pluse_count(motor_operation_t *motor_t, uint32_t pluse_count, motor_dircetion_m dir);

#ifdef __cplusplus
}
#endif

#endif  /*__MOTOR__H__*/

/**
  * @}
  */

/**
  * @}
  */

/***************************** END OF FILE *************************************/

motor_operation_t motor_opr[POINTER_MAX_MOTOR] =&syste_global.xxx;   /* pointer point to system global variables */

void motor_init(motor_operation_t *motor_t, uint32_t motor_pwm_period, uint16_t motor_pwm_pluse)
{
    /* Initalization base data */
    motor_t->motor_state = MOTOR_STATE_STOP;
    motor_t->motor_dircetion = MOTOR_DIR_POSITIVER;
    motor_t->motor_pluse_count = 0x00;
    motor_t->motor_pwm_pluse = motor_pwm_pluse;
    motor_t->motor_pwm_period = motor_pwm_period;
    
    /* Initzlization the special data */
    switch (motor_locat) {
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
    motor_t->motor_roration_direction(motor_t->motor_dircetion);
    motor_t->motor_work_state(motor_t->motor_state);
}

void motor_control_start(motor_operation_t *motor_t, motor_dircetion_m dir)
{
    /* WARNING: must set state before set direction */
    if (motor_t->motor_dircetion != dir)
    {
        motor_t->motor_dircetion = dir;
        motor_t->motor_roration_direction(dir);
    }
    motor_t->motor_state = MOTOR_STATE_START;
    motor_t->motor_work_state(motor_t->motor_state);
}

void motor_control_stop(motor_operation_t *motor_t)
{
    motor_t->motor_state = MOTOR_STATE_STOP;
    motor_t->motor_work_state(motor_t->motor_state);
}

void set_pluse_count(motor_operation_t *motor_t, uint32_t pluse_count)
{
    /* set the motor stop */
    motor_control_stop(motor_locat); 
    motor_t->motor_pluse_count = pluse_count;
}

uint32_t get_pluse_count(motor_operation_t *motor_t)
{
    return motor_t->motor_pluse_count;
}

motor_dircetion_m get_motor_state(motor_operation_t *motor_t)
{
    /* motor state */
    return motor_t->motor_state;
}

motor_dircetion_m get_motor_direction(motor_operation_t *motor_t)
{
    /* motor direction */
    return motor_t->motor_dirction;
}

void motor_work_by_pluse_count(motor_operation_t *motor_t, uint32_t pluse_count, motor_dircetion_m dir)
{
    /* set the pluse count */
    set_pluse_count(motor_t, pluse_count);

    /* start the motor work */
    motor_control_start(motor_t, dir);

    /* when the pluse_count = 0, the motor stop for Hanlder function exceeded */
}

void timer_handler(void)
{
    motor_operation_t *motor_opr = &syste_global.motor_t[POINTER_A_MOTOR];

    if (internet is active) 
    {
        if ((motor_opr.motor_pluse_count != 0) && (motor_opr.motor_state == MOTOR_STATE_START))
        {
            motor_opr.motor_pluse_count--;
        }
        else if (motor_opr.motor_pluse_count == 0)
        {
            motor_control_stop(motor_opr);
        }
        else
        {
            motor_opr.motor_pluse_count = 0;
        }
    }
}

static void motor_a_timer_init(uint32_t period, uint16_t pluse_count)
{
    /* motor timer init */
}

static void motor_a_work_state(motor_state_m state)
{
    if (motor_state == MOTOR_START)
    {
        /* Timer start and PWM output */
    }
    else
    {
        /* Timer stop and PWM stop output */
    }
}

static void motor_a_roration_direction(motor_dircetion_m dirction)
{
    if (dirction == DIR_POSITIVER)
    {
        /* Motor positiver roration */
    }
    else
    {
        /* Motor opposite roration */
    }
