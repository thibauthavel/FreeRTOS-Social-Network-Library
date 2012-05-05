/*
 *	FreeRTOS Social Network Library
 *	https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *	
 *	Author: Thibaut HAVEL
 *	Date:   2012
 *
 */
 
 
 /*-----------------------------------------------------------*/


#ifndef TWITTER_TASKS_H
#define TWITTER_TASKS_H

void startTwitterTask (unsigned portBASE_TYPE uxPriority);
portBASE_TYPE xAreTwitTaskStillRunning (void);

#endif
