/*
 *  FreeRTOS Social Network Library
 *  https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *  
 *  Author: Thibaut HAVEL
 *  Date:   2012
 *
 */


/*
 * INCLUDES AND DEFINES
 *-----------------------------------------------------------*/


/* System headers */
#include <stdio.h>
#include <string.h>

/* Other requiered library */
#include "oauth.h"

/* Library header */
#include "libtwitter.h"


/*
 * STATIC FIELDS
 *-----------------------------------------------------------*/


#define TWITTER_REQUEST_TOKEN_URL  "https://api.twitter.com/oauth/request_token"
#define TWITTER_DIRECT_TOKEN_URL   "https://api.twitter.com/oauth/authorize"
#define TWITTER_ACCESS_TOKEN_URL   "https://api.twitter.com/oauth/access_token"
#define TWITTER_SEND_TWEET_URL     "http://twitter.com/statuses/update.xml"
#define TWITTER_TIMELINE_USER_URL  "http://twitter.com/statuses/user_timeline.xml"

#define TWITTER_TWEET_SIZE         140



/*
 * TOOL FUNCTIONS
 *-----------------------------------------------------------*/


char * xstrcat (const char *s1, const char *s2) 
{
    int ptr_len = 0;
    char *ptr = xstrdup(s1);

    ptr_len = strlen(ptr) + strlen(s2);

    ptr = xrealloc(ptr, (ptr_len + 1) * sizeof(char));
    
    strcat(ptr, s2);

    return ptr;
}


/*-----------------------------------------------------------*/


int xstrchr2 (const char *in_str, unsigned in_chr, unsigned in_chr_times)
{
    int in_strchr_len;
    int rtn_strchr_len;
    int rtn_code = 0;
    int chr_in_str_pos = 0;
    unsigned i;
    
    char *in_strchr = NULL;
    char *rtn_strchr = NULL;
    
    in_strchr = xstrdup(in_str);


    if (0 == in_chr_times)
    {
        in_chr_times = 1;
    }
    
    for (i=0; i<in_chr_times; i++)
    {        
        rtn_strchr = strchr(in_strchr, in_chr);
        
        in_strchr_len = strlen(in_strchr);
        rtn_strchr_len = strlen(rtn_strchr);
        
        chr_in_str_pos += in_strchr_len - rtn_strchr_len;
        
        if (i > 0)    
        {
            chr_in_str_pos += 1;
        }
        
        xsubstr(rtn_strchr, 1, 0, &in_strchr);
    }
    
    rtn_code = chr_in_str_pos;
    
    return rtn_code;
}


/*-----------------------------------------------------------*/


int xsubstr(const char *in_str, unsigned in_start, unsigned in_end, char **out_substr)
{
    int substr_len = 0;
    int rtn_code = 0;
    char *substr = NULL;
    int continue_flag = 0;

    
    if (0 == in_end)
    {
        substr_len = strlen(in_str) - in_start;
        continue_flag = 1;
    }
    else if (in_start <= in_end)
    {
        substr_len = in_end - in_start;
        continue_flag = 1;
    }
    else
    {
        continue_flag = 1;
        rtn_code = -1;
    }


    if (1 == continue_flag)
    {
        substr = xrealloc (substr, substr_len+1);    
        memset(substr, 0, substr_len+1);

        strncpy(substr, in_str+in_start, substr_len);
    }
    
    substr[substr_len] = 0;
    *out_substr = substr;    
    
    return rtn_code;
}


/*-----------------------------------------------------------*/


char * xstrtrim (const char *str_src)
{
    char *str = xstrdup(str_src);
    char *str2 = NULL;
    int str_src_len = strlen(str);

    str2 = xrealloc(str2, str_src_len+1);
    memset(str2, 0, str_src_len+1);

    int i, j = 0;

    for (i = 0; i < str_src_len; i++)       
    {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
        {
            continue;
        }
        else
        {                   
            str2[j] = str[i];
            j++;
        }
    }   

    str2[j] = 0;

    return str2;
}


/*-----------------------------------------------------------*/


int isubstr (const char * cs, const char * ct)
{
    int index = -1;

    if (cs != NULL && ct != NULL)
    {
        char *ptr_pos = NULL;

        ptr_pos = strstr (cs, ct);
        if (ptr_pos != NULL)
        {
            index = ptr_pos - cs;
        }
    }
    return index;
}


