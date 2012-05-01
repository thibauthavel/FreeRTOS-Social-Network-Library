/*
 *	FreeRTOS Social Network Library
 *	https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *	
 *	Author: Thibaut HAVEL
 *	Date:   2012
 *
 */
 
 
 /*-----------------------------------------------------------*/


#ifndef LIB_TWITTER_H
#define LIB_TWITTER_H

typedef struct tae
{
    char * user_id;
    char * user_screen_name;
    char * consumer_key;
    char * consumer_secret;
    char * access_key;
    char * access_secret;
} twitterAuthEntity;

typedef struct te
{
    char * tweet_id;
    char * tweet_date;
    char * user_screen_name;
    char * tweet_text; 
} tweetEntity;


twitterAuthEntity twitter_authentication(const char *consumer_key, const char *consumer_secret, const char *user_screen_name, const char *user_password);

int twitter_receive_tweets(const twitterAuthEntity auth, tweetEntity **tweets);

tweetEntity twitter_send_tweet (const twitterAuthEntity auth, const char * tweet);

#endif
