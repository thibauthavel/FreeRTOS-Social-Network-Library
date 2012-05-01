/*
 *	FreeRTOS Social Network Library
 *	https://github.com/thibauthavel/FreeRTOS-Social-Network-Library
 *	
 *	Author: Thibaut HAVEL
 *	Date:   2012
 *
 */
 
 
 /*-----------------------------------------------------------*/


#ifndef NET_TASKS_H
#define NET_TASKS_H

void startNetworkTask (unsigned portBASE_TYPE uxPriority);
portBASE_TYPE xAreNetTaskStillRunning (void);

#endif
