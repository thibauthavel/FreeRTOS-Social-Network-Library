/* System header */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

/* FreeRTOS header */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "print.h"

/* Program header */
#include "twitterSend.h"

/* Social network library header */
#include "NetworkLib/libtwitter.h"

/* Defines */
#define twitSTACK_SIZE      ((unsigned short)512)
#define twitNUMBER_OF_TASKS (2)
#define VERBOSE             1

#define USER_SCREEN_NAME    "dev_socnetlib"
#define USER_PASSWORD       "secretpassword"
#define CONSUMER_KEY        "VB5FifD1HLhmLmsj8tZA"
#define CONSUMER_SECRET     "OdE18zFiva5TsC3rPQmq9BlYXhfBPFWJq2bY6Ib40"


static void mainTask(void *pvParameters);
static volatile unsigned short taskCheck[twitNUMBER_OF_TASKS] = { (short) 0 };


/*-----------------------------------------------------------*/


void startTwitterSendTask (unsigned portBASE_TYPE uxPriority)
{
    twitterAuthEntity auth = twitter_authentication(CONSUMER_KEY, CONSUMER_SECRET, USER_SCREEN_NAME, USER_PASSWORD);

    for(;;)
    {
        vDisplayMessage("Hello\n");
        vTaskDelay(2000);
    }
}


static void mainTask (void *pvParameters)
{

}


/*-----------------------------------------------------------*/


/* This is called to check that all the created tasks are still running. */
portBASE_TYPE xAreTwitSTaskStillRunning (void)
{
    /* Keep a history of the check variables so we know if they have been incremented since the last call. */
    static unsigned short lastTaskCheck[twitNUMBER_OF_TASKS] = { (short) 0, (short) 0 };
    portBASE_TYPE xReturn = pdTRUE, xTask;

    /* Check the tasks are still running by ensuring their check variables are still incrementing. */
    for(xTask = 0 ; xTask < twitNUMBER_OF_TASKS ; xTask++)
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
