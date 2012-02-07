#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "print.h"

/* Demo program include files. */
#include "pong.h"

/* Defines */
#define pongSTACK_SIZE      ((unsigned short)512)
#define pongNUMBER_OF_TASKS (2)

/* Task definition */
static void pingTask(void *pvParameters);
static void pongTask(void *pvParameters);

/* Check tasks running */
static volatile unsigned short taskCheck[pongNUMBER_OF_TASKS] = { (unsigned short)0 };


/*-----------------------------------------------------------*/


void vStartPongTask (unsigned portBASE_TYPE uxPriority)
{
	xTaskCreate(pingTask, "Ping", pongSTACK_SIZE, (void *)&(taskCheck[0]), uxPriority, NULL);
	xTaskCreate(pongTask, "Pong", pongSTACK_SIZE, (void *)&(taskCheck[1]), uxPriority, NULL);
}


/*-----------------------------------------------------------*/


static void pingTask (void *pvParameters)
{
    volatile unsigned short *pusTaskCheckVariable;
    
    const char * const pcTaskStartMsg = "Ping task started.\r\n";
    const char * const pcTaskPingMsg = "Ping !\r\n";
   

	/* Task has started */
	vPrintDisplayMessage(&pcTaskStartMsg);

	/* The variable this task increments to show it is still running is passed in as the parameter. */
	pusTaskCheckVariable = (unsigned short *) pvParameters;

	/* Keep performing a calculation and checking the result against a constant. */
	for(;;)
	{
        vPrintDisplayMessage(&pcTaskPingMsg);
		taskYIELD();
		(*pusTaskCheckVariable)++;
		taskYIELD();
	}
}

static void pongTask (void *pvParameters)
{
    volatile unsigned short *pusTaskCheckVariable;
    
    const char * const pcTaskStartMsg = "Pong task started.\r\n";
    const char * const pcTaskPongMsg = "Pong !\r\n";
   

	/* Task has started */
	vPrintDisplayMessage(&pcTaskStartMsg);

	/* The variable this task increments to show it is still running is passed in as the parameter. */
	pusTaskCheckVariable = (unsigned short *) pvParameters;

	/* Keep performing a calculation and checking the result against a constant. */
	for(;;)
	{
        vPrintDisplayMessage(&pcTaskPongMsg);
		taskYIELD();
		(*pusTaskCheckVariable)++;
		taskYIELD();
	}
}


/*-----------------------------------------------------------*/


/* This is called to check that all the created tasks are still running. */
portBASE_TYPE xArePongTaskStillRunning (void)
{
    /* Keep a history of the check variables so we know if they have been incremented since the last call. */
    static unsigned short lastTaskCheck[pongNUMBER_OF_TASKS] = { (unsigned short)0 };
    portBASE_TYPE xReturn = pdTRUE, xTask;

	/* Check the ping and ping tasks are still running by ensuring their check variables are still incrementing. */
	for(xTask = 0 ; xTask < pongNUMBER_OF_TASKS ; xTask++)
	{
		if(taskCheck[xTask] == lastTaskCheck[xTask])
		{
			/* The check has not incremented so an error exists. */
			xReturn = pdFALSE;
		}

		lastTaskCheck[xTask] = taskCheck[xTask];
	}

	return xReturn;
}