/*-----------------------------------------------------------*/


char * xml_parser_get (const char * xml_content, const char * element_key)
{

    char * element_begin;
    int    element_begin_size;
    char * element_end;
    int    index_begin;
    int    index_end;
    char * value;

    element_begin = xstrdup("<");
    element_begin = xstrcat(element_begin, element_key);
    element_begin = xstrcat(element_begin, ">");
    element_begin_size = strlen(element_begin);
    
    element_end = xstrdup("</");
    element_end = xstrcat(element_end, element_key);
    element_end = xstrcat(element_end, ">");
    
    index_begin = isubstr(xml_content, element_begin);
    index_end = isubstr(xml_content, element_end);
    
    if(index_begin != -1)
    {
        xsubstr(xml_content, (index_begin + element_begin_size), index_end, &value);
        return value;
    }
    
    return NULL;
}


/*-----------------------------------------------------------*/


int xml_parser_count (const char * xml_content, const char * element_key)
{
    char * buffer;
    char * element_begin;
    int    count;
    int    buffer_position;
    size_t element_begin_size;
    
    if(strlen(element_key)== 0)   return 0;

    element_begin = xstrdup("<");
    element_begin = xstrcat(element_begin, element_key);
    element_begin = xstrcat(element_begin, ">");
    element_begin_size = strlen(element_begin);
    
    buffer = strdup(xml_content);
    buffer_position = isubstr(buffer, element_begin);
    count  = 0;
    
    while(buffer_position != -1)
    {   
        count++;
        
        xsubstr(buffer, (buffer_position + element_begin_size), strlen(buffer)-1, &buffer);
        buffer_position = isubstr(buffer, element_begin);
    }
    
    return count;
}


/*-----------------------------------------------------------*/


void xml_parser_getall (const char * xml_content, const char * element_key, char * element_value[])
{
    char * buffer;
    char * element_end;
    int    buffer_position;
    size_t element_end_size;
    
    if(strlen(element_key)== 0)   return 0;

    element_end = xstrdup("</");
    element_end = xstrcat(element_end, element_key);
    element_end = xstrcat(element_end, ">");
    element_end_size = strlen(element_end);
    
    buffer = strdup(xml_content);
    buffer_position = isubstr(buffer, element_end);
    
    int i = 0;
    while(buffer_position != -1)
    {
        element_value[i] = xml_parser_get(buffer, element_key);
        i++;
        //printf("element=%s\n", xml_parser_get(buffer, element_key));
        
        xsubstr(buffer, (buffer_position + element_end_size), strlen(buffer)-1, &buffer);
        buffer_position = isubstr(buffer, element_end);
    }
}



/*
 * MAIN FUNCTIONS
 *-----------------------------------------------------------*/
 
 
/*
 *  IN     : consumer_key
 *  IN     : consumer_secret
 *  IN     : user_screen_name
 *  IN     : user_password
 *  RETURN : twitterAuthEntity
 */

twitterAuthEntity twitter_authentication (const char * consumer_key, const char * consumer_secret, const char * user_screen_name, const char * user_password)
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
    
    twitterAuthEntity auth;

    twitter_request_token_url(consumer_key, consumer_secret, &request_token_url);
    twitter_request_token(request_token_url, &oauth_token, &oauth_token_secret, &callback);
    
    twitter_direct_token_url(oauth_token, &direct_token_url);
    twitter_verifier(direct_token_url, oauth_token, user_screen_name, user_password, &verifier);

    twitter_access_token_url(consumer_key, consumer_secret, oauth_token, oauth_token_secret, verifier, &access_token_url);
    twitter_access_token(access_token_url, &access_token, &access_token_secret, &access_token_user_id, &access_token_user_name);

    auth.user_id = xstrdup(access_token_user_id);
    auth.user_screen_name = xstrdup(access_token_user_name);
    auth.consumer_key = xstrdup(consumer_key);
    auth.consumer_secret = xstrdup(consumer_secret);
    auth.access_key = xstrdup(access_token);
    auth.access_secret = xstrdup(access_token_secret);

    return auth;
}


/*
 *  IN     : twitterAuthEntity
 *  OUT    : tweetEntity[]
 *  RETURN : count
 */

