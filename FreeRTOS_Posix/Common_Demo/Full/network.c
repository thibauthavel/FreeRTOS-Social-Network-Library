/*
 * FreeRTOS Social Network Library
 * https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *	
 * Author: Thibaut HAVEL
 * Date:   08/02/2012
 *
 * This program use the OAuth library in order to get 'Tweets'
 * from a Twitter account (thibaut_havel).
 *
 */


/*-----------------------------------------------------------*/


//  SOCIAL NETWORK      Twitter
//  ACCESS PROTOCOL     OAuth
//  ACCESS LEVEL        Read-only
//  ---------------------------------------------------------
//  Request token URL   https://api.twitter.com/oauth/request_token
//  Authorize URL       https://api.twitter.com/oauth/authorize
//  Access token URL    https://api.twitter.com/oauth/access_token
//  ---------------------------------------------------------
//  consumer_key        VB5FifD1HLhmLmsj8tZA
//  consumer_secret     OdE18zFiva5TsC3rPQmq9BlYXhfBPFWJq2bY6Ib40
//  oauth_token         488884688-gBSVVjk722PjHgPx5mBPY1wTsL0Bmlv8QAazwgMy
//  oauth_token_secret  6jLaHA3wLRiUGAidt9q4doIx3IXX8U1D6ntyUuVMN1g


/*-----------------------------------------------------------*/


/* System header */
#include <stdio.h> // Remove after test done.
#include <stdlib.h>
#include <strings.h>

/* FreeRTOS header */
#include "FreeRTOS.h"
#include "task.h"
#include "print.h"

/* Program header */
#include "network.h"

/* Social network library header */
#include "NetworkLib/oauth.h"

/* Defines */
#define netSTACK_SIZE      ((unsigned short)512)
#define netNUMBER_OF_TASKS (1)

/* Task definition */
static void mainTask(void *pvParameters);

/* Check tasks running */
static volatile unsigned short taskCheck[netNUMBER_OF_TASKS] = { (unsigned short)0 };

/* Twitter account */
char *consumer_key       = "VB5FifD1HLhmLmsj8tZA";                               // Consumer key
char *consumer_secret    = "OdE18zFiva5TsC3rPQmq9BlYXhfBPFWJq2bY6Ib40";          // Consumer secret
char *oauth_token        = "488884688-gBSVVjk722PjHgPx5mBPY1wTsL0Bmlv8QAazwgMy"; // Token key
char *oauth_token_secret = "6jLaHA3wLRiUGAidt9q4doIx3IXX8U1D6ntyUuVMN1g";  


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
            twitterizer(); 
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
