/*
 * HMI_ECU.c
 *
 *  Created on: Dec 15, 2020
 *      Author: Amr
 ******************************************************************************************************************************
 *                                                 Description                                                                *
 ******************************************************************************************************************************
 *       This source file is for an ECU to control security of a Door Lock , the program
 *      uses LCD , KEYPAD , UART and TIMER Modules , the first time to open the program it will ask you
 *      to enter a New password twice then it will give you an option to (=) for opening the door or
 *      the option (+) to change the password , in both options you are asked to enter the password
 *      created at the first time of usage , If you entered a wrong password you are given 2 more
 *      attempts , After that a Buzzer connected to the CONTOL_ECU will be turned on for a minute,
 *      NOTES : the program functionality may differ depending on simulation due to the delay functions ,
 *      it depends on the OS speed of the PC or LAPTOP ,this problem is faced when running simulation
 *      on PROTEUS 8 due to real time issues, the program is handled to run perfect on my OS
 *      , Hope it does for yours author Amr Mohameds well.
 ******************************************************************************************************************************/

/***  Includes for modules files used ***/
#include "keypad.h"
#include "lcd.h"
#include "used_functions.h"
#include "timer0_config.h"
#include "uart_config.h"

/*** Important MACRO definitions ***/

/**  These definations is used by UART module synchronization **/
#define OPTION 0x50
#define NEW_PASS 0x60
#define BUZZER 0x70
#define MOTOR 0x80
/*******************************************************************/

/** These definations in used by TIMER module for counting seconds **/
#define OPEN_CLOSE_TIME 15
#define HOLD_TIME 3
#define BUZZER_TIME 60
/*********************************************************************/

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
/********************************************************************************************************************************
 *                                  ** Main Application **
 *******************************************************************************************************************************/
int main(void)
{
	/* This variable takes the return value of the function KeyPad_getPressedKey() from KEYPAD module */
	uint8 key;
	/* This variable takes the return value of the function LOCK_enterPass(,) which is described in source file (used_functions) */
	uint8 flag ;
	/* This array of characters stores the input password String from the user */
	char passStr1[7];
	/* This array of characters stores re_enterd new password then the Right password received from EEPROM in  CONTROL_ECU
	 * its main usage is for comparing with the entered password from the user */
	char passStr2[7];
	/* set Global interrupt flag */
	SREG  |= ( 1<<7 ) ;


	/* This function is responsible for initialization of the UART module for AVR MCP */
	UART_init(&Uart_Config);
	/* This function is responsible for initialization of the LCD module for AVR MCP */
	LCD_init();
	/* This function is responsible for passing the TIMER_counter function to the TIMER module ISR */
	Timer0_OVF_setCallBack(TIMER_counter);

	/* This function is responsible for sending the new password set by the user to the CONTROL_ECU (described in used_functions.c) */
	LOCK_sendNewPass(passStr1,passStr2);
	/* This function is responsible for receiving the right password saved at first time in EEPROM in CONTOL_ECU
	 *  (described in used_functions.c) */
	LOCK_receiveRightPass(passStr2);


	while(1)
	{
		/* This function is responsible for clearing the LCD screen */
		LCD_clearScreen();
		/* This function is responsible for displaying the string "+ : Change Pass" from the first Row & Column */
		LCD_displayStringRowColumn(0,0,"+ : Change Pass ");
		/* This function is responsible for displaying the string "= : Open door" from the second Row & first Column */
		LCD_displayStringRowColumn(1,0,"= : Open door ");
		/* This while loop function is to hold the program until the user enter (+) or (=) */
		while( (KeyPad_getPressedKey()!= '=') && (KeyPad_getPressedKey() != '+') ){}
		/* storing the return value of the KEYPAD in the variable key */
		key = KeyPad_getPressedKey() ;
		/* This delay function is for handling the key pressed to be once for single press*/
		_delay_ms(2000);

		/***** This if else statements is for the options selected by the user in case of (=) Open door or (+) Change password *****/
		if(key == '+')
		{
			/* The LOCK_enterPass functions returns 0 if the user entered a wrong password 3 times in a row and returns 1 if the user
			 * entered the right password */
			flag = LOCK_enterPass(passStr1,passStr2);
			if(flag == 0)
			{
				/* if flag = 0 the HMI_ECU sends to the CONTOL_ECU that the BUZZER must be turned on and the HMI_ECU displays BUZZER!!! on
				 * the LCD screen for 60 seconds  ***************/
				UART_sendByte(OPTION,&Uart_Config);
				UART_sendByte(BUZZER,&Uart_Config);
				LCD_clearScreen();
				LCD_displayString("BUZZER!!!");
				/* This function is responsible for counting 60 seconds using TIMER Module (described in used_functions.c)*/
				LOCK_timerCount(BUZZER_TIME);
				/* After displaying BUZZER for 60 seconds the HMI_ECU returns to the main options screen */
				continue;
			}
			/* send to the CONTROL_ECU that the option chosen is change pass */
			UART_sendByte(OPTION,&Uart_Config);
			UART_sendByte(NEW_PASS,&Uart_Config);
			/* This function is responsible for sending the new password set by the user to the CONTROL_ECU (described in used_functions.c) */
			LOCK_sendNewPass(passStr1,passStr2);
			/* This function is responsible for receiving the right password saved at first time in EEPROM in CONTOL_ECU (described in used_functions.c) */
			LOCK_receiveRightPass(passStr2);
		}
		else
		{
			/* The LOCK_enterPass functions returns 0 if the user entered a wrong password 3 times in a row and returns 1 if the user
			 * entered the right password */
			flag =LOCK_enterPass(passStr1,passStr2);
			if(flag == 0)
			{
				/* if flag = 0 the HMI_ECU sends to the CONTOL_ECU that the BUZZER must be turned on and the HMI_ECU displays BUZZER!!! on
				 * the LCD screen for 60 seconds  ***************/
				UART_sendByte(OPTION,&Uart_Config);
				UART_sendByte(BUZZER,&Uart_Config);
				LCD_clearScreen();
				LCD_displayString("BUZZER!!!");
				/* This function is responsible for counting 60 seconds using TIMER Module (described in used_functions.c)*/
				LOCK_timerCount(BUZZER_TIME);
				/* After displaying BUZZER for 60 seconds the HMI_ECU returns to the main options screen */
				continue;
			}
			/* send to the CONTROL_ECU that the option chosen is Opening the door for 15 seconds then hold 3 seconds and close for 15 seconds */
			UART_sendByte(OPTION,&Uart_Config);
			UART_sendByte(MOTOR,&Uart_Config);
			/* displaying "Opening..." in the LCD screen for 15 seconds */
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Opening ... ");
			/* This function is responsible for counting 15 seconds ( opening time ) using TIMER Module (described in used_functions.c)*/
			LOCK_timerCount(OPEN_CLOSE_TIME);
			/* displaying "hold.." in the LCD screen for 3 seconds */
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"hold..");
			/* This function is responsible for counting 3 seconds ( hold time )using TIMER Module (described in used_functions.c)*/
			LOCK_timerCount(HOLD_TIME);
			/* displaying "closing..." in the LCD screen for 15 seconds */
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"closing ... ");
			/* This function is responsible for counting 15 seconds ( closing time ) using TIMER Module (described in used_functions.c)*/
			LOCK_timerCount(OPEN_CLOSE_TIME);
		}
	}
	return 0;
}
