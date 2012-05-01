/*
 *  FreeRTOS Social Network Library
 *  https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *  
 *  Author: Thibaut HAVEL
 *  Date:   2012
 *
 *  This program use the OAuth library in order to get 'Tweets'
 *  from a Twitter account (thibaut_havel).
 *
 */


/*-----------------------------------------------------------*/


/* System header */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

/* FreeRTOS header */
#include "FreeRTOS.h"
#include "task.h"
#include "print.h"

/* Program header */
#include "network.h"

/* Social network library header */
#include "NetworkLib/libtwitter.h"



/* Defines */
#define netSTACK_SIZE      ((unsigned short)512)
#define netNUMBER_OF_TASKS (1)
#define VERBOSE            1

#define USER_SCREEN_NAME   "dev_socnetlib"
#define USER_PASSWORD      "secretpassword"
#define CONSUMER_KEY       "VB5FifD1HLhmLmsj8tZA"
#define CONSUMER_SECRET    "OdE18zFiva5TsC3rPQmq9BlYXhfBPFWJq2bY6Ib40"



/* Task definition */
static void mainTask(void *pvParameters);

/* Check tasks running */
static volatile unsigned short taskCheck[netNUMBER_OF_TASKS] = { (unsigned short)0 };


/*-----------------------------------------------------------*/


void startNetworkTask (unsigned portBASE_TYPE uxPriority)
{
    xTaskCreate(mainTask, "Twitter", netSTACK_SIZE, (void *)&(taskCheck[0]), uxPriority, NULL);
}


/*-----------------------------------------------------------*/


static void mainTask (void *pvParameters)
{
    volatile unsigned short *pusTaskCheckVariable; 
    int doItOnce = 1;
    
    const char * const taskStartMsg = "Twitter task started.\r\n";
   
    vPrintDisplayMessage(&taskStartMsg);
    pusTaskCheckVariable = (unsigned short *) pvParameters;

    /* Keep performing the task */
    for(;;)
    {
        if(doItOnce)
        {
            vTaskSuspendAll();
            {
                printf("\n[TWITTER] Authentication...\n");
                twitterAuthEntity auth = twitter_authentication(CONSUMER_KEY, CONSUMER_SECRET, USER_SCREEN_NAME, USER_PASSWORD);
                if(VERBOSE)
                {
                    printf("user_id=%s\n", auth.user_id);
                    printf("user_screen_name=%s\n", auth.user_screen_name);
                    printf("consumer_key=%s\n", auth.consumer_key);
                    printf("consumer_secret=%s\n", auth.consumer_secret);
                    printf("access_key=%s\n", auth.access_key);
                    printf("access_secret=%s\n", auth.access_secret);
                }

                printf("[TWITTER] Receive tweets...\n");
                tweetEntity * tweets;
                int count = twitter_receive_tweets(auth, &tweets);
                if(VERBOSE)
                {
                    printf("count=%d\n", count);
                    int i;
                    for(i = 0 ; i < count ; i++)
                    {
                        printf("Tweet (%d) : %s\n", i, tweets[i].tweet_text);
                    }
                }

                printf("[TWITTER] Send a tweet...\n");
                tweetEntity tweet = twitter_send_tweet(auth, "This is another test.");
                if(VERBOSE)
                {
                    printf("tweet_id=%s\n", tweet.tweet_id);
                    printf("tweet_date=%s\n", tweet.tweet_date);
                    printf("user_screen_name=%s\n", tweet.user_screen_name);
                    printf("tweet_text=%s\n", tweet.tweet_text);
                }

                printf("\n");
                fflush(stdout);
            }
            xTaskResumeAll();

            doItOnce--;
        }
        
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
