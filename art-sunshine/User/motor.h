/**
  ******************************************************************************
  * @file    motor.h
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    16-June-2020
  * @brief   This file contains all the functions prototypes for sytem status  
  *			 led configure and operation.
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
	POINTER_C_MOTOR,
	POINTER_D_MOTOR,
	POINTER_MAX_MOTOR,
}motor_type_m;

/* motor direction */
typedef enum {
	MOTOR_DIR_POSITIVER = 0x00,
	MOTOR_DIR_OPPOSITE,
}motor_dircetion_m;

/* motor control state */
typedef enum {
	MOTOR_STATE_START = 0x00,			/* motor start */
	MOTOR_STATE_STOP,					/* motor Stop */
}motor_state_m;

/* motor control structer */
typedef struct {
	motor_type_m motor_local;			/* motor localtion */
	motor_state_m motor_state;			/* motor operaton status */
	motor_dircetion_m motor_direction;	/* motor direction */
	uint32_t motor_pluse_count;         /* pluse counter */
	uint32_t motor_pwm_period;          /* PWM period */
	uint16_t motor_pwm_pluse;           /* PWM pluse */
	void (*motor_gpio_init)(motor_type_m motor);   /* init the gpio pin for the motor */
    void (*motor_timer_init)(motor_type_m motor, uint32_t period, uint16_t pluse_count);    /* init the timer */
	void (*motor_work_state)(motor_type_m motor, motor_state_m state);                      /* set teh motor state */
    void (*motor_roration_direction)(motor_type_m motor, motor_dircetion_m dirction);       /* set the motor direction */
}motor_operation_t;
/* Exported constants --------------------------------------------------------*/
extern motor_operation_t motor_opr[POINTER_MAX_MOTOR];

/* Exported macro ------------------------------------------------------------*/

/* 
 * macro: @system MOTOR GPIO define.
 */
#define MOTOR_A_DIR_PORT    GPIOD				/* motor A ENA and PUL GPIO Pin */
#define MOTOR_A_DIR_PIN     GPIO_Pin_3
#define MOTOR_A_DIR_CLOCK	RCC_AHB1Periph_GPIOD

#define MOTOR_A_ENA_PORT    GPIOD
#define MOTOR_A_ENA_PIN     GPIO_Pin_7
#define MOTOR_A_ENA_CLOCK	RCC_AHB1Periph_GPIOD

#define MOTOR_B_DIR_PORT    GPIOD				/* motor B ENA and PUL GPIO Pin */
#define MOTOR_B_DIR_PIN     GPIO_Pin_11
#define MOTOR_B_DIR_CLOCK	RCC_AHB1Periph_GPIOD

#define MOTOR_B_ENA_PORT    GPIOF
#define MOTOR_B_ENA_PIN     GPIO_Pin_11
#define MOTOR_B_ENA_CLOCK	RCC_AHB1Periph_GPIOF

#define MOTOR_C_DIR_PORT    GPIOF				/* motor C ENA and PUL GPIO Pin */
#define MOTOR_C_DIR_PIN     GPIO_Pin_1
#define MOTOR_C_DIR_CLOCK	RCC_AHB1Periph_GPIOF

#define MOTOR_C_ENA_PORT    GPIOF
#define MOTOR_C_ENA_PIN     GPIO_Pin_2
#define MOTOR_C_ENA_CLOCK	RCC_AHB1Periph_GPIOF

#define MOTOR_D_DIR_PORT    GPIOC				/* motor D ENA and PUL GPIO Pin */
#define MOTOR_D_DIR_PIN     GPIO_Pin_8
#define MOTOR_D_DIR_CLOCK	RCC_AHB1Periph_GPIOC

#define MOTOR_D_ENA_PORT    GPIOH
#define MOTOR_D_ENA_PIN     GPIO_Pin_7
#define MOTOR_D_ENA_CLOCK	RCC_AHB1Periph_GPIOH

/* motor control parameters */
#define MOTOR_PLUSE_DEFAULT_REV		1			/* pluse rev table */
#define MOTOR_PLUSE_800_REV			4				
#define MOTOR_PLUSE_1600_REV		8
#define MOTOR_PLUSE_3200_REV		16
#define MOTOR_PLUSE_6400_REV		32
#define MOTOR_PLUSE_12800_REV		64
#define MOTOR_PLUSE_25600_REV		128
#define MOTOR_PLUSE_51200_REV		256
#define MOTOR_REDUCTION_RATION		20			/* motor reduction ratio */ 
/* WARNING:
 * counter = (new_angle - old_angle) / 1.8
 * pluse counter = counter * MOTOR_PLUSE_X_REV *  MOTOR_REDUCTION_RATION
 */

/* Exported functions --------------------------------------------------------*/
void system_motor_init(uint16_t motor_pwm_period, uint16_t motor_pwm_pluse);
void motor_control_start(motor_operation_t *motor_t, motor_dircetion_m dir);
void motor_control_stop(motor_operation_t *motor_t);
void set_pluse_count(motor_operation_t *motor_t, uint32_t pluse_count);
uint32_t get_pluse_count(motor_operation_t *motor_t);
motor_state_m get_motor_state(motor_operation_t *motor_t);
motor_dircetion_m get_motor_direction(motor_operation_t *motor_t);
void motor_work_by_pluse_count(motor_operation_t *motor_t, uint32_t pluse_count, motor_dircetion_m dir);
void test_motor(void);
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
