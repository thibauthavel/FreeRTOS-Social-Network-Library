/*
 *	FreeRTOS Social Network Library
 *	https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *	
 *	Author: Thibaut HAVEL
 *	Date:   2012
 *
 */
 
 
 /*-----------------------------------------------------------*/


#ifndef TWITTERSEND_TASKS_H
#define TWITTERSEND_TASKS_H

void startTwitterSendTask (unsigned portBASE_TYPE uxPriority);
portBASE_TYPE xAreTwitSTaskStillRunning (void);

#endif