int twitter_receive_tweets(const twitterAuthEntity auth, tweetEntity ** tweets)
{
    char * timeline_user;

    twitter_timeline_user(auth.consumer_key, auth.consumer_secret, auth.access_key, auth.access_secret, auth.user_screen_name, &timeline_user);

    int count_tweets = xml_parser_count(timeline_user, "text");

    char * tweet_id[count_tweets];
    char * tweet_date[count_tweets];
    char * tweet_text[count_tweets];
    xml_parser_getall(timeline_user, "id", tweet_id);
    xml_parser_getall(timeline_user, "created_at", tweet_date);
    xml_parser_getall(timeline_user, "text", tweet_text);

    tweetEntity result[count_tweets];
    int i;
    for(i = 0 ; i < count_tweets ; i++)
    {
        result[i].tweet_id = xstrdup(tweet_id[i]);
        result[i].tweet_date = xstrdup(tweet_date[i]);
        result[i].user_screen_name = xstrdup(auth.user_screen_name);
        result[i].tweet_text = xstrdup(tweet_text[i]);
    }

    *tweets = result;
    return count_tweets;
}


/*
 *  IN     : auth
 *  IN     : tweet
 *  RETURN : tweetEntity
 */

tweetEntity twitter_send_tweet (const twitterAuthEntity auth, const char * tweet)
{
    char * tweet_url;
    char * tweet_param;
    char * tweet_result;

    tweetEntity result;

    twitter_tweet_url(tweet, auth.consumer_key, auth.consumer_secret, auth.access_key, auth.access_secret, &tweet_url, &tweet_param);
    
    twitter_tweet(tweet_url, tweet_param, &tweet_result);

    result.tweet_id = xml_parser_get(tweet_result, "id");
    result.tweet_date = xml_parser_get(tweet_result, "created_at");
    result.user_screen_name = auth.user_screen_name;
    result.tweet_text = xml_parser_get(tweet_result, "text");

    return result;
}



/*
 * AUTHENTICATION FUNCTIONS
 *-----------------------------------------------------------*/


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
 *  IN  : oauth_token
 *  OUT : direct_token_url
 */
void twitter_direct_token_url (const char * oauth_token, char ** direct_token_url)
{
    char * url_tmp;

    url_tmp = xstrdup(TWITTER_DIRECT_TOKEN_URL);
    url_tmp = xstrcat(url_tmp, "?oauth_token=");
    url_tmp = xstrcat(url_tmp, oauth_token);
    
    *direct_token_url = url_tmp;
}


/*
 *  IN  : direct_token_url
 *  IN  : authenticity
 *  IN  : direct_token
 *  IN  : user
 *  IN  : password
 *  IN  : submit_value
 *  IN  : signin_flag
 *  OUT : direct_token_url2
 *  OUT : direct_token_prm2
 */
void twitter_direct_token_url2 (const char * direct_token_url, const char * authenticity, const char * direct_token, const char * user, const char * password, const char * submit_value, int signin_flag, char ** direct_token_url2, char ** direct_token_prm2)
{
    const char * authenticity_str = xstrdup("authenticity_token");
    const char * oauth_token_str  = xstrdup("oauth_token");
    const char * equal_str        = xstrdup("=");
    const char * and_str          = xstrdup("&");
    const char * user_str         = xstrdup("session%5Busername_or_email%5D");  // session%5Busername_or_email%5D
    const char * password_str     = xstrdup("session%5Bpassword%5D");           // session%5Bpassword%5D
    const char * deny_str         = xstrdup("cancel");
    
    char * direct_token_url2_tmp = NULL;
    char * direct_token_prm2_tmp = NULL;
    
    direct_token_url2_tmp = xstrdup(direct_token_url);
    
    direct_token_prm2_tmp = xstrdup(authenticity_str);
    direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, equal_str);
    direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, authenticity);
    
    direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, and_str);
    direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, oauth_token_str);
    direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, equal_str);
    direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, direct_token);
    
    if(0 == signin_flag)
    {
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, and_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, user_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, equal_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, user);
        
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, and_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, password_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, equal_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, password);
    }
    else if(!strcmp(submit_value, "Deny"))
    {
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, and_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, deny_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, equal_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2_tmp, submit_value);
    }

    *direct_token_url2 = direct_token_url2_tmp;
    *direct_token_prm2 = direct_token_prm2_tmp;
}


/*
 *  IN  : direct_token_url
 *  IN  : oauth_token
 *  IN  : user_screen_name
 *  IN  : user_password
 *  OUT : verifier
 */
