/*
 *	FreeRTOS Social Network Library
 *	https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *	
 *	Author: Thibaut HAVEL
 *	Date:   13/02/2012
 *
 */


/*-----------------------------------------------------------*/


/* System headers */
#include <stdio.h>
#include <string.h>

/* FreeRTOS headers */
#include "FreeRTOS.h"
#include "task.h"

/* Other requiered library */
#include "oauth.h"

/* Library header */
#include "libtwitter.h"



#define TWITTER_REQUEST_TOKEN_URL "https://api.twitter.com/oauth/request_token"
#define TWITTER_DIRECT_TOKEN_URL "https://api.twitter.com/oauth/authorize"
#define TWITTER_ACCESS_TOKEN_URL "https://api.twitter.com/oauth/access_token"


/*-----------------------------------------------------------*/


#define CONSUMER_KEY       "VB5FifD1HLhmLmsj8tZA"
#define CONSUMER_SECRET    "OdE18zFiva5TsC3rPQmq9BlYXhfBPFWJq2bY6Ib40"
#define OAUTH_TOKEN        "488884688-gBSVVjk722PjHgPx5mBPY1wTsL0Bmlv8QAazwgMy"
#define OAUTH_TOKEN_SECRET "6jLaHA3wLRiUGAidt9q4doIx3IXX8U1D6ntyUuVMN1g"


/*-----------------------------------------------------------*/


void twitterizer (void)
{
	vTaskSuspendAll();
	{
        char * request_token_url;
        char * oauth_token;
        char * oauth_token_secret;
        char * callback;
        char * verifier;
        char * direct_token_url;

        // Step 1 : Get the request token URL
        printf("\n\nStep 1 --------------------------------\n\n");
        twitter_request_token_url(CONSUMER_KEY, CONSUMER_SECRET, &request_token_url);
		printf("request_token_url : %s \n", request_token_url);
		
		// Step 2 : Use the request token URL to get a token
		printf("\n\nStep 2 --------------------------------\n\n");
		twitter_request_token(request_token_url, &oauth_token, &oauth_token_secret, &callback);
		printf("oauth_token        : %s \n", oauth_token);
		printf("oauth_token_secret : %s \n", oauth_token_secret);
		printf("callback           : %s \n", callback);
		
		// Step 3 : Get the direct token URL
		printf("\n\nStep 3 --------------------------------\n\n");
		twitter_direct_token_url(oauth_token, &direct_token_url);
		printf("direct_token_url : %s \n", direct_token_url);
		
		fflush(stdout);
	}
	xTaskResumeAll();
}


/*
 *  IN  : consumer_key
 *  IN  : consumer_secret
 *  OUT : request_token_url
 */
void twitter_request_token_url (const char * consumer_key, const char * consumer_secret, char ** request_token_url)
{
    // Useing OAuth to get the URL
	*request_token_url = oauth_sign_url2(TWITTER_REQUEST_TOKEN_URL, NULL, OA_HMAC, NULL, consumer_key, consumer_secret, NULL, NULL);
}


/*
 *  IN  : request_token_url
 *  OUT : oauth_token
 *  OUT : oauth_token_secret
 *  OUT : callback
 */
void twitter_request_token (const char * request_token_url, char ** oauth_token, char ** oauth_token_secret, char ** callback)
{
    char *  request_token_param;
    char *  request_token_result;
	char ** request_token_result_value;
    int     request_token_result_count;
    
    // Using OAuth to get the result
    request_token_result = oauth_http_get(request_token_url, request_token_param);
    
    // Split results into variables
    *oauth_token        = strtok(request_token_result, "&");
    *oauth_token_secret = strtok(NULL, "&");
    *callback           = strtok(NULL, "&");
    
    strtok(*oauth_token, "=");
    *oauth_token        = strtok(NULL, "=");
    strtok(*oauth_token_secret, "=");
    *oauth_token_secret = strtok(NULL, "=");
    strtok(*callback, "=");
    *callback           = strtok(NULL, "=");
}


/*
 *  IN  : consumer_key
 *  IN  : consumer_secret
 *  OUT : request_token_url
 */
void twitter_direct_token_url (const char * oauth_token, char ** direct_token_url)
{
	strcpy(*direct_token_url, TWITTER_DIRECT_TOKEN_URL);
	strcat(*direct_token_url, "?oauth_token=");
	strcat(*direct_token_url, oauth_token);
}



/*
 *  IN  : oauth_token
 *  OUT : verifier
 */
void twitter_verifier (const char * direct_token_url, const char * oauth_token, char ** verifier)
{
    direct_token_result = oauth_http_get(direct_token_url, NULL);
    
    //<< reset variable values
    needle_keychar1 = 34;	//<< 34 corresponds to the English characters "
    needle_keychar1_times = 4;	//<< the fourth 
    needle_keychar2 = 34;
    needle_keychar2_times = 5;	//<< the fifth

    //<< get authenticity_token value
    twit_func_get_authenticity_token(direct_token_web_script, needle_keychar1, needle_keychar1_times, needle_keychar2, needle_keychar2_times, &authenticity_token_value);
}

















