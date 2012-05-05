
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
#include "twitter.h"

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

/* Queue definition */
typedef struct TWITTER_QUEUE
{
    xQueueHandle     queue;
    portTickType     blockTime;
    volatile short * checkVariable;
} twitterQueue;

/* Task definition */
static void clientTask (void *pvParameters);
static void eventTask  (void *pvParameters);

/* Check tasks running */
static volatile unsigned short taskCheck[twitNUMBER_OF_TASKS] = { (short) 0, (short) 0 };


/*-----------------------------------------------------------*/


void startTwitterTask (unsigned portBASE_TYPE uxPriority)
{
    twitterQueue * tQ1;
    twitterQueue * tQ2;

    tQ1                 = (twitterQueue *)pvPortMalloc(sizeof(twitterQueue));
    tQ1->queue          = xQueueCreate(10, (unsigned portBASE_TYPE)sizeof(unsigned short));
    tQ1->blockTime      = (portTickType)0;
    tQ1->checkVariable  = &(taskCheck[0]);

    tQ2                 = (twitterQueue *)pvPortMalloc(sizeof(twitterQueue));
    tQ2->queue          = tQ1->queue;
    tQ2->blockTime      = (portTickType)0;
    tQ2->checkVariable  = &(taskCheck[1]);

    xTaskCreate(clientTask, "Client", twitSTACK_SIZE, (void *)tQ1, uxPriority, NULL);
    xTaskCreate(eventTask,  "Event",  twitSTACK_SIZE, (void *)tQ2, uxPriority, NULL);
}


/*-----------------------------------------------------------*/


static void clientTask (void *pvParameters)
{
    unsigned short  usValue = 0;
    twitterQueue *  tQ      = (twitterQueue *) pvParameters;

    const char * const taskStartMsg = "Client task started.\r\n";
    vPrintDisplayMessage(&taskStartMsg);

    /* Keep performing the task */
    for(;;)
    {
        if(xQueueSendToBack(tQ->queue, (void *)&usValue, tQ->blockTime) != pdPASS)
        {
           printf("Client error!\n"); 
        }
        else
        {
            printf("Client send message!\n");
            (*tQ->checkVariable)++;
        }

        ++usValue;
        vTaskDelay(2000);
    }
}


/*-----------------------------------------------------------*/


static void eventTask  (void *pvParameters)
{
    unsigned short  usValue = 0;
    twitterQueue *  tQ      = (twitterQueue *) pvParameters;

    const char * const taskStartMsg = "Event task started.\r\n";
    vPrintDisplayMessage(&taskStartMsg);

    /* Keep performing the task */
    for(;;)
    {
        if(xQueueReceive(tQ->queue, &usValue, tQ->blockTime) == pdPASS)
        {
            printf("Event receive message!\n");

            (*tQ->checkVariable)++;
            ++usValue;
        }
    }
}


/*-----------------------------------------------------------*/


/* This is called to check that all the created tasks are still running. */
portBASE_TYPE xAreTwitTaskStillRunning (void)
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
