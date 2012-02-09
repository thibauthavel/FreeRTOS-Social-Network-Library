/*
 *	FreeRTOS Social Network Library
 *	https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *	
 *	Author: Thibaut HAVEL
 *	Date:   08/02/2012
 *
 *  First library test.
 *
 */


/*-----------------------------------------------------------*/


/* System headers */
#include <stdio.h>
#include <string.h>

/* FreeRTOS headers */
#include "FreeRTOS.h"
#include "task.h"

/* Library header */
#include "libtest.h"


/*-----------------------------------------------------------*/


void sayHelloTo (const char * const someone)
{
	vTaskSuspendAll();
	{
		printf("Hello, %s !\r\n", someone);
		fflush(stdout);
	}
	xTaskResumeAll();
}

