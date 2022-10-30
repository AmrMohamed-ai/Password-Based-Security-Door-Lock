/*****************************************************************************************************************
 *[FILE NAME]: used_functions.h
 *
 *[AUTHOR]: Amr Mohamed
 *
 *[DATE CREATED]: 12/12/2020
 *
 *[DESCRIPTION]: This file contains important functions prototypes used in the CONTOL_ECU , Includes of used Modules in this file and global variables externed from main file.
 *
 ************************************************************************************************************************/


#ifndef USED_FUNCTIONS2_H_
#define USED_FUNCTIONS2_H_

/********** * * * * * * Includes and Definitions * * * * * * ***********/

#include "uart_config.h"
#include <string.h>
#include "external_eeprom.h"
#include "timer0_config.h"
#define M_READY 0x10
#define FINISHED_NEW_PASS 0x30
#define FINISHED_PASS 0x40
#define PASS_ADDRESS_EEPROM 0x0311


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

/* This function is responsible for receiving Newpassword entered by the user in HMI_ECU. (described in used_functions2.c) */
void LOCK_receiveAndSaveNewPass(char * a_str_PTR);
/* This function is responsible for sending the password saved in the EEPROM to HMI_ECU. (described in used_functions2.c) */
void LOCK_sendRightPass(char * a_str_PTR);
/* This function is responsible for counting seconds using TIMER Module (described in used_functions2.c)*/
void LOCK_timerCount(uint8 a_time);

#endif /* USED_FUNCTIONS2_H_ */
