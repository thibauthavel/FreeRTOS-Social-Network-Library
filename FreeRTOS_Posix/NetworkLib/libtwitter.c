/*
 *    FreeRTOS Social Network Library
 *    https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *    
 *    Author: Thibaut HAVEL
 *    Date:   13/02/2012
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



#define TWITTER_REQUEST_TOKEN_URL   "https://api.twitter.com/oauth/request_token"
#define TWITTER_DIRECT_TOKEN_URL    "https://api.twitter.com/oauth/authorize"
#define TWITTER_ACCESS_TOKEN_URL    "https://api.twitter.com/oauth/access_token"


/*-----------------------------------------------------------*/


#define USER_SCREEN_NAME    "thibaut_havel"
#define USER_EMAIL          "********"
#define USER_ID             "********"
#define USER_PASSWORD       "********"	
#define CONSUMER_KEY        "VB5FifD1HLhmLmsj8tZA"
#define CONSUMER_SECRET     "OdE18zFiva5TsC3rPQmq9BlYXhfBPFWJq2bY6Ib40"
#define OAUTH_TOKEN         "488884688-gBSVVjk722PjHgPx5mBPY1wTsL0Bmlv8QAazwgMy"
#define OAUTH_TOKEN_SECRET  "6jLaHA3wLRiUGAidt9q4doIx3IXX8U1D6ntyUuVMN1g"


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

        // Step 4 : Verifier
        printf("\n\nStep 4 --------------------------------\n\n");
        twitter_verifier(direct_token_url, oauth_token, &verifier);
        
        printf("End of test\n");
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
 *  IN  : oauth_token
 *  OUT : direct_token_url
 */
void twitter_direct_token_url (const char * oauth_token, char ** direct_token_url)
{
    strcpy(*direct_token_url, TWITTER_DIRECT_TOKEN_URL);
    strcat(*direct_token_url, "?oauth_token=");
    strcat(*direct_token_url, oauth_token);
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
    const char * user_str         = xstrdup("session%5Busername_or_email%5D");
    const char * password_str     = xstrdup("session%5Bpassword%5D");
    const char * deny_str         = xstrdup("cancel");
    
    char * direct_token_url2_tmp = NULL;
    char * direct_token_prm2_tmp = NULL;
    
    direct_token_url2_tmp = xstrdup(direct_token_url);
    
    direct_token_prm2_tmp = xstrdup(authenticity_str);
    direct_token_prm2_tmp = xstrcat(direct_token_prm2, equal_str);
    direct_token_prm2_tmp = xstrcat(direct_token_prm2, authenticity);
    
    direct_token_prm2_tmp = xstrcat(direct_token_prm2, and_str);
    direct_token_prm2_tmp = xstrcat(direct_token_prm2, oauth_token_str);
    direct_token_prm2_tmp = xstrcat(direct_token_prm2, equal_str);
    direct_token_prm2_tmp = xstrcat(direct_token_prm2, direct_token);
    
    if(0 == signin_flag)
    {
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, and_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, user_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, equal_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, user);
        
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, and_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, password_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, equal_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, password);
    }
    
    if(!strcmp(submit_value, "Deny"))
    {
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, and_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, deny_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, equal_str);
        direct_token_prm2_tmp = xstrcat(direct_token_prm2, submit_value);
    }

    *direct_token_url2 = direct_token_url2_tmp;
    *direct_token_prm2 = direct_token_prm2_tmp;
}


/*
 *  IN  : direct_token_url
 *  IN  : oauth_token
 *  OUT : verifier
 */