void twitter_verifier (const char * direct_token_url, const char * oauth_token, const char * user_screen_name, const char * user_password, char ** verifier)
{
    char * direct_token_script;
    char * direct_token_authenticity;
    char * direct_token_url2;
    char * direct_token_prm2;
    char * direct_token_script2;
    char * direct_token_pin;

    direct_token_script = oauth_http_get(direct_token_url, NULL);
    
    unsigned needle_keychar1 = 34;      // corresponds to the character "
    unsigned needle_keychar2 = 34;
    unsigned needle_keychar1_times = 4; 
    unsigned needle_keychar2_times = 5;
    twitter_direct_token_authenticity(direct_token_script, needle_keychar1, needle_keychar1_times, needle_keychar2, needle_keychar2_times, &direct_token_authenticity);
    
    twitter_direct_token_url2(direct_token_url, direct_token_authenticity, oauth_token, user_screen_name, user_password, NULL, 0,  &direct_token_url2, &direct_token_prm2);
    
    direct_token_script2 = oauth_http_post(direct_token_url2, direct_token_prm2);
    
    needle_keychar1 = 62;               // corresponds to the character >
    needle_keychar2 = 60;               // corresponds to the character <
    needle_keychar1_times = 2;
    needle_keychar2_times = 2;
    twitter_direct_token_pin(direct_token_script2, needle_keychar1, needle_keychar1_times, needle_keychar2, needle_keychar2_times, &direct_token_pin);
    
    *verifier = direct_token_pin;
}


/*
 *  IN  : script
 *  IN  : keychar1
 *  IN  : keychar1_times
 *  IN  : keychar2
 *  IN  : keychar2_times
 *  OUT : authenticity
 */
void twitter_direct_token_authenticity (const char * script, unsigned keychar1, unsigned keychar1_times, unsigned keychar2, unsigned keychar2_times, char ** authenticity)
{
    int needle_keychar1_pos;
    int needle_keychar2_pos;
    char * needle_keyword;
    char * hay_stack;
    char * authenticity_value;

    needle_keyword = xstrdup("authenticity_token");
    hay_stack = strstr(script, needle_keyword);
    needle_keychar1_pos = xstrchr2(hay_stack, keychar1, keychar1_times);
    needle_keychar2_pos = xstrchr2(hay_stack, keychar2, keychar2_times);
    xsubstr(hay_stack, needle_keychar1_pos + 1, needle_keychar2_pos, &authenticity_value);

    *authenticity = authenticity_value;
}


/*
 *  IN  : script
 *  IN  : keychar1
 *  IN  : keychar1_times
 *  IN  : keychar2
 *  IN  : keychar2_times
 *  OUT : pin
 */
void twitter_direct_token_pin (const char * script, unsigned keychar1, unsigned keychar1_times, unsigned keychar2, unsigned keychar2_times, char ** pin)
{
    int needle_keychar1_pos;
    int needle_keychar2_pos;
    char * needle_keyword;
    char * hay_stack;
    char * pin_value;
    
    needle_keyword = xstrdup("kbd"); // oauth_pin
    hay_stack = strstr(script, needle_keyword);
    needle_keychar1_pos = xstrchr2(hay_stack, keychar1, keychar1_times);
    needle_keychar2_pos = xstrchr2(hay_stack, keychar2, keychar2_times);
    xsubstr(hay_stack, needle_keychar1_pos + 1, needle_keychar2_pos, &pin_value);
    
    *pin = pin_value;
}


/*
 *  IN  : consumer_key
 *  IN  : consumer_secret
 *  IN  : oauth_token
 *  IN  : oauth_token_secret
 *  IN  : verifier
 *  OUT : url
 */
void twitter_access_token_url (const char * consumer_key, const char * consumer_secret, const char * oauth_token, const char * oauth_token_secret, const char * verifier, char ** access_token_url)
{
    char * url_tmp;
    
    url_tmp = oauth_sign_url2(TWITTER_ACCESS_TOKEN_URL, NULL, OA_HMAC, NULL, consumer_key, consumer_secret, oauth_token, oauth_token_secret);
    url_tmp = xstrcat(url_tmp, "&oauth_verifier=");
    url_tmp = xstrcat(url_tmp, verifier);
    
    *access_token_url = url_tmp;
}


/*
 *  IN  : access_token_url
 *  OUT : access_token
 *  OUT : access_token_secret
 *  OUT : access_token_user_name
 *  OUT : access_token_user_id
 */
