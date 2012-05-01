/*
 *	FreeRTOS Social Network Library
 *	https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *	
 *	Author: Thibaut HAVEL
 *	Date:   2012
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
#define USER_SCREEN_NAME   "thibaut_havel"
#define USER_PASSWORD      "74107410"
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
    const char * const doItMsg = "I did it!\r\n";
   
	vPrintDisplayMessage(&taskStartMsg);
	pusTaskCheckVariable = (unsigned short *) pvParameters;

	/* Keep performing the task */
	for(;;)
	{
	    if(doItOnce)
	    {
            vTaskSuspendAll();
            {
                char * request_token_url;
                char * oauth_token;
                char * oauth_token_secret;
                char * callback;
                char * direct_token_url;
                char * verifier;
                char * access_token_url;
                char * access_token;
                char * access_token_secret;
                char * access_token_user_name;
                char * access_token_user_id;
                char * tweet_url;
                char * tweet_param;
                
                twitterAuthEntity auth;


                printf("\n\nAuthentication:\n");
                auth = twitter_authentication(CONSUMER_KEY, CONSUMER_SECRET, USER_SCREEN_NAME, USER_PASSWORD);
		/*
		printf("user_id=%s\n", auth.user_id);
		printf("user_screen_name=%s\n", auth.user_screen_name);
		printf("consumer_key=%s\n", auth.consumer_key);
		printf("consumer_secret=%s\n", auth.consumer_secret);
		printf("access_key=%s\n", auth.access_key);
		printf("access_secret=%s\n", auth.access_secret);
		*/

                printf("\n\nReceive tweets:\n");
                
                // Step 1 : Get the user timeline
                printf("\n\nStep 1 --------------------------------\n\n");
                char * timeline_user;
                twitter_timeline_user(CONSUMER_KEY, CONSUMER_SECRET, access_token, access_token_secret, access_token_user_name, &timeline_user);
                printf("timeline_user : [XML content]\n", timeline_user);
                
                // Step 2 : Parse the timeline into tweets
                printf("\n\nStep 2 --------------------------------\n\n");
                int count_tweets = xml_parser_count(timeline_user, "text");
                char * tweets[count_tweets];
                xml_parser_getall(timeline_user, "text", tweets);
                int i;
                for(i = 0 ; i < sizeof(tweets)/sizeof(char*) ; i++)
                {
                    printf("Tweet (%d) : %s\n", i, tweets[i]);
                }
                
                // Step 3 : Get the send tweets URL
                printf("\n\nStep 3 --------------------------------\n\n");
                time_t time_tmp;
                time(&time_tmp);
                char * tweet = xstrdup("This tweet has been sent via the library, the "); 
                tweet = xstrcat(tweet, ctime(&time_tmp));
                twitter_tweet_url(tweet, CONSUMER_KEY, CONSUMER_SECRET, access_token, access_token_secret, &tweet_url, &tweet_param);
                printf("tweet_url : %s\n", tweet_url);
                printf("tweet_param : %s\n", tweet_param);
                
                // Step 4 : Send the tweet
                printf("\n\nStep 4 --------------------------------\n\n");
                char * post_result;
                //twitter_tweet(tweet_url, tweet_param, &post_result);
                //printf("post_result : %s\n", post_result);
                
                printf("\n\nEnd behaviours ************************\n\n");
                printf("\n\n+++++++++++++++++++++++++++++++++++++++\n\n");
                */
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
