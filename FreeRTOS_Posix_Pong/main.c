/* System headers */
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <unistd.h>

/* FreeRTOS headers */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "partest.h"

/* Demo tasks headers */
#include "pong.h"

/*-----------------------------------------------------------*/

int main( void )
{
    /* RTOS init */
	vParTestInitialise();
	vPrintInitialise();
	
	/* Starting tasks */
	vStartPongTask();
	
	/* Starting scheduler */
	vTaskStartScheduler();

    /* Should never pass here */
	return 1;
}
/*-----------------------------------------------------------*/
void vMainQueueSendPassed( void )
{
    /* Need to be defined */
}
/*-----------------------------------------------------------*/
void vApplicationIdleHook( void )
{
    /* Need to be defined */
}
/*-----------------------------------------------------------*/
