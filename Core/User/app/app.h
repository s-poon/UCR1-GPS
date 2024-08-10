/*
 * app.h
 *
 *  Created on: Aug 9, 2024
 *      Author: Steven
 */

#ifndef USER_APP_APP_H_
#define USER_APP_APP_H_

void RTOS_Init(void);

#define UARTBUFFERLENGTH 255

void StartGPS(void *argument);

#endif /* USER_APP_APP_H_ */
