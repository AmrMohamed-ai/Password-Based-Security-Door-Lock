/*****************************************************************************************************************
 *[FILE NAME]: used_functions.h
 *
 *[AUTHOR]: Amr Mohamed
 *
 *[DATE CREATED]: 12/12/2020
 *
 *[DESCRIPTION]: This file contains important functions prototypes used in the HMI_ECU , Includes of used Modules in this file and global variables externed from main file.
 *
 ************************************************************************************************************************/

#ifndef USED_FUNCTIONS_H_
#define USED_FUNCTIONS_H_

/********** * * * * * * Includes and Definitions * * * * * * ***********/

#include "lcd.h"
#include "keypad.h"
#include "timer0_config.h"
#include <string.h>
#include "uart_config.h"
#define FINISHED_NEW_PASS 0x30
#define FINISHED_PASS 0x40
#define M_READY 0x10

/* this global variable is responsible for counting seconds for the timer Module */
extern uint8 g_timer ;
/* this global variable is responsible for counting the number of overflows for the timer Module */
extern uint8 g_sec ;
/* this structure declaration is for the Timer module configuration */
extern Timer0_ConfigType Timer0_Config;
/* this structure declaration is for the UART module configuration */
extern Uart_ConfigType Uart_Config;


/****************************************************************************************************************************
 ***************************************** Functions Prototypes *************************************************************
 **************************************************************************************************************************/

/* This function is responsible for getting password from user and check if its right */
uint8 LOCK_enterPass(char *a_str_PTR1 , char*a_str_PTR2 );
/* This function is responsible for receiving the right password saved at first time in EEPROM in CONTOL_ECU (described in used_functions.c) */
void LOCK_receiveRightPass(char * a_str_PTR);
/* This function is responsible for sending the new password set by the user to the CONTROL_ECU (described in used_functions.c) */
void LOCK_sendNewPass(char *a_str_PTR1 , char *a_str_PTR2);
/* This function is responsible for getting any entered string from user twice (described in used_functions.c) */
void LOCK_pass(char *a_str_PTR , char *a_lcdDisplayStr_PTR);
/* This function is responsible for counting seconds using TIMER Module (described in used_functions.c)*/
void LOCK_timerCount(uint8 a_time);


#endif /* USED_FUNCTIONS_H_ */
