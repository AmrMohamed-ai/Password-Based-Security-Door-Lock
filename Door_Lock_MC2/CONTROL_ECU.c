/*
 * CONTROL.c
 **  Created on: Dec 15, 2020
 *      Author: Amr
 ******************************************************************************************************************************
 *                                                 Description                                                                *
 ******************************************************************************************************************************
 *        This source file is responsible for controling the door lock motor and buzzer , it  also communicates with an EEPROM
 *        through the TWI MODULE , the main function of this program is to receive the entered pass from the user in HMI_ECU
 *        then save it in the EEPROM , and then sends the save password in the EEPROM to HMI_ECU , there are 3 operations this
 *        program does , 1st to activate the motor during opening or closing the door , 2nd to activate the Buzzer in case of
 *        wrong password indicated by HMI_ECU , 3rd is to update the saved password in the EEPROM .
 ******************************************************************************************************************************/


/*** Includes for modules files used ***/

#include "uart_config.h"
#include "external_eeprom.h"
#include "Dc_Motor.h"
#include "used_functions2.h"
#include "timer0_config.h"
#include <string.h>

/*** Important MACRO definitions ***/

/**  These definations is used by UART module synchronization **/
#define OPTION 0x50
#define NEW_PASS 0x60
#define BUZZER 0x70
#define MOTOR 0x80
/****************************************************************/

/** These definations in used by TIMER module for counting seconds **/
#define OPEN_CLOSE_TIME 15
#define HOLD_TIME 3
#define BUZZER_TIME 60
/********************************************************************/

/* this global variable is responsible for counting seconds for the timer Module */
uint8 g_timer = 0;
/* this global variable is responsible for counting the number of overflows for the timer Module */
uint8 g_sec = 0;
/* this structure declaration is for the Timer module configuration */
Timer0_ConfigType Timer0_Config = {NORMAL_MODE,0,F_CPU_1024};
/* this structure declaration is for the UART module configuration */
Uart_ConfigType Uart_Config = {8,9600,Disabled,ONE_STOP_BIT,ASynchronous,DoubleSpeed};

void TIMER_counter(void)
{
	/********************************************************************************************************************************
	 * [Function Name] : TIMER_counter
	 * [Description] : This function is responsible for incrementing Timer Seconds and Overflows its passed to the ISR of the timer
	 *                 Through a pointer to function in the function Timer0_OVF_setCallBack.
	 * [Args]        : The function takes no arguments .
	 * [Returns]     : The function returns void .
	 *
	 *********************************************************************************************************************************/
	g_sec++;
	if(g_sec == 30)
	{
		g_timer++;
		g_sec = 0;
	}
}


int main (void)
{
	/* This variable is responsible for storing the option sent from HMI_ECU */
	uint8 option ;
	/* This array of characters stores the recieved new password from HMI_ECU */
	char passStr3[7] ;
	/* This array of characters stores the password saved in EEPROM and is sent to HMI_ECU */
	char passStr4[7];
	/* configure pin 4 in PORTD as output pin */
	SET_BIT(DDRD,PD4);
	/* set Global interrupt flag */
	SREG  |= ( 1<<7 ) ;
	/* This function is responsible for initialization of the UART module for AVR MCP */
	UART_init(&Uart_Config);
	/* This function is responsible for passing the TIMER_counter function to the TIMER module ISR */
	Timer0_OVF_setCallBack(TIMER_counter);
	/* This function is responsible for initialization of the MOTOR for AVR MCP */
	MOTOR_init();
	/* This function is responsible for initialization of the External EEPROM */
	EEPROM_init();
	_delay_ms(10);
	/* This function is responsible for receiving Newpassword entered by the user in HMI_ECU. (described in used_functions2.c) */
	LOCK_receiveAndSaveNewPass(passStr3);
	/* This function is responsible for sending the password saved in the EEPROM to HMI_ECU. (described in used_functions2.c) */
	LOCK_sendRightPass(passStr4);


	while(1)
	{

		/* This while loop waits for HMI_ECU to send the option that CONTROL_ECU will make */
		while(UART_receiveByte(&Uart_Config)!=OPTION){}
		option = UART_receiveByte(&Uart_Config);
		/* If option is equal to NEW_PASS the program will save a new password in the EEPROM , if option is
		 * equal to BUZZER the CONTROL_ECU will activate the buzzer , else the CONTROL_ECU will open the door then close it */
		if( option == NEW_PASS )
		{
			LOCK_receiveAndSaveNewPass(passStr3);

			LOCK_sendRightPass(passStr4);

		}
		else if( option == BUZZER)
		{
			SET_BIT(PORTD,PD4);       // BUZZER ON
			/* This function is responsible for counting 60 seconds (BUZZER_TIME) using TIMER Module (described in used_functions2.c)*/
			LOCK_timerCount(BUZZER_TIME);
			CLEAR_BIT(PORTD,PD4);    // BUZZER OFF
		}
		else
		{

			MOTOR_forward();   // MOTOR CLOCKWISE
			/* This function is responsible for counting 15 seconds ( opening time ) using TIMER Module (described in used_functions2.c)*/
			LOCK_timerCount(OPEN_CLOSE_TIME);



			MOTOR_stop();     // MOTOR STOP
			/* This function is responsible for counting 3 seconds ( hold time ) using TIMER Module (described in used_functions.c)*/
			LOCK_timerCount(HOLD_TIME);


			MOTOR_reverse();  // MOTOR ANTICLOCKWISE
			/* This function is responsible for counting 15 seconds ( closing time ) using TIMER Module (described in used_functions.c)*/
			LOCK_timerCount(OPEN_CLOSE_TIME);
			MOTOR_stop();
		}

	}

	return 0;
}
