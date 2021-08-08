/**
  ******************************************************************************
  * @file    main.c
  * @author  Macro <makermuyi@gmail.com>
  * @version V1.0.0
  * @date    18-August-2020
  * @brief   This file provides main functions for project.
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
#include "common.h"
#include "arm_math.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "timers.h"
#include "sun.h"
#include "led.h"
#include "user.h"
#include "sensor.h"
#include "iwdg.h"

/** @addtogroup ART-SUNSHINE_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Task heap stack size */
#define SUNSHINESTART_TASK_RAM		        128
#define SUNSHINECONTROL_TASK_RAM		    256
#define USARTSEND_TASK_RAM		    		256
#define SUNREADWINDSPPED_TASK_RAM			512
#define LTERECEIVER_TASK_RAM				512
#define USERAPPLICATION_TASK_RAM			512


/* Task Priority */
#define SUNSHINESTART_TASK_PRIORITY		    1
#define SUNSHINECONTROL_TASK_PRIORITY		6
#define USARTSEND_TASK_PRIORITY		    	2
#define SUNREADWINDSPPED_TASK_PRIORITY		4
#define LTERECEIVER_TASK_PRIORITY			5
#define USERAPPLICATION_TASK_PRIORITY	    3


/* Queue items numbers */
#define USART_QUEUE_SIZE					3

/* Private variables ---------------------------------------------------------*/
TaskHandle_t vStartTaskHandler, vControlTaskHandler, vUSARTSendTaskHandler, 
			 vReadWindSpeedtaskHandler, vLTEReceiverTaskHandler, vUserAppTaskHandler;

QueueHandle_t xSerialTxQueue;
SemaphoreHandle_t xSerial485Mutex;

/* Private function prototypes -----------------------------------------------*/
void SunshineStart_Task(void *pvParameters);

/* Private functions ---------------------------------------------------------*/

/**
  * @function main.
  * @brief  Main program.
  * @param  None.
  * @retval None.
  */
int main(void)
{
	/* system init */
	System_Init();
	//Wait_LTEStartUp(LTE_WAIT_SECOND);

	/* Config and Enable the Watchdog */
	IWDG_Config(IWDG_TIMEOUT_1S);

	/* create the system start task */
	xTaskCreate(SunshineStart_Task, "SunshineStart_Task", SUNSHINESTART_TASK_RAM, NULL, SUNSHINESTART_TASK_PRIORITY, &vStartTaskHandler);
	
 	/* Start the scheduler so the tasks start executing. */
	vTaskStartScheduler();

	/* Execution will only reach here if there was not enough FreeRTOS heap memory
       remaining for the idle task to be created. */
	for(;;);
}

/**
  * @function   SunshineStart_Task
  * @brief      Start Task is to cerate all task and delete self.
  * @param[in]  pvParameters: default.
  * @retval     None.
  */
void SunshineStart_Task(void *pvParameters)
{
	taskENTER_CRITICAL();    /* Enter the critical zone */
	/* Create the Queue */
	xSerialTxQueue = xQueueCreate(USART_QUEUE_SIZE, (BaseType_t)sizeof(uint8_t *));
	if (xSerialTxQueue == NULL)
	{
		PDEBUG("The xSerialTxQueue Create Failed.\n");
	}

	/* Create the Mutex */
	xSerial485Mutex = xSemaphoreCreateMutex();
	if (xSerial485Mutex == NULL)
	{
		PDEBUG("The xSerial485Mutex Create Failed.\n");
	}
	
	/* create the system main task to control console by calculater sunrise and sunset. */
	xTaskCreate(SunshineControl_Task, "SunshineControl_Task", SUNSHINECONTROL_TASK_RAM, NULL, SUNSHINECONTROL_TASK_PRIORITY, &vControlTaskHandler);
	xTaskCreate(USARTSend_Task, "USARTSend_Task", USARTSEND_TASK_RAM, NULL, USARTSEND_TASK_PRIORITY, &vUSARTSendTaskHandler);
	xTaskCreate(SunshineReadWind_Task, "SunshineReadWind_Task", SUNREADWINDSPPED_TASK_RAM, NULL, SUNREADWINDSPPED_TASK_PRIORITY, &vReadWindSpeedtaskHandler);
	xTaskCreate(LTEReceiver_Task, "LTEReceiver_Task", LTERECEIVER_TASK_RAM, NULL, LTERECEIVER_TASK_PRIORITY, &vLTEReceiverTaskHandler);
	xTaskCreate(UserApplication_Task, "UserApplication_Task", USERAPPLICATION_TASK_RAM, NULL, USERAPPLICATION_TASK_PRIORITY, &vUserAppTaskHandler);

	/* Deleted the SunshineStart_Task Task */
	vTaskDelete(vStartTaskHandler);
	PDEBUG("\rSunshineStart_Task is Delete.\n");
	
	taskEXIT_CRITICAL();    /* Exit the critical zone */
}

/**
  * @}
  */

/***************************** END OF FILE *************************************/