void twitter_verifier (const char * direct_token_url, const char * oauth_token, char ** verifier)
{
    char * direct_token_script;
    char * direct_token_authenticity;
    char * direct_token_url2;
    char * direct_token_prm2;
    char * direct_token_script2;

    unsigned needle_keychar1 = 34;      // corresponds to the English characters "
    unsigned needle_keychar2 = 34;
    unsigned needle_keychar1_times = 4; 
    unsigned needle_keychar2_times = 5;

    direct_token_script = oauth_http_get(direct_token_url, NULL);
    
    twitter_direct_token_authenticity(direct_token_script, needle_keychar1, needle_keychar1_times, needle_keychar2, needle_keychar2_times, &direct_token_authenticity);
    printf("direct_token_authenticity=%s\n", direct_token_authenticity);
    
    twitter_direct_token_url2(direct_token_url, direct_token_authenticity, oauth_token, USER_SCREEN_NAME, USER_PASSWORD, NULL, 0,  &direct_token_url2, &direct_token_prm2);
    direct_token_script2 = oauth_http_post(direct_token_url2, direct_token_prm2);
    printf("direct_token_script2=%s\n", direct_token_script2);
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



// -------------------------------------------------------------------------------



// Extend define string function
/*
static void *xmalloc_fatal(size_t size) 
{    
    if (size==0) 
    {        
        return NULL;
    }
    
    fprintf(stderr, "Out of memory.");
    
    exit(1);
}


void *xmalloc (size_t size) 
{
    void *ptr = malloc (size);
    
    if (ptr == NULL) 
    {
        return xmalloc_fatal(size);
    }
    
    return ptr;
}


void *xcalloc (size_t nmemb, size_t size) 
{
    void *ptr = calloc (nmemb, size);
    
    if (ptr == NULL) 
    {
        return xmalloc_fatal(nmemb*size);
    }
    
    return ptr;
}


void *xrealloc (void *ptr, size_t size) 
{
    void *p = realloc (ptr, size);
    
    if (p == NULL) 
    {
        return xmalloc_fatal(size);
    }
    
    return p;
}

char *xstrdup (const char *s) 
{
    void *ptr = xmalloc(strlen(s) + 1);    

    strcpy (ptr, s);
    
    return (char*) ptr;
}
*/

char *xstrcat (const char *s1, const char *s2) 
{
    int ptr_len = 0;
    char *ptr = xstrdup(s1);

    ptr_len = strlen(ptr) + strlen(s2);

    ptr = xrealloc(ptr, (ptr_len + 1) * sizeof(char));
    
    strcat(ptr, s2);

    return ptr;
}

char *xstrtrim(const char *str_src)
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

int xstrchr(const char*str_1, int c)
{
    int chr_position = -1;

    int len_str_1 = 0;
    int len_substr = 0;
    char *substr = NULL;
    
    len_str_1 = strlen(str_1);

    substr = strchr(str_1, c);

    if ( substr )
    {
        len_substr = strlen(substr);
        
        if (len_str_1 >= len_substr)
        {
            chr_position = len_str_1 - len_substr;
        }
        else
        {
            chr_position = -2;
        }
    }


    return chr_position;
}

int xstrchr2(const char *in_str, unsigned in_chr, unsigned in_chr_times)
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

int xstrtok(const char *in_str_token, const char *in_str_delimit, char **out_str_token1, char **out_str_token0)
{
    int rtn_code = 0;

    char *str_token0 = xstrdup(in_str_token);
    char *str_token1 = NULL;

    int str_token0_len = 0;
    int str_token1_len = 0;
    int substr_len = 0;

    str_token0_len = strlen(str_token0);
    
    if (str_token0_len > 0)
    {
        str_token1 = strstr(str_token0, in_str_delimit);

        if ( str_token1 )
        {
            rtn_code = 1;

            str_token1_len = strlen(str_token1);            

            substr_len = str_token0_len - str_token1_len;

            xsubstr(str_token0, 0, substr_len, &str_token1);

            xsubstr(str_token0, substr_len + strlen(in_str_delimit), 0, &str_token0);
        }
        else
        {
            rtn_code = 2;
        }        
    }

    *out_str_token1 = str_token1;
    *out_str_token0 = str_token0;

    return rtn_code;
}

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


int xsubstr2(const char *in_source_str, const char *in_needle_keyword1, const char *in_needle_keyword2, char **out_substr)
{
    int rtn_code = 0;
    int hay_stack1_len = 0;
    int hay_stack2_len = 0;
    int substr_len = 0;

    char *hay_stack1 = NULL;
    char *hay_stack2 = NULL;

    char *substr = NULL;


    hay_stack1 = strstr(in_source_str, in_needle_keyword1);
    hay_stack2 = strstr(hay_stack1, in_needle_keyword2);

    hay_stack1_len = strlen(hay_stack1);
    hay_stack2_len = strlen(hay_stack2);
    substr_len = hay_stack1_len - hay_stack2_len + strlen(in_needle_keyword2);

    xsubstr(hay_stack1, 0, substr_len, &substr);

    *out_substr = substr;

    return rtn_code;
}

