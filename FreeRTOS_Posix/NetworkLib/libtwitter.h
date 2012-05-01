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


void twitter_timeline_user_url(const char *url_init,const char *url_init_type,const char *consumer_key,const char *consumer_secret,const char *access_token,const char *access_token_secret,const char *access_token_user_name,char **url);
void twitter_direct_token_pin(const char *script,unsigned keychar1,unsigned keychar1_times,unsigned keychar2,unsigned keychar2_times,char **pin);
void twitter_direct_token_authenticity(const char *script,unsigned keychar1,unsigned keychar1_times,unsigned keychar2,unsigned keychar2_times,char **authenticity);
void twitter_direct_token_url2(const char *direct_token_url,const char *authenticity,const char *direct_token,const char *user,const char *password,const char *submit_value,int signin_flag,char **direct_token_url2,char **direct_token_prm2);
void twitter_tweet(const char *url,const char *param,char **script);
void twitter_tweet_url(const char *status,const char *consumer_key,const char *consumer_secret,const char *access_token,const char *access_token_secret,char **url,char **param);
void twitter_timeline_user(const char *consumer_key,const char *consumer_secret,const char *access_token,const char *access_token_secret,const char *access_token_user_name,char **timeline_user_result);
void twitter_access_token(const char *access_token_url,char **access_token,char **access_token_secret,char **access_token_user_name,char **access_token_user_id);
void twitter_access_token_url(const char *consumer_key,const char *consumer_secret,const char *oauth_token,const char *oauth_token_secret,const char *verifier,char **access_token_url);
void twitter_verifier(const char *direct_token_url, const char *oauth_token, const char *user_screen_name, const char *user_password, char **verifier);
void twitter_direct_token_url(const char *oauth_token,char **direct_token_url);
void twitter_request_token(const char *request_token_url,char **oauth_token,char **oauth_token_secret,char **callback);
void twitter_request_token_url(const char *consumer_key,const char *consumer_secret,char **request_token_url);
void twitterizer(void);
void xml_parser_getall(const char *xml_content,const char *element_key,char *element_value[]);
int xml_parser_count(const char *xml_content,const char *element_key);
char *xml_parser_get(const char *xml_content,const char *element_key);
int isubstr(const char *cs,const char *ct);
char *xstrtrim(const char *str_src);
int xsubstr(const char *in_str,unsigned in_start,unsigned in_end,char **out_substr);
int xstrchr2(const char *in_str,unsigned in_chr,unsigned in_chr_times);
char *xstrcat(const char *s1,const char *s2);

#endif