void twitter_access_token(const char * access_token_url, char ** access_token, char ** access_token_secret, char ** access_token_user_name, char ** access_token_user_id)
{
    char * access_token_result;

    // Using OAuth to get the result
    access_token_result = oauth_http_get(access_token_url, NULL);
    
    // Split results into variables
    *access_token           = strtok(access_token_result, "&");
    *access_token_secret    = strtok(NULL, "&");
    *access_token_user_name = strtok(NULL, "&");
    *access_token_user_id   = strtok(NULL, "&");
    
    strtok(*access_token, "=");
    *access_token = strtok(NULL, "=");
    
    strtok(*access_token_secret, "=");
    *access_token_secret = strtok(NULL, "=");
    
    strtok(*access_token_user_name, "=");
    *access_token_user_name = strtok(NULL, "=");
    
    strtok(*access_token_user_id, "=");
    *access_token_user_id = strtok(NULL, "=");
}



/*
 * USER BEHAVIOURS FUNCTIONS
 *-----------------------------------------------------------*/
 

/*
 *  IN  : url_init
 *  IN  : url_init_type
 *  IN  : consumer_key
 *  IN  : consumer_secret
 *  IN  : access_token
 *  IN  : access_token_secret
 *  IN  : access_token_user_name
 *  OUT : url
 */
void twitter_timeline_user_url(const char * url_init, const char * url_init_type, const char * consumer_key, const char * consumer_secret, const char * access_token, const char * access_token_secret, const char * access_token_user_name, char ** url)
{
    char * timeline_user_url;

    if (url_init_type == 1)
    {
        timeline_user_url = xstrdup(url_init);
        timeline_user_url = xstrcat(timeline_user_url, "/");
        timeline_user_url = xstrcat(timeline_user_url, access_token_user_name);
    }
    else
    {
        timeline_user_url = xstrdup(url_init);
    }

    timeline_user_url = oauth_sign_url2(timeline_user_url, NULL, OA_HMAC, NULL, consumer_key, consumer_secret, access_token, access_token_secret);

    *url = timeline_user_url;
}


/*
 *  IN  : consumer_key
 *  IN  : consumer_secret
 *  IN  : access_token
 *  IN  : access_token_secret
 *  IN  : access_token_user_name
 *  OUT : timeline_user_result
 */
void twitter_timeline_user (const char * consumer_key, const char * consumer_secret, const char * access_token, const char * access_token_secret, const char * access_token_user_name, char ** timeline_user_result)
{
    char * timeline_user_url;
    char * timeline_user_url_init;
    int    timeline_user_url_init_type;  // 0 = xml  ,  1 = url
    char * timeline_user_result_tmp;

    timeline_user_url_init = xstrdup(TWITTER_TIMELINE_USER_URL);
    timeline_user_url_init_type = 0;

    twitter_timeline_user_url(timeline_user_url_init, timeline_user_url_init_type, consumer_key, consumer_secret, access_token, access_token_secret, access_token_user_name, &timeline_user_url);
    timeline_user_result_tmp = oauth_http_get(timeline_user_url, NULL);
    
    *timeline_user_result = timeline_user_result_tmp;
}


/*
 *  IN  : status
 *  IN  : consumer_key
 *  IN  : consumer_secret
 *  IN  : access_token
 *  IN  : access_token_secret
 *  OUT : url
 *  OUT : param
 */
void twitter_tweet_url (const char * status, const char * consumer_key, const char * consumer_secret, const char * access_token, const char * access_token_secret, char ** url, char ** param)
{
    char * url_tmp;
    char * param_tmp; 
    
    url_tmp = xstrdup(TWITTER_SEND_TWEET_URL);
    url_tmp = xstrcat(url_tmp, "?");
    url_tmp = xstrcat(url_tmp, "status=");
    url_tmp = xstrcat(url_tmp, status);
    
    url_tmp = oauth_sign_url2(url_tmp, &param_tmp, OA_HMAC, NULL, consumer_key, consumer_secret, access_token, access_token_secret);

    *url   = url_tmp;
    *param = param_tmp;
}


/*
 *  IN  : url
 *  IN  : param
 *  OUT : script
 */
void twitter_tweet (const char * url, const char * param, char ** script)
{
    char * result = oauth_http_post(url, param);
    
    *script = result;
}

