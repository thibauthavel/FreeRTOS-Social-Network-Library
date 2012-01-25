#include <stdlib.h>
#include <math.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "print.h"

/* Demo program include files. */
#include "pong.h"

void vStartPongTask()
{
    for( ;; )
	{
	    /* Print from the FreeRTOS API */
	    vPrintDisplayMessage("Pong !");
	    
	    /* Allows multitasking */
	    taskYIELD();
	}
}

