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
static void producerTask (void *pvParameters);
static void consumerTask  (void *pvParameters);

/* Check tasks running */
static volatile unsigned short taskCheck[twitNUMBER_OF_TASKS] = { (short) 0, (short) 0 };


/*-----------------------------------------------------------*/


void startTwitterTask (unsigned portBASE_TYPE uxPriority)
{
    twitterQueue * tQ1;
    twitterQueue * tQ2;

    tQ1                 = (twitterQueue *)pvPortMalloc(sizeof(twitterQueue));
    tQ1->queue          = xQueueCreate(10, (unsigned portBASE_TYPE)sizeof(tweetSet));
    tQ1->blockTime      = (portTickType)0;
    tQ1->checkVariable  = &(taskCheck[0]);

    tQ2                 = (twitterQueue *)pvPortMalloc(sizeof(twitterQueue));
    tQ2->queue          = tQ1->queue;
    tQ2->blockTime      = (portTickType)0;
    tQ2->checkVariable  = &(taskCheck[1]);

    xTaskCreate(producerTask, "TwitterProducer", twitSTACK_SIZE, (void *)tQ1, uxPriority, NULL);
    xTaskCreate(consumerTask, "TwitterConsumer", twitSTACK_SIZE, (void *)tQ2, uxPriority, NULL);
}


/*-----------------------------------------------------------*/


static void producerTask (void *pvParameters)
{
    twitterQueue *  tQ  = (twitterQueue *) pvParameters;

    twitterAuthEntity auth;
    int isAuthenticated = 0;

    const char * const taskStartMsg = "[TWITTER] Producer task started.\r\n";
    vPrintDisplayMessage(&taskStartMsg);

    /* Keep performing the task */
    for(;;)
    {
        if(isAuthenticated)
        {
            tweetSet ts = twitter_receive_tweets(auth);

            if(ts.count != 0)
            {
                if(xQueueSend(tQ->queue, (void *)&ts, tQ->blockTime) == pdPASS)
                {
                    (*tQ->checkVariable)++;
                }
            }
            else
            {
                /* No tweet, it's weird: check the authentication */ 
                isAuthenticated = 0;
            }
        }
        else
        {
            auth = twitter_authentication(CONSUMER_KEY, CONSUMER_SECRET, USER_SCREEN_NAME, USER_PASSWORD);

            if(strlen(auth.access_key) != 0  &&  strlen(auth.access_secret) != 0)
            {
                vDisplayMessage("[TWITTER] Authentication succeed.\n");
                isAuthenticated = 1;
            }
            else
            {
                vDisplayMessage("[TWITTER] Authentication failed.\n");
            }
        }

        vTaskDelay(2000);
    }
}


/*-----------------------------------------------------------*/


static void consumerTask  (void *pvParameters)
{
    twitterQueue * tQ  = (twitterQueue *) pvParameters;
    tweetSet ts;
    char * readId[100];
    int iRead = 0;
    char * msg;
    int i;

    const char * const taskStartMsg = "[TWITTER] Consumer task started.\r\n";
    vPrintDisplayMessage(&taskStartMsg);

    /* Keep performing the task */
    for(;;)
    {
        if(xQueueReceive(tQ->queue, &ts, tQ->blockTime) == pdPASS)
        {
            for(i = 0 ; i < ts.count ; i++)
            {
                if(!arrayContains(readId, iRead, ts.tweets[i].tweet_id))
                {
                    readId[iRead] = ts.tweets[i].tweet_id;
                    iRead++;

                    msg = xstrdup("[TWITTER] Tweet received: ");
                    msg = xstrcat(msg, ts.tweets[i].tweet_text);
                    msg = xstrcat(msg, "\n");

                    vDisplayMessage(msg);
                }
            }

            (*tQ->checkVariable)++;
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


/*------------------------------------------------------------*/

int arrayContains (char * array[], int count, char * element)
{
    int i;
    for(i = 0 ; i < count ; i++)
    {
        if(strcmp(array[i], element) == 0)
            return 1;
    }

    return 0;
}
