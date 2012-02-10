/*
 *	FreeRTOS Social Network Library
 *	https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *	
 *	Author: Thibaut HAVEL
 *	Date:   08/02/2012
 *
 *  First library test.
 *
 */


/*-----------------------------------------------------------*/


/* System headers */
#include <stdio.h>
#include <string.h>

/* FreeRTOS headers */
#include "FreeRTOS.h"
#include "task.h"

/* Library header */
#include "libtest.h"


#include <math.h>
#include <openssl/bio.h>



/*-----------------------------------------------------------*/


void sayHelloTo (const char * const someone)
{
	vTaskSuspendAll();
	{
        const char *k = (unsigned char*)'k';
        BIO_new_mem_buf((unsigned char*) k, strlen(k));


		printf("Hello, %s ! The result is %f.\r\n", someone, floor(3.2));
		fflush(stdout);
	}
	xTaskResumeAll();
}

