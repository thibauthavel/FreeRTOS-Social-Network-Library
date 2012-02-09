/*
 * FreeRTOS Social Network Library
 * https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *	
 * Author: Thibaut HAVEL
 * Date:   08/02/2012
 *
 * Program using the first test library.
 *
 */


/*-----------------------------------------------------------*/


/* System header */
#include <stdlib.h>

/* FreeRTOS header */
#include "FreeRTOS.h"
#include "task.h"
#include "print.h"

/* Test library header */
#include "NetworkLib/libtest.h"

/* Test program header */
#include "network.h"

/* Defines */
#define netSTACK_SIZE      ((unsigned short)512)
#define netNUMBER_OF_TASKS (1)

/* Task definition */
static void mainTask(void *pvParameters);

/* Check tasks running */
static volatile unsigned short taskCheck[netNUMBER_OF_TASKS] = { (unsigned short)0 };


/*-----------------------------------------------------------*/


void startNetworkTask (unsigned portBASE_TYPE uxPriority)
{
	xTaskCreate(mainTask, "Networking", netSTACK_SIZE, (void *)&(taskCheck[0]), uxPriority, NULL);
}


/*-----------------------------------------------------------*/


static void mainTask (void *pvParameters)
{
    volatile unsigned short *pusTaskCheckVariable;
    
    const char * const taskStartMsg = "Networking task started.\r\n";
   

	/* Task has started */
	vPrintDisplayMessage(&taskStartMsg);

	/* The variable this task increments to show it is still running is passed in as the parameter. */
	pusTaskCheckVariable = (unsigned short *) pvParameters;

	/* Keep performing the task */
	for(;;)
	{
        sayHelloTo("from the 'Networking' task");
		taskYIELD();
		(*pusTaskCheckVariable)++;
		taskYIELD();
	}
}


/*-----------------------------------------------------------*/


/* This is called to check that all the created tasks are still running. */
portBASE_TYPE xAreNetTaskStillRunning (void)
{
    /* Keep a history of the check variables so we know if they have been incremented since the last call. */
    static unsigned short lastTaskCheck[netNUMBER_OF_TASKS] = { (unsigned short)0 };
    portBASE_TYPE xReturn = pdTRUE, xTask;

	/* Check the tasks are still running by ensuring their check variables are still incrementing. */
	for(xTask = 0 ; xTask < netNUMBER_OF_TASKS ; xTask++)
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
