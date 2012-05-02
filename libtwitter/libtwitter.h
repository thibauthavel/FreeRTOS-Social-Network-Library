/**
 *  @brief      Social Network Library - Twitter
 *  @details    https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *  @file       libtwitter.h
 *  @author     Thibaut Havel
 *  @date       2012
 */
 
 
 /*-----------------------------------------------------------*/


#ifndef LIB_TWITTER_H
#define LIB_TWITTER_H


/** \struct twitterAuthEntity
 * Twitter authentication entity
 */ 
typedef struct tae
{
    char * user_id;             ///< User identifier
    char * user_screen_name;    ///< User screen name
    char * consumer_key;        ///< Consumer token (key)
    char * consumer_secret;     ///< Consumer token (secret)
    char * access_key;          ///< Access token (key)
    char * access_secret;       ///< Access token (secret)
} twitterAuthEntity;


/** \struct tweetEntity
 * Tweet entity
 */
typedef struct te
{
    char * tweet_id;            ///< Tweet identifier ('id')
    char * tweet_date;          ///< Tweet date ('created_at')
    char * user_screen_name;    ///< Tweet author ('user' > 'screen_name')
    char * tweet_text;          ///< Tweet text-based content ('text')
} tweetEntity;


/**
 * Main authentication function gathering the whole authentication process to Twitter.
 *
 * @param consumer_key Consumer token (key) provided by Twitter.
 * @param consumer_secret Consumer token (secret) provided by Twitter.
 * @param user_screen_name The user account login.
 * @param user_password The user account password.
 * @return A authentication entity.
 */
twitterAuthEntity twitter_authentication(const char *consumer_key, const char *consumer_secret, const char *user_screen_name, const char *user_password);


/**
 * Main receiving function getting all tweets from a user timeline.
 *
 * @param auth The authentication entity.
 * @param tweets To store the received tweets.
 * @return The number of received tweets.
 */
int twitter_receive_tweets(const twitterAuthEntity auth, tweetEntity **tweets);


/**
 * Main sending function posting a tweet to the user timeline.
 *
 * @param auth The authentication entity.
 * @param tweet The message to send.
 * @return A tweet entity representing the sent tweet. 
 */
tweetEntity twitter_send_tweet (const twitterAuthEntity auth, const char * tweet);

#endif
